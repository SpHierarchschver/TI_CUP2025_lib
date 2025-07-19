#include "demodulation.h"
#include "filter.h"

static void multiply_carrier (float32_t signalIn[], int N, float32_t carrierFreq, float32_t sampleRate);
static void lowpass_filter (float32_t signalIn[], float32_t signalOut[], int N);
static ModType judge_mod_type (float32_t signalOut[], int N);

static void demod_AM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_FM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_PM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_ASK2 (float32_t signalIn[], float32_t signalOut[], int N, float32_t smapleRate);
static void demod_FSK2 (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_PSK2 (float32_t signalIn[], float32_t signalOut[], int N);

void
demod (float32_t signalIn[], float32_t signalOut[], int N, float32_t carrierFreq, float32_t sampleRate)
{
  ModType signalModType;
  signalModType = ASK2;

  switch (signalModType)
  {
  case AM:
    demod_AM (signalIn, signalOut, N);
    break;

  case FM:
    demod_FM (signalIn, signalOut, N);
    break;

  case PM:
    demod_PM (signalIn, signalOut, N);
    break;

  case ASK2:
    demod_ASK2 (signalIn, signalOut, N, sampleRate);
    break;

  case FSK2:
    demod_FSK2 (signalIn, signalOut, N);
    break;

  case PSK2:
    demod_PSK2 (signalIn, signalOut, N);
    break;
  
  default:
    break;
  }

  // print_demod_out (signalOut, N);
}

void
print_demod_out (float32_t signal[], int N)
{
  for (int i = 0; i < N; ++i)
    printf ("%f\r\n", signal[i]);
}

static void
multiply_carrier (float32_t signalIn[], int N, float32_t carrierFreq, float32_t sampleRate)
{

}

static void
lowpass_filter (float32_t signalIn[], float32_t signalOut[], int N)
{
  fir_filter (N, signalIn, signalOut, LPF_Fs512k_Fc100k_O10_COEF, LPF_Fs512k_Fc100k_O10_ORDER);
}

static ModType
judge_mod_type (float32_t signalOut[], int N)
{

}

static void
demod_AM (float32_t signalIn[], float32_t signalOut[], int N)
{

}

static void
demod_FM (float32_t signalIn[], float32_t signalOut[], int N)
{

}

static void
demod_PM (float32_t signalIn[], float32_t signalOut[], int N)
{

}

void biquad_init_lowpass(Biquad *f, float32_t cutoff, float32_t fs) {
  float32_t w0 = 2.0f * PI * cutoff / fs;
  float32_t cosw = arm_cos_f32(w0);
  float32_t sinw = arm_sin_f32(w0);
  float32_t Q = 0.1f;  // 二阶 Butterworth
  float32_t alpha = sinw / (2.0f * Q);

  float32_t b0 = (1.0f - cosw) / 2.0f;
  float32_t b1 = 1.0f - cosw;
  float32_t b2 = b0;
  float32_t a0 = 1.0f + alpha;
  float32_t aa1 = -2.0f * cosw;
  float32_t aa2 = 1.0f - alpha;

  f->b0 = b0 / a0;
  f->b1 = b1 / a0;
  f->b2 = b2 / a0;
  f->a1 = aa1 / a0;
  f->a2 = aa2 / a0;
  // 清零延迟存储
  f->x1 = f->x2 = f->y1 = f->y2 = 0.0f;
}

// Biquad 核心处理函数
static inline float32_t biquad_process(Biquad *f, float32_t x) {
  float32_t y = f->b0 * x + f->b1 * f->x1 + f->b2 * f->x2
            - f->a1 * f->y1 - f->a2 * f->y2;
  f->x2 = f->x1; f->x1 = x;
  f->y2 = f->y1; f->y1 = y;
  return y;
}

static void
demod_ASK2 (float32_t signalIn[], float32_t signalOut[], int N, float32_t sampleRate)
{
  // // 一阶 IIR 低通滤波器系数
  // float32_t rc = 1.0f / (2.0f * PI * 2e4);
  // float32_t dt = 1.0f / sampleRate;
  // float32_t alpha = dt / (rc + dt);
  // float32_t z = 0.0f;

  Biquad lp;
  biquad_init_lowpass(&lp, 24e3, sampleRate);

  // print_demod_out (signalIn, N);

  
  for (int i = 0; i < N; ++i)
    signalIn[i] = f32abs (signalIn[i]);

  // print_demod_out (signalIn, N);

  /*
  for (int i = 0; i < N; ++i)
    signalOut[i] = biquad_process(&lp, signalIn[i]);
  */

  fir_filter (N, signalIn, signalOut, LPF_Fs512k_Fc20k_O10_COEF, LPF_Fs512k_Fc20k_O10_ORDER);

  // print_demod_out (signalOut, N);

  
  for (int i = 0; i < N; ++i)
    signalOut[i] = (signalOut[i] > 200 ? 1.0f : 0.0f);
  
  print_demod_out (signalOut, N);

  /*
  for (int i = 0; i < N; ++i)
  {
    // z += alpha * (f32abs (signalIn[i]) - z);
    // signalOut[i] = z;
    signalOut[i] = (biquad_process(&lp, f32abs (signalIn[i])) > 100) ? 1.0f : 0.0f;
    // signalOut[i] = biquad_process(&lp, f32abs (signalIn[i]));
  }
  */
}

static void
demod_FSK2 (float32_t signalIn[], float32_t signalOut[], int N)
{

}

static void
demod_PSK2 (float32_t signalIn[], float32_t signalOut[], int N)
{

}
