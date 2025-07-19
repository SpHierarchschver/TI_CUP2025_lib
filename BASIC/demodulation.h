#ifndef DEMODULATION_H
#define DEMODULATION_H

#include "basic.h"

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

void demod (float32_t signalIn[], float32_t signalOut[], int N, float32_t sampleRate, ModType signalModType);

#endif /* demodulation.h */
