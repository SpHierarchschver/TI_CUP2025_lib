#include "demodulation.h"
#include "filter.h"

static void multiply_carrier (float32_t signalIn[], int N, float32_t carrierFreq, float32_t sampleRate);
static void lowpass_filter (float32_t signalIn[], float32_t signalOut[], int N);
static ModType judge_mod_type (float32_t signalOut[], int N);

static void demod_AM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_FM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_PM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_ASK2 (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_FSK2 (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_PSK2 (float32_t signalIn[], float32_t signalOut[], int N);

void
demod (float32_t signalIn[], float32_t signalOut[], int N, float32_t carrierFreq, float32_t sampleRate)
{
  ModType signalModType;
  signalModType = AM;

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
    demod_ASK2 (signalIn, signalOut, N);
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
  // print_demod_out (signalIn, N);
  
  for (int i = 0; i < N; ++i)
    signalIn[i] = f32abs (signalIn[i]);

  // print_demod_out (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs512k_Fc20k_O50_COEF, LPF_Fs512k_Fc20k_O50_ORDER);

  print_demod_out (signalOut, N);
}

static void
demod_FM (float32_t signalIn[], float32_t signalOut[], int N)
{

}

static void
demod_PM (float32_t signalIn[], float32_t signalOut[], int N)
{

}

static void
demod_ASK2 (float32_t signalIn[], float32_t signalOut[], int N)
{
  // print_demod_out (signalIn, N);
  
  for (int i = 0; i < N; ++i)
    signalIn[i] = f32abs (signalIn[i]);

  // print_demod_out (signalIn, N);

  fir_filter (N, signalIn, signalOut, LPF_Fs512k_Fc20k_O10_COEF, LPF_Fs512k_Fc20k_O10_ORDER);

  // print_demod_out (signalOut, N);
  
  for (int i = 0; i < N; ++i)
    signalOut[i] = (signalOut[i] > 200 ? 1.0f : 0.0f);
  
  // print_demod_out (signalOut, N);
}

static void
demod_FSK2 (float32_t signalIn[], float32_t signalOut[], int N)
{

}

static void
demod_PSK2 (float32_t signalIn[], float32_t signalOut[], int N)
{

}
