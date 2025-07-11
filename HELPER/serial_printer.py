import tkinter as tk
from tkinter import scrolledtext, messagebox, ttk
import numpy as np
import time
import threading
import ctypes
import mmap
import os
import subprocess
from datetime import datetime
import matplotlib
matplotlib.use('Agg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

BAUD_RATE = 115200
DEFAULT_MAX_POINTS = 1024

# 定义 Windows API 函数
kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)

CreateFileMapping = kernel32.CreateFileMappingW
CreateFileMapping.argtypes = [
    ctypes.c_void_p,  # hFile
    ctypes.c_void_p,  # lpAttributes
    ctypes.c_uint32,  # flProtect
    ctypes.c_uint32,  # dwMaximumSizeHigh
    ctypes.c_uint32,  # dwMaximumSizeLow
    ctypes.c_wchar_p  # lpName
]
CreateFileMapping.restype = ctypes.c_void_p

MapViewOfFile = kernel32.MapViewOfFile
MapViewOfFile.argtypes = [
    ctypes.c_void_p,  # hFileMappingObject
    ctypes.c_uint32,  # dwDesiredAccess
    ctypes.c_uint32,  # dwFileOffsetHigh
    ctypes.c_uint32,  # dwFileOffsetLow
    ctypes.c_size_t   # dwNumberOfBytesToMap
]
MapViewOfFile.restype = ctypes.c_void_p

UnmapViewOfFile = kernel32.UnmapViewOfFile
UnmapViewOfFile.argtypes = [ctypes.c_void_p]  # lpBaseAddress
UnmapViewOfFile.restype = ctypes.c_bool

CloseHandle = kernel32.CloseHandle
CloseHandle.argtypes = [ctypes.c_void_p]  # hObject
CloseHandle.restype = ctypes.c_bool

# 定义共享数据结构
class SharedData(ctypes.Structure):
    _fields_ = [
        ("running", ctypes.c_int),
        ("data_count", ctypes.c_int),
        ("data", ctypes.c_double * 1024),
        ("raw_data", ctypes.c_char * (1024 * 64)),
        ("raw_data_len", ctypes.c_int)
    ]

