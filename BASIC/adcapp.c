#include "adcapp.h"

extern uint8_t adcFlag;
extern uint32_t adcData[];

void
adc_enable (int N)
{
  adcFlag = 0;
  // HAL_ADC_Start_DMA (&hadc1, adcData, N);
  HAL_ADCEx_MultiModeStart_DMA (&hadc1, adcData, N);
	adc_set_Fs (Fs_1024k);
}

void
adc_disable (int N)
{
  adcFlag = 1;
  // HAL_ADC_Stop_DMA (&hadc1);
  HAL_ADCEx_MultiModeStop_DMA (&hadc1);
  memset (adcData, 0, N);
}

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
adc_set_Fs (AdcSampleRate mode)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  ADC_Disable (&hadc1);
  HAL_Delay (2);

  switch (mode)
  {
  case Fs_64k:
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M = 22;
    PeriphClkInitStruct.PLL3.PLL3N = 144;
    PeriphClkInitStruct.PLL3.PLL3R = 128;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 1468;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    HAL_ADC_Init (&hadc1);

    break;

  case Fs_128k:
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M = 22;
    PeriphClkInitStruct.PLL3.PLL3N = 144;
    PeriphClkInitStruct.PLL3.PLL3R = 64;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 1468;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    HAL_ADC_Init (&hadc1);

    break;

  case Fs_256k:
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M = 22;
    PeriphClkInitStruct.PLL3.PLL3N = 144;
    PeriphClkInitStruct.PLL3.PLL3R = 32;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 1468;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    HAL_ADC_Init (&hadc1);

    break;

  case Fs_512k:
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M = 22;
    PeriphClkInitStruct.PLL3.PLL3N = 144;
    PeriphClkInitStruct.PLL3.PLL3R = 16;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 1468;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    HAL_ADC_Init (&hadc1);

    break;

  case Fs_1024k:
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M = 22;
    PeriphClkInitStruct.PLL3.PLL3N = 144;
    PeriphClkInitStruct.PLL3.PLL3R = 8;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 1468;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    HAL_ADC_Init (&hadc1);

    break;

  case Fs_2048k:
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M = 22;
    PeriphClkInitStruct.PLL3.PLL3N = 144;
    PeriphClkInitStruct.PLL3.PLL3R = 4;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 1468;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    HAL_ADC_Init (&hadc1);

    break;

  case Fs_4096k:
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M = 22;
    PeriphClkInitStruct.PLL3.PLL3N = 144;
    PeriphClkInitStruct.PLL3.PLL3R = 2;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 1468;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    HAL_ADC_Init (&hadc1);

    break;

  default:
    break;
  }
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
