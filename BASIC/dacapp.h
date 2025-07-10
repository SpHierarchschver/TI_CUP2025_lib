#ifndef DACAPP_H
#define DACAPP_H

#include "func_gen.h"
#include "stdint.h"
#include "stm32h7xx.h"
#include "dac.h"
#include "tim.h"

#define DAC_AMP         20000
#define DAC_MAX_PHASE   4294967295
#define DAC_SYS_CLK     1e6
#define DAC_RESOLUTION  4096
#define DAC_MAX_VAL     4095
#define DAC_MIN_VAL     0
#define DAC_OFFSET      2047
#define DAC_MAX_VOL     3300

#define DAC_AMP_TOL     10
#define DAC_FREQ_TOL    10
#define DAC_PHI_TOL     1e-2
#define DAC_OFFSET_TOL  10

typedef enum
{
  SINE,
  TRIANGULAR,
  SQUARE,
} WaveType;

void dac_set_wave_single
(float32_t amp, float32_t freq, 
 float32_t phi, float32_t offset, WaveType type);
void dac_set_wave (float32_t signalIn[], int N);
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim);

/* For debug. */
void print_dacOut (void);

#endif /* dacapp.h */
