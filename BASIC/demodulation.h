#ifndef DEMODULATION_H
#define DEMODULATION_H

#include "basic.h"

typedef enum
{
  UNKNOWN_MOD_TYPE,
  AM,
  FM,
  PM,
  ASK2,
  FSK2,
  PSK2,
} ModType;

typedef struct {
  // Biquad 系数（Direct Form I 结构）
  float b0, b1, b2;
  float a1, a2;
  // 延迟量状态
  float x1, x2, y1, y2;
} Biquad;

void demod (float32_t signalIn[], float32_t signalOut[], int N, float32_t carrierFreq, float32_t sampleRate);

/* For debug. */
void print_demod_out (float32_t signalOut[], int N);

#endif /* demodulation.h */
