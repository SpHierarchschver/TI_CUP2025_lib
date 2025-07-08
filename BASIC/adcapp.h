#ifndef ADCAPP_H
#define ADCAPP_H

#include "basic.h"
#include "adc.h"

/*
 * ADC_SAMPLE_RATE 4.096MHz
 * DIVM3 15, DIVN3 491, DIVR3 10, fracn3 4260 
 */

#define ADC_MIN_VAL         0               // ADC min value.
#define ADC_MAX_VAL         65535           // ADC max value.
#define ADC_MAX_VOL         1650
#define ADC_OFFSET          32767           // ADC offset value.
// #define ADC_SAMPLE_RATE     4096000         // ADC sample rate.
// #define ADC_LEN             4096            // ADC sample length.
#define ADC_SAMPLE_RATE     1e6         // ADC sample rate.
#define ADC_LEN             10000            // ADC sample length.

void adc_convert (uint32_t adcIn[], float32_t adcOut[], int N);    // Convert adc value to real voltage value.
void adc_inv_convert (float32_t adcIn[], int32_t adcOut[], int N);
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc);    // ADC interrupt handler function.

#endif /* adcapp.h */
