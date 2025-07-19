#include "demodulation.h"
#include "filter.h"

static void demod_AM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_FM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_PM (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_BASK (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_BFSK (float32_t signalIn[], float32_t signalOut[], int N);
static void demod_BPSK (float32_t signalIn[], float32_t signalOut[], int N);

void
demod (float32_t signalIn[], float32_t signalOut[], int N, float32_t sampleRate, ModType signalModType)
{
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

  case BASK:
    demod_BASK (signalIn, signalOut, N);
    break;

  case BFSK:
    demod_BFSK (signalIn, signalOut, N);
    break;

  case BPSK:
    demod_BPSK (signalIn, signalOut, N);
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
demod_FM (float32_t signalIn[], float32_t signalOut[], int N)
{

}

static void
demod_PM (float32_t signalIn[], float32_t signalOut[], int N)
{

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
demod_BFSK (float32_t signalIn[], float32_t signalOut[], int N)
{
  fir_filter (N, signalIn, signalOut, BPF_Fs1024k_Fc59k_61k_O200_COEF, BPF_Fs1024k_Fc59k_61k_O200_ORDER);
  // iir_filter (N, signalIn, signalOut, BPI_Fs1024k_Fc55k_65k_O20_NUM, BPI_Fs1024k_Fc55k_65k_O20_DEN, BPI_Fs1024k_Fc55k_65k_O20_ORDER);

  for (int i = 0; i < N; ++i)
    signalIn[i] = signalOut[i];

  print_arr_f (signalIn, N);

  demod_BASK (signalIn, signalOut, N);

  // print_arr_f (signalOut, N);
}

static void
demod_BPSK (float32_t signalIn[], float32_t signalOut[], int N)
{

}
