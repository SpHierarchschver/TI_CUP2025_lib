#include "basic.h"

#define IIR_MAX_ORDER 101

static float32_t iirZ[IIR_MAX_ORDER][2];

static void iir_init (int N);

void
fir_filter (int N, const float32_t signalIn[], float32_t signalOut[], const float32_t coef[], int order)
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
iir_filter (int N, const float32_t signalIn[], float32_t signalOut[], const float32_t NUM[][3], const float32_t DEN[][3], int order)
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
