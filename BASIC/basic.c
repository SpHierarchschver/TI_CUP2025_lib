#include "basic.h"

/* float32_t abs function. */
float32_t
f32abs (float32_t x)
{
	if (x < 0)
		return -x;

	return x;
}

/* float32_t power function. */
float32_t
f32pow (float32_t x, uint8_t n)
{
	if (n == 0)
		return 1.0;
	else if (n == 1)
		return x;
	
	for (uint8_t i = 1; i < n; ++i)
		x *= x;

	return x;
}

/* Integral method for arctan function. */
float32_t
arctan (float32_t x)
{
	float32_t tmp;
	float32_t sum = 0.0;
	float32_t stepSize = f32abs (x) / TAN_INT_STEP;
	
	for (uint32_t i = 0; i < TAN_INT_STEP; ++i)
	{
		tmp = stepSize*i + stepSize/2.0;
		sum += stepSize / (1.0 + tmp*tmp);
	}
	
	if (x < 0)
		sum *= -1;
	
	return sum;
}
