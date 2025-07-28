#include "sweep.h"
#include "adcapp.h"
#include "AD9959.h"
#include "signal_process.h"

extern uint8_t adcFlag;
extern uint32_t adcData[];
float32_t results[SWEEP_MAX_LEN];

void sweep_freq (float32_t begin, float32_t end, float32_t step, int N)
{
  adc_disable (N);

  int stepNum = (int)((end - begin) / step);
  uint32_t curFreq = (uint32_t)begin;

  for (int i = 0; i < stepNum; ++i)
  {
    adc_disable (N);
		AD9959_SetFrequency4Channel (curFreq, curFreq, curFreq, curFreq);

		// HAL_Delay (10000);
    adc_enable (N);

    while (!adcFlag);

    /* Signal Process Function. */
    results[i] = sigprocess_filter_attenuation (adcData, N);
    curFreq += (uint32_t)step;
    adcFlag = 0;
  }
}

void sweep_amp (float32_t begin, float32_t end, float32_t step, int N)
{
  
}