class SerialMonitor(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("High-Speed Serial Monitor (Windows)")
        self.geometry("1200x800")
        
        # 初始化共享内存相关变量
        self.hMapFile = None
        self.shm_ptr = None
        self.shared_data = None
        self.reader_process = None
        
        # 尝试连接到共享内存
        try:
            # 创建共享内存
            self.hMapFile = CreateFileMapping(
                ctypes.c_void_p(-1),  # INVALID_HANDLE_VALUE
                None,                # 安全属性
                0x04,                # PAGE_READWRITE
                0,                   # 最大大小高32位
                1024 * 1024,         # 最大大小低32位 (1MB)
                "Local\\SerialMonitorShm"
            )
            
            if not self.hMapFile:
                raise ctypes.WinError(ctypes.get_last_error())
                
            # 映射共享内存
            self.shm_ptr = MapViewOfFile(
                self.hMapFile,
                0x0002 | 0x0004,     # FILE_MAP_WRITE | FILE_MAP_READ
                0,
                0,
                0
            )
            
            if not self.shm_ptr:
                raise ctypes.WinError(ctypes.get_last_error())
            
            # 创建共享数据结构
            self.shared_data = ctypes.cast(self.shm_ptr, ctypes.POINTER(SharedData)).contents
            
            # 启动C程序
            self.start_c_reader()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to initialize shared memory: {str(e)}")
            # 创建虚拟共享数据
            self.shared_data = SharedData()
            self.shared_data.running = 0
        
        self.port = None
        self.running = False
        self.data_values = []
        self.max_points = DEFAULT_MAX_POINTS
        self._create_widgets()
        self.last_update_time = time.time()
        
        # 启动数据更新线程
        self.update_thread = threading.Thread(target=self.update_data, daemon=True)
        self.update_thread.start()
        
        # 定期刷新UI
        self.after(20, self.update_ui)

    def start_c_reader(self):
        """启动C语言数据采集程序"""
        try:
            # 检查C程序是否存在
            if not os.path.exists("serial_reader.exe"):
                messagebox.showwarning("Warning", "C reader executable not found")
                return
                
            # 启动C程序
            self.reader_process = subprocess.Popen(["serial_reader.exe"])
        except Exception as e:
            messagebox.showwarning("Warning", f"Failed to start C reader: {str(e)}")

    def _create_widgets(self):
        # 创建控制面板
        control_frame = ttk.Frame(self, padding=10)
        control_frame.pack(fill=tk.X)
        
        # 端口选择
        ttk.Label(control_frame, text="Port:").grid(row=0, column=0, sticky=tk.W)
        self.port_var = tk.StringVar(value="COM5")
        port_entry = ttk.Entry(control_frame, textvariable=self.port_var, width=20)
        port_entry.grid(row=0, column=1, padx=5)
        
        # 模式选择
        ttk.Label(control_frame, text="Mode:").grid(row=0, column=2, sticky=tk.W, padx=(10, 0))
        self.mode_var = tk.StringVar(value="Graphic Output")
        mode_combo = ttk.Combobox(
            control_frame, textvariable=self.mode_var, 
            values=["String Output", "Graphic Output"], 
            state="readonly", width=15
        )
        mode_combo.grid(row=0, column=3, padx=5)
        
        # 数据点设置
        ttk.Label(control_frame, text="Max Points:").grid(row=0, column=4, sticky=tk.W, padx=(10, 0))
        self.points_var = tk.IntVar(value=self.max_points)
        points_entry = ttk.Entry(control_frame, textvariable=self.points_var, width=8)
        points_entry.grid(row=0, column=5, padx=5)
        
        # 控制按钮
        self.toggle_btn = ttk.Button(control_frame, text="Start", command=self.toggle_running)
        self.toggle_btn.grid(row=0, column=6, padx=(20, 0))
        
        ttk.Button(control_frame, text="Clear", command=self.clear_data).grid(row=0, column=7, padx=5)
        
        # 状态栏
        self.status_var = tk.StringVar(value="Ready")
        status_bar = ttk.Label(control_frame, textvariable=self.status_var)
        status_bar.grid(row=0, column=8, padx=(20, 5), sticky=tk.E)
        
        # 创建主显示区域
        main_frame = ttk.Frame(self)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        
        # 文本输出区域
        self.text = scrolledtext.ScrolledText(
            main_frame, font=("Consolas", 10), wrap=tk.WORD
        )
        self.text.pack(fill=tk.BOTH, expand=True)
        self.text.config(state=tk.DISABLED)
        
        # 图形输出区域
        self.figure = Figure(figsize=(10, 6), dpi=100)
        self.ax = self.figure.add_subplot(111)
        self.ax.grid(True, linestyle='--', alpha=0.7)
        self.line, = self.ax.plot([], [], 'b-', linewidth=1.5)
        self.ax.set_xlabel("Sample Index")
        self.ax.set_ylabel("Value")
        self.ax.set_title("Real-time Serial Data")
        
        self.canvas = FigureCanvasTkAgg(self.figure, main_frame)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        self.canvas.get_tk_widget().pack_forget()
        
        # 绑定事件
        self.mode_var.trace_add('write', self.on_mode_change)
        self.points_var.trace_add('write', self.on_max_points_change)

    def on_mode_change(self, *args):
        """切换显示模式"""
        mode = self.mode_var.get()
        if mode == "String Output":
            self.canvas.get_tk_widget().pack_forget()
            self.text.pack(fill=tk.BOTH, expand=True)
        else:
            self.text.pack_forget()
            self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
            self.update_plot()

    def on_max_points_change(self, *args):
        """更新最大数据点数"""
        try:
            self.max_points = max(100, min(10000, int(self.points_var.get())))
            self.points_var.set(self.max_points)
            if len(self.data_values) > self.max_points:
                self.data_values = self.data_values[-self.max_points:]
            self.update_plot()
        except:
            pass

    def toggle_running(self):
        """切换运行状态"""
        self.running = not self.running
        self.toggle_btn.config(text="Stop" if self.running else "Start")
        self.status_var.set("Running..." if self.running else "Stopped")
        
        if self.running:
            self.clear_data()
            # 通知C程序开始运行
            if self.shared_data:
                self.shared_data.running = 1
        else:
            # 通知C程序停止
            if self.shared_data:
                self.shared_data.running = 0

    def clear_data(self):
        """清除数据"""
        self.data_values = []
        self.text.config(state=tk.NORMAL)
        self.text.delete(1.0, tk.END)
        self.text.config(state=tk.DISABLED)
        self.update_plot()

    def update_data(self):
        """从共享内存更新数据"""
        while True:
            if self.running and hasattr(self, 'shared_data') and self.shared_data:
                # 获取新数据点
                data_count = self.shared_data.data_count
                if data_count > 0:
                    # 直接访问共享内存中的数据
                    new_data = list(self.shared_data.data)[:data_count]
                    if new_data:
                        self.data_values.extend(new_data)
                        if len(self.data_values) > self.max_points:
                            self.data_values = self.data_values[-self.max_points:]
                
                # 获取原始数据
                raw_data_len = self.shared_data.raw_data_len
                if raw_data_len > 0:
                    raw_data_bytes = bytes(self.shared_data.raw_data[:raw_data_len])
                    try:
                        raw_text = raw_data_bytes.decode('utf-8', errors='ignore')
                        if raw_text and self.mode_var.get() == "String Output":
                            self.append_text(raw_text)
                    except:
                        pass
                    
                    # 重置原始数据缓冲区
                    self.shared_data.raw_data_len = 0
                
                # 重置数据计数
                self.shared_data.data_count = 0
            
            time.sleep(0.001)  # 1ms休眠

    def append_text(self, text):
        """追加文本到显示区域"""
        self.text.config(state=tk.NORMAL)
        ts = datetime.now().strftime("%Y.%m.%d %H:%M:%S")
        self.text.insert(tk.END, f"{ts} {text}\n")
        self.text.see(tk.END)
        self.text.config(state=tk.DISABLED)

    def update_plot(self):
        """更新图表"""
        if not self.data_values:
            self.ax.clear()
            self.ax.grid(True, linestyle='--', alpha=0.7)
            self.ax.set_title("Real-time Serial Data (No data)")
            self.ax.set_xlabel("Sample Index")
            self.ax.set_ylabel("Value")
            self.canvas.draw()
            return
        
        x = list(range(len(self.data_values)))
        y = self.data_values
        
        self.ax.clear()
        self.ax.grid(True, linestyle='--', alpha=0.7)
        self.ax.plot(x, y, 'b-', linewidth=1.5)
        
        # 设置标题和标签
        self.ax.set_title(f"Real-time Serial Data ({len(self.data_values)} points)")
        self.ax.set_xlabel("Sample Index")
        self.ax.set_ylabel("Value")
        
        # 自动调整范围
        self.ax.relim()
        self.ax.autoscale_view()
        
        self.canvas.draw()

    def update_ui(self):
        """更新用户界面"""
        if self.running and self.mode_var.get() == "Graphic Output":
            # 计算FPS
            current_time = time.time()
            elapsed = current_time - self.last_update_time
            fps = 1.0 / elapsed if elapsed > 0 else 0
            self.last_update_time = current_time
            
            # 更新状态
            self.status_var.set(f"Running... | FPS: {fps:.1f} | Points: {len(self.data_values)}/{self.max_points}")
            
            # 更新图表
            self.update_plot()
        
        self.after(20, self.update_ui)  # 约50FPS

    def on_close(self):
        """关闭应用程序"""
        # 通知C程序停止
        if hasattr(self, 'shared_data') and self.shared_data:
            self.shared_data.running = 0
        
        # 清理共享内存
        if self.shm_ptr:
            UnmapViewOfFile(self.shm_ptr)
        
        if self.hMapFile:
            CloseHandle(self.hMapFile)
        
        # 终止C程序
        if hasattr(self, 'reader_process') and self.reader_process:
            try:
                self.reader_process.terminate()
            except:
                pass
        
        self.destroy()

if __name__ == "__main__":
    app = SerialMonitor()
    app.protocol("WM_DELETE_WINDOW", app.on_close)
    app.mainloop()