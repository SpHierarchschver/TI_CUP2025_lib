#include "signal_process.h"
#include "adcapp.h"

uint32_t tmp[SIGPROCESS_MAX_LEN];
float32_t signalIn[SIGPROCESS_MAX_LEN];

float32_t Vin, Vout, res;

float32_t
sigprocess_filter_attenuation (uint32_t adcDoubleIn[], int N)
{
  /* The upper 16bits are Vout, and the lower 16bits are Vin.
     The formula is: 20 * log_10 (Vout / Vin) */
  for (int i = 0; i < N; ++i)
    tmp[i] = adcDoubleIn[i] & 0xffff;

  adc_convert (tmp, signalIn, N);
  Vin = rms (signalIn, N);

  for (int i = 0; i < N; ++i)
    tmp[i] = (adcDoubleIn[i] >> 16) & 0xffff;

  adc_convert (tmp, signalIn, N);
  Vout = rms (signalIn, N);
	res = Vout / Vin;

  arm_vlog_f32 (&res, &res, 1);
  
  return 20 * res;
}
