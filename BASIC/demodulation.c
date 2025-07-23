#include "demodulation.h"
#include "filter.h"
#include "fft.h"
#include "func_gen.h"

complex_t cmplxOut[FFT_MAX_LEN];
SortIns qsortIns[FFT_MAX_LEN];
float32_t env[FFT_MAX_LEN*2];

static int is_BFSK (float32_t fftIn[], int N, float32_t thresholdVal, int thresholdNum);

static void demod_AM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_FM (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);
static void demod_PM (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);
static void demod_BASK (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_BFSK (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);
static void demod_BPSK (float32_t signalIn[], float32_t signalOut[], int N, float32_t Fs);

ModTypeDebug
judge_mod_type (float32_t signalIn[], int N, float32_t Fs)
{
  for (int i = 0; i < N; ++i)
    env[i] = f32abs (signalIn[i]);

  float32_t envMean = mean (env, N);
  float32_t envDev = squdev (env, N, envMean);
  arm_sqrt_f32 (envDev, &envDev);

  rfft (signalIn, N, cmplxOut, env, NO_WIN);
  // print_arr_f (env, N/2+1);

  float32_t fftMean = mean (env, N/2+1);
  float32_t fftDev = squdev (env, N/2+1, fftMean);
  arm_sqrt_f32 (fftDev, &fftDev);

  ModTypeDebug debug;
  debug.envMean = envMean;
  debug.envDev = envDev;
  debug.fftMean = fftMean;
  debug.fftDev = fftDev;
  
  if (is_spectrum_leak (env, N/2+1, LEAK_VAL_THRESHOLD, LEAK_NUM_THRESHOLD))
  {
    /* BA/F/PSK. */
    if (envDev > ENV_DEV_MEAN_RATIO * envMean)
      debug.modType = BASK;
    else
    {
      if (is_BFSK (env, N/2+1, BFSK_VAL_THRESHOLD, BFSK_NUM_THRESHOLD))
        debug.modType = BFSK;
      else
        debug.modType = BPSK;
    }
  }
  else
  {
    /* A/F/PM. */
    if (envDev > ENV_DEV_MEAN_RATIO * envMean)
      debug.modType = AM;
    else
      /* It's impossible to tell FM and PM apart without prior. We consider it as FM as default. */
      debug.modType = FM;
  }

  return debug;
}

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

static int
is_BFSK (float32_t fftIn[], int N, float32_t thresholdVal, int thresholdNum)
{
  int sum = 0;

  for (int i = 0; i < N; ++i)
  {
    if (fftIn[i] > thresholdVal)
      ++sum;
  }

  if (sum > thresholdNum)
    return 0;

  return 1;
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
