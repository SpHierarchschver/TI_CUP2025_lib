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

void make_win (int N);                                        // Initialize window coefficients.
void cfft (complex_t x[], int N);                             // Complex FFT.
void cifft (complex_t x[], int N);                            // Complex inverse FFT.
void rfft (float32_t input[], int N, 
           float32_t outputMagnitude[], WindowType winType);  // Real FFT.
void rifft (const float32_t outputMagnitude[], int N, 
            float32_t time_data[]);                           // Real inverse FFT.

/* For debug. */
void cifft_test (int N, float32_t time_data[]);

#endif /* fft.h */
