#ifndef ADCAPP_H
#define ADCAPP_H

#include "basic.h"
#include "adc.h"

/*
 * 22 144 1468 128 -> 1.28
 * 24 196 4981 10 -> 20.48
 * 15 491 4260 10 -> 81.92
 */

typedef enum
{
  Fs_64k,
  Fs_128k,
  Fs_256k,
  Fs_512k,
  Fs_1024k,
  Fs_2048k,
  Fs_4096k,
} AdcSampleRate;

#define ADC_MIN_VAL         0               // ADC min value.
#define ADC_MAX_VAL         65535           // ADC max value.
#define ADC_MAX_VOL         1650
#define ADC_OFFSET          32767           // ADC offset 

void adc_convert (uint32_t adcIn[], float32_t adcOut[], int N);    // Convert adc value to real voltage value.
void adc_inv_convert (float32_t adcIn[], int32_t adcOut[], int N);
void adc_set_Fs (AdcSampleRate mode);
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc);    // ADC interrupt handler function.
void HAL_ADC_ConvHalfCpltCallback (ADC_HandleTypeDef *hadc);

#endif /* adcapp.h */
