#ifndef FFT_H
#define FFT_H

#include "basic.h"

#define ADC_MAX_LEN       8193
#define HANNING_FACTOR    2.0

typedef enum
{
  NO_WIN,
  HANNING,
} WindowType;

void make_win (int N);
void cfft (complex_t *x, int n);
void rfft (float32_t input[], int n, float32_t outputMagnitude[], WindowType winType);

#endif /* fft.h */
