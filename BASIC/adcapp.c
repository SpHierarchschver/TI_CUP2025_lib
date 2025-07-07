#include "adcapp.h"

extern uint8_t adc1Flag;

void
adc_convert (uint32_t adcIn[], float32_t adcOut[], int N)
{
  for (int i = 0; i < N; ++i)
    adcOut[i] = ((float32_t)adcIn[i] - (float32_t)ADC_OFFSET) 
              / (float32_t)((ADC_MAX_VAL - ADC_MIN_VAL) / 2);
}

void
HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc)
{	
	if (hadc->Instance == ADC1)
	{
		adc1Flag = 1;
		HAL_ADC_Stop_DMA (&hadc1);
	}
}
