#include "demodulation.h"
#include "filter.h"
#include "fft.h"
#include "func_gen.h"

complex_t cmplxOut[FFT_MAX_LEN];
SortIns qsortIns[FFT_MAX_LEN];

static void demod_AM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_FM (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);
static void demod_PM (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);
static void demod_BASK (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_BFSK (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);
static void demod_BPSK (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);

void
demod (float32_t signalIn[], float32_t signalOut[], int N, float32_t sampleRate, ModType signalModType)
{
  switch (signalModType)
  {
  case AM:
    demod_AM (signalIn, signalOut, N);
    break;

  case FM:
    demod_FM (signalIn, signalOut, N, sampleRate);
    break;

  case PM:
    demod_PM (signalIn, signalOut, N, sampleRate);
    break;

  case BASK:
    demod_BASK (signalIn, signalOut, N);
    break;

  case BFSK:
    demod_BFSK (signalIn, signalOut, N, sampleRate);
    break;

  case BPSK:
    demod_BPSK (signalIn, signalOut, N, sampleRate);
    break;
  
  default:
    break;
  }
}

static void
demod_AM (float32_t signalIn[], float32_t signalOut[], int N)
{
  // print_arr_f (signalIn, N);
  
  for (int i = 0; i < N; ++i)
    signalIn[i] = f32abs (signalIn[i]);

  // print_arr_f (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs512k_Fc20k_O50_COEF, LPF_Fs512k_Fc20k_O50_ORDER);

  // print_arr_f (signalOut, N);
}

static void
demod_FM (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs)
{
  rfft (signalIn, N, cmplxOut, signalOut, NO_WIN);
  // print_arr_f (signalOut, N/2);

  init_qsort (signalOut, qsortIns, N/2);
  qsort (qsortIns, N/2, DESCENDING);
  float32_t bandFreq = ((float32_t)qsortIns[1].idx - (float32_t)qsortIns[0].idx) * Fs / N;
  gen_sine (signalOut, N, 1000, bandFreq, 0, Fs);
  // print_arr_f (signalOut, N);
}

static void
demod_PM (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs)
{
  float32_t meanZero, meanNinety;

  for (int i = 0; i < N; ++i)
    signalOut[i] = signalIn[i] * arm_sin_f32 (2 * PI * 50e3 * i / Fs);

  meanZero = mean (signalOut, N);

  for (int i = 0; i < N; ++i)
    signalOut[i] = signalIn[i] * arm_sin_f32 (2 * PI * 50e3 * i / Fs + PI / 2);

  meanNinety = mean (signalIn, N);

  float32_t phi = arctan (meanNinety / meanZero);

  for (int i = 0; i < N; ++i)
    signalIn[i] *= arm_sin_f32 (2 * PI * 50e3 * i / Fs + phi);

  // print_arr_f (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs1024k_Fc10k_O50_COEF, LPF_Fs1024k_Fc10k_O50_ORDER);

  // print_arr_f (signalOut, N);
}

static void
demod_BASK (float32_t signalIn[], float32_t signalOut[], int N)
{
  // print_arr_f (signalIn, N);
  
  for (int i = 0; i < N; ++i)
    signalIn[i] = f32abs (signalIn[i]);

  // print_arr_f (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs512k_Fc20k_O10_COEF, LPF_Fs512k_Fc20k_O10_ORDER);

  // print_arr_f (signalOut, N);
  
  for (int i = 0; i < N; ++i)
    signalOut[i] = (signalOut[i] > 200 ? 1.0f : 0.0f);
  
  // print_arr_f (signalOut, N);
}

static void
demod_BFSK (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs)
{
  for (int i = 0; i < N; ++i)
    signalIn[i] *= arm_sin_f32 (2 * PI * 50e3 * i / Fs);

  // print_arr_f (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs1024k_Fc60k_O100_COEF, LPF_Fs1024k_Fc60k_O100_ORDER);

  for (int i = 0; i < N; ++i)
    signalIn[i] = signalOut[i];

  // print_arr_f (signalIn, N);

  for (int i = 0; i < N; ++i)
    signalIn[i] = f32abs (signalIn[i]);

  // print_arr_f (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs1024k_Fc10k_O100_COEF, LPF_Fs1024k_Fc10k_O100_ORDER);

  // print_arr_f (signalOut, N);

  for (int i = 0; i < N; ++i)
    signalOut[i] = (signalOut[i] > 200 ? 1.0f : 0.0f);

  // print_arr_f (signalOut, N);
}

static void
demod_BPSK (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs)
{
  float32_t meanZero, meanNinety;

  for (int i = 0; i < N; ++i)
    signalOut[i] = signalIn[i] * arm_sin_f32 (2 * PI * 50e3 * i / Fs);

  meanZero = mean (signalOut, N);

  for (int i = 0; i < N; ++i)
    signalOut[i] = signalIn[i] * arm_sin_f32 (2 * PI * 50e3 * i / Fs + PI / 2);

  meanNinety = mean (signalIn, N);

  float32_t phi = arctan (meanNinety / meanZero);

  for (int i = 0; i < N; ++i)
    signalIn[i] *= arm_sin_f32 (2 * PI * 50e3 * i / Fs + phi);

  // print_arr_f (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs1024k_Fc10k_O20_COEF, LPF_Fs1024k_Fc10k_O20_ORDER);

  // print_arr_f (signalOut, N);

  for (int i = 0; i < N; ++i)
    signalOut[i] = (signalOut[i] >= 0.0) ? 0.0 : 1.0;

  // print_arr_f (signalOut, N);
}
