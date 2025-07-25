#include "basic.h"

#define IIR_MAX_ORDER 101

static float32_t iirZ[IIR_MAX_ORDER][2];

static void iir_init (int N);

void
fir_filter (int N, const float32_t signalIn[], 
            float32_t signalOut[], const float32_t coef[], 
            int order)
{
	for (int i = 0; i < N; ++i)
	{
    signalOut[i] = 0;

		for (int j = 0; j <= order; ++j)
		{
			if (i-j >= 0)
				signalOut[i] += signalIn[i-j] * coef[j];
		}
	}
}

void
fir_single_init (FIRState *st, int num_taps)
{
  st->num_taps = num_taps;
  st->idx = 0;
  st->hist = (float32_t *)calloc(num_taps, sizeof(float32_t));
}

void
fir_single_free (FIRState *st)
{
  free(st->hist);
  st->hist = NULL;
}

float32_t
fir_single (float32_t input, const float32_t *coef, FIRState *st)
{
  int N = st->num_taps;
  float32_t acc = 0.0;

  st->hist[st->idx] = input;

  int tap = 0;
  for (int k = st->idx; k >= 0; k--, tap++) {
      acc += st->hist[k] * coef[tap];
  }
  for (int k = N - 1; tap < N; k--, tap++) {
      acc += st->hist[k] * coef[tap];
  }

  st->idx++;
  if (st->idx >= N) st->idx = 0;

  return acc;
}

void
iir_filter (int N, const float32_t signalIn[], 
            float32_t signalOut[], const float32_t NUM[][3], 
            const float32_t DEN[][3], int order)
{
  if (order > IIR_MAX_ORDER)
    return;

  float32_t x, y;

  iir_init (order+1);

  for (int i = 0; i < N; ++i)
  {
    x = signalIn[i];

    for (int j = 0; j <= order; ++j)
    {
      y = NUM[j][0]*x + iirZ[j][0];
      iirZ[j][0] = NUM[j][1]*x - DEN[j][1]*y + iirZ[j][1];
      iirZ[j][1] = NUM[j][2]*x - DEN[j][2]*y;
      x = y;
    }

    signalOut[i] = x;
  }
}

static void
iir_init (int order)
{
  for (int i = 0; i <= order; ++i)
  {
    iirZ[i][0] = 0.0;
    iirZ[i][1] = 0.0;
  }
}
