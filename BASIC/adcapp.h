#ifndef ADCAPP_H
#define ADCAPP_H

#include "basic.h"
#include "adc.h"

#define ADC_MIN_VAL         0               // ADC min value.
#define ADC_MAX_VAL         65535           // ADC max value.
#define ADC_OFFSET          32767           // ADC offset value.
#define ADC_SAMPLE_RATE     4096000         // ADC sample rate.
#define ADC_LEN             4096            // ADC sample length.

void adc_convert (uint32_t adcIn[], float32_t adcOut[], int N);    // Convert adc value to real voltage value.
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc);    // ADC interrupt handler function.

#endif /* adcapp.h */
