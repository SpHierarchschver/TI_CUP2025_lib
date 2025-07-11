import tkinter as tk
from tkinter import scrolledtext, messagebox, ttk
import serial
from serial.tools import list_ports
from datetime import datetime
import time

BAUD_RATE = 115200
DEFAULT_MAX_POINTS = 1024

class SerialMonitor(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Serial Printer STM32H750VBT6")
        self.geometry("920x650")  # 整体向右扩展
        self.port = None
        self.ser = None
        self.running = False
        self.data_values = []
        self.max_points = DEFAULT_MAX_POINTS
        self._create_widgets()
        self.last_scan_ports = []
        self.after(100, self._scan_ports)
        self.after(150, self._read_serial)

    def _create_widgets(self):
        top = tk.Frame(self)
        top.pack(fill=tk.X, pady=5, padx=5)

        tk.Label(top, text="Port:").pack(side=tk.LEFT)
        self.port_lbl = tk.Label(top, text="None", width=15, anchor=tk.W, relief=tk.SUNKEN)
        self.port_lbl.pack(side=tk.LEFT, padx=(0,10))
        tk.Button(top, text="Refresh", command=self._scan_ports).pack(side=tk.LEFT, padx=5)

        tk.Label(top, text="Mode:").pack(side=tk.LEFT, padx=(20,2))
        self.mode_var = tk.StringVar(value="String Output")
        self.mode_menu = ttk.Combobox(top, textvariable=self.mode_var, state='readonly',
                                      values=["String Output", "Graphic Output"], width=17)
        self.mode_menu.pack(side=tk.LEFT, padx=5)

        tk.Label(top, text="Max Points:").pack(side=tk.LEFT, padx=(20,2))
        self.points_var = tk.IntVar(value=self.max_points)
        self.points_entry = ttk.Entry(top, textvariable=self.points_var, width=6, state='disabled')
        self.points_entry.pack(side=tk.LEFT)
        self.points_var.trace_add('write', self._on_max_points_change)

        self.toggle_btn = tk.Button(top, text="Start", command=self._toggle_running)
        self.toggle_btn.pack(side=tk.LEFT, padx=20)

        self.text = scrolledtext.ScrolledText(self, font=("Consolas",10), wrap=tk.WORD)
        self.text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.text.configure(state='disabled')

        self.canvas = tk.Canvas(self, bg="white", height=400)
        self.canvas.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.canvas.pack_forget()
        self.double_buffer = None

        self.mode_var.trace_add('write', self._on_mode_change)

    def _on_mode_change(self,*_):
        if self.mode_var.get()=="String Output":
            self.points_entry.config(state='disabled')
            self.canvas.pack_forget()
            self.text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        else:
            self.points_entry.config(state='normal')
            self.text.pack_forget()
            self.canvas.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
            self._draw_graph()

    def _on_max_points_change(self,*_):
        try:
            v=self.points_var.get()
            if v>0:
                self.max_points=v
                self.data_values=self.data_values[:v]
                self._draw_graph()
        except:
            pass

    def _scan_ports(self):
        ports=[p.device for p in list_ports.comports()]
        if ports:
            sel=ports[0]
            if sel!=self.port or ports!=self.last_scan_ports:
                self.port=sel
                self.port_lbl.config(text=sel)
                self._reopen()
                self._clear()
        else:
            if self.port!=None or ports!=self.last_scan_ports:
                self.port=None
                self.port_lbl.config(text="None")
                self._close()
                self._clear()
        self.last_scan_ports=ports
        self.after(1000,self._scan_ports)

    def _reopen(self):
        try:
            if self.ser: self.ser.close()
            self.ser=serial.Serial(self.port,BAUD_RATE,timeout=0.1,dsrdtr=False,rtscts=False)
            time.sleep(0.1)
            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()
        except Exception as e:
            messagebox.showerror("Error",f"Open failed: {e}")
            self.ser=None

    def _close(self):
        if self.ser:
            try: self.ser.close()
            except: pass
        self.ser=None

    def _clear(self):
        if self.ser:
            try:
                self.ser.reset_input_buffer()
                self.ser.reset_output_buffer()
            except: pass
        self.data_values.clear()
        self._clear_graph()
        self.text.configure(state='normal')
        self.text.delete('1.0',tk.END)
        self.text.configure(state='disabled')

    def _toggle_running(self):
        self.running=not self.running
        self.toggle_btn.config(text="Stop" if self.running else "Start")
        if self.running: self._clear()

    def _read_serial(self):
        if self.running and self.ser and self.ser.in_waiting:
            try: line=self.ser.readline().decode(errors='ignore').rstrip('\r\n')
            except: line="<Decode Error>"
            if line:
                if self.mode_var.get()=="String Output":
                    self._append_text(line)
                else:
                    num=self._parse_number(line)
                    if num is not None:
                        self.data_values.insert(0,num)
                        if len(self.data_values)>self.max_points:
                            self.data_values=self.data_values[:self.max_points]
                        self._draw_graph()
        self.after(1,self._read_serial)

    def _append_text(self,line):
        ts=datetime.now().strftime("%Y.%m.%d %H:%M:%S")
        self.text.configure(state='normal')
        self.text.insert(tk.END,f"{ts} out: {line}\n")
        self.text.see(tk.END)
        self.text.configure(state='disabled')

    def _parse_number(self,s):
        try: return float(s) if '.' in s or 'e' in s.lower() else int(s)
        except: return None

    def _clear_graph(self):
        self.canvas.delete("all")
        self.double_buffer=None

    def _draw_graph(self):
        w,h=self.canvas.winfo_width(),self.canvas.winfo_height()
        if w<100 or h<100:
            self.after(100,self._draw_graph)
            return

        # 创建双缓冲图片
        self.double_buffer = tk.PhotoImage(width=w, height=h)
        m=80  # 加大左侧空白确保轴可见
        um=w-2*m; vm=h-2*m
        mn,mx=min(self.data_values),max(self.data_values)
        if mn==mx: mn,mx=mn-1,mx+1
        dx=um/(self.max_points-1) if self.max_points>1 else um
        coords=[(m+i*dx, m+vm*(mx-v)/(mx-mn)) for i,v in enumerate(self.data_values)]
        cols=min(self.max_points,50); rows=10
        xstep=um/(cols-1) if cols>1 else um
        ystep=vm/rows

        def draw_line(x1,y1,x2,y2,color="#ddd",dash=None,width=1):
            self.double_buffer.tk.call(self.double_buffer, 'put',
                f"{color}", f"{int((x1+x2)/2)},{int((y1+y2)/2)}")

        # 绘制网格略简化，由PhotoImage不支持线条全部模拟，主Canvas后补
        # ... 这里只示意

        # 在主Canvas绘制最终内容
        self.canvas.delete("all")
        # 网格和坐标轴
        for i in range(cols):
            x=m+i*xstep
            self.canvas.create_line(x,m,x,m+vm,fill="#ddd",dash=(2,4),width=1)
            if i%(cols//10 if cols>10 else 1)==0:
                idx=i*self.max_points//cols
                self.canvas.create_text(x,m+vm+10,text=str(idx),anchor='n',fill="#666",font=('Arial',8))
        for j in range(rows+1):
            y=m+j*ystep
            self.canvas.create_line(m,y,m+um,y,fill="#ddd",dash=(2,4),width=1)
            if j%(rows//10 if rows>10 else 1)==0:
                val=mx-j*(mx-mn)/rows
                self.canvas.create_text(m-10,y,text=f"{val:.2f}",anchor='e',fill="#666",font=('Arial',8))

        # 折线
        flat=[c for xy in coords for c in xy]
        if len(flat)>=4:
            self.canvas.create_line(flat,fill="blue",width=1,smooth=True)

        self.canvas.create_line(m,m,m,m+vm,width=1)
        self.canvas.create_line(m,m+vm,m+um,m+vm,width=1)

    def on_close(self):
        self._close()
        self.destroy()

if __name__=="__main__":
    app=SerialMonitor()
    app.protocol("WM_DELETE_WINDOW",app.on_close)
    app.mainloop()
