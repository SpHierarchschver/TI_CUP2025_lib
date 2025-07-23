#ifndef DEMODULATION_H
#define DEMODULATION_H

#include "basic.h"

#define LEAK_VAL_THRESHOLD  1
#define LEAK_NUM_THRESHOLD  20
#define BFSK_VAL_THRESHOLD  0.2
#define BFSK_NUM_THRESHOLD  100
#define ENV_DEV_MEAN_RATIO  0.7

typedef enum
{
  UNKNOWN_MOD_TYPE,
  AM,
  FM,
  PM,
  BASK,
  BFSK,
  BPSK,
} ModType;

typedef struct
{
  ModType modType;
  float32_t envMean, envDev, fftMean, fftDev;
} ModTypeDebug;


ModTypeDebug judge_mod_type (float32_t signalIn[], int N, float32_t Fs);
void demod (float32_t signalIn[], float32_t signalOut[], int N, float32_t sampleRate, ModType signalModType);

#endif /* demodulation.h */
