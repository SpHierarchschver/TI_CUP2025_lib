#include "fft.h"

complex_t z[ADC_MAX_LEN];
complex_t complexOutput[ADC_MAX_LEN/2+1];

/* Windows. */
float32_t hanning[ADC_MAX_LEN];

static void bit_reverse (complex_t *x, int n);

void
make_win (int N)
{
  if (N >= ADC_MAX_LEN)
    return;

  for (int i = 0; i < N; ++i)
    hanning[i] = 0.5 * (1.0 - arm_cos_f32 (2*PI*i/(N-1)));
}

void
cfft (complex_t *x, int N)
{
  bit_reverse (x, N);

  for (int i = 2; i <= N; i <<= 1)
  {
    int half_len = i >> 1;
    float32_t angleStep = -2.0f * PI / (float32_t)i;

    for (int j = 0; j < N; j += i)
    {
      for (int k = 0; k < half_len; ++k)
      {
        float32_t angle = angleStep * k;
        complex_t twiddle = 
        {
          .real = arm_cos_f32 (angle),
          .imag = arm_sin_f32 (angle)
        };

        int idx1 = j + k;
        int idx2 = idx1 + half_len;
        complex_t a = x[idx1];
        complex_t b = x[idx2];
        complex_t rotated_b = 
        {
          .real = b.real * twiddle.real - b.imag * twiddle.imag,
          .imag = b.real * twiddle.imag + b.imag * twiddle.real
        };

        x[idx1].real = a.real + rotated_b.real;
        x[idx1].imag = a.imag + rotated_b.imag;
        x[idx2].real = a.real - rotated_b.real;
        x[idx2].imag = a.imag - rotated_b.imag;
      }
    }
  }
}

void
rfft (float32_t input[], int n, float32_t outputMagnitude[], WindowType winType)
{
  int N2 = n / 2;

  for (int i = 0; i < N2; ++i)
  {
    z[i].real = input[2*i];
    z[i].imag = input[2*i+1];

    switch (winType)
    {
    case NO_WIN:
      break;

    case HANNING:
      z[i].real *= hanning[2*i];
      z[i].imag *= hanning[2*i+1];
      break;

    default:
      break;
    }
  }

  cfft (z, N2);

  complexOutput[0].real = z[0].real + z[0].imag;
  complexOutput[0].imag = 0;
  outputMagnitude[0] = f32abs (complexOutput[0].real);
  outputMagnitude[0] *= 2.0 / n;

  complexOutput[N2].real = z[0].real - z[0].imag;
  complexOutput[N2].imag = 0;
  outputMagnitude[N2] = f32abs (complexOutput[N2].real);
  outputMagnitude[N2] *= 2.0 / n;

  for (int k = 1; k < N2; ++k)
  {
    int conjugateIdx = N2 - k;
    complex_t zConj = 
    {
      .real = z[conjugateIdx].real,
      .imag = -z[conjugateIdx].imag
    };
    complex_t even = 
    {
      .real = (z[k].real + zConj.real) * 0.5f,
      .imag = (z[k].imag + zConj.imag) * 0.5f
    };
    complex_t diff = 
    {
      .real = z[k].real - zConj.real,
      .imag = z[k].imag - zConj.imag
    };
    complex_t odd = 
    {
      .real =  diff.imag * 0.5f,
      .imag = -diff.real * 0.5f
    };
    float32_t theta = -2.0f * PI * k / (float32_t)n;
    complex_t twiddle = 
    {
      .real = arm_cos_f32(theta),
      .imag = arm_sin_f32(theta)
    };
    complex_t rotatedOdd = {
      .real = odd.real * twiddle.real - odd.imag * twiddle.imag,
      .imag = odd.real * twiddle.imag + odd.imag * twiddle.real
    };

    complexOutput[k].real = even.real + rotatedOdd.real;
    complexOutput[k].imag = even.imag + rotatedOdd.imag;
    arm_sqrt_f32(complexOutput[k].real * complexOutput[k].real + complexOutput[k].imag * complexOutput[k].imag, &outputMagnitude[k]);

    outputMagnitude[k] *= 2.0 / n;

    switch (winType)
    {
    case NO_WIN:
      break;

    case HANNING:
      outputMagnitude[k] *= HANNING_FACTOR;
      break;
    
    default:
      break;
    }
  }
}

static void
bit_reverse (complex_t *x, int n)
{
  for (int i = 1, j = 0; i < n; ++i)
  {
    int bit = n >> 1;

    while (j & bit)
    {
      j ^= bit;
      bit >>= 1;
    }

    j |= bit;

    if (i < j)
    {
      complex_t tmp = x[i];
      x[i] = x[j];
      x[j] = tmp;
    }
  }
}
