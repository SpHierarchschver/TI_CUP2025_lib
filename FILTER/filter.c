#include "basic.h"

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
