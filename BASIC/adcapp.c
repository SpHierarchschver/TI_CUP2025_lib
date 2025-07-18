#include "adcapp.h"

extern uint8_t adcFlag;

void
adc_convert (uint32_t adcIn[], float32_t adcOut[], int N)
{
  for (int i = 0; i < N; ++i)
    adcOut[i] = ((float32_t)adcIn[i] - (float32_t)ADC_OFFSET) 
              / (float32_t)((ADC_MAX_VAL - ADC_MIN_VAL) / 2) * ADC_MAX_VOL;
}

void
adc_inv_convert (float32_t adcIn[], int32_t adcOut[], int N)
{
  for (int i = 0; i < N; ++i)
    adcOut[i] = (int)(adcIn[i] / ADC_MAX_VOL * (float32_t)((ADC_MAX_VAL - ADC_MIN_VAL) / 2));
}

void
HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc)
{	
	if (hadc->Instance == ADC1)
		adcFlag = 1;
}

void
HAL_ADC_ConvHalfCpltCallback (ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
		adcFlag = 2;
}
