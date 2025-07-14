#include "dacapp.h"

float32_t amplitude, frequency, initPhase, dc;
WaveType waveformOut;
uint32_t curIdx, phaseStep;
uint32_t dacOut[DAC_RESOLUTION];

static uint32_t f2dac (float32_t x);

void
dac_set_wave_single (float32_t amp, float32_t freq, float32_t phi, float32_t offset, WaveType type)
{
  if (is_equal_f (amp, amplitude, DAC_AMP_TOL)   &&
      is_equal_f (freq, frequency, DAC_FREQ_TOL) &&
      is_equal_f (phi, initPhase, DAC_PHI_TOL)   &&
      is_equal_f (offset, dc, DAC_OFFSET_TOL)    &&
      waveformOut == type)
    return;

  amplitude = amp;
  frequency = freq;
  initPhase = phi;
  dc = offset;
  waveformOut = type;
  curIdx = 0;

  switch (waveformOut)
  {
  case SINE:
    // gen_sine (dacOut, DAC_RESOLUTION, amplitude, frequency, initPhase, DAC_SYS_CLK);
    break;

  case TRIANGULAR:
    break;

  case SQUARE:
    break;
  
  default:
    break;
  }
}

void
dac_set_wave (float32_t signalIn[], uint32_t freq, int corr, int N)
{
  phaseStep = 4294.967296 * freq * 1.5 + corr;
  // phaseStep = DAC_MAX_PHASE / DAC_SYS_CLK * freq * 1.5 + corr;

  for (int i = 0; i < N; ++i)
    dacOut[i] = f2dac (signalIn[i]);  
}

void
HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim)
{
  __HAL_TIM_CLEAR_IT (&htim6, TIM_IT_UPDATE);
  curIdx += phaseStep;
  HAL_DAC_SetValue (&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacOut[(curIdx >> 20) & 0xFFF]);
}

void
print_dacOut ()
{
  for (int i = 0; i < DAC_RESOLUTION; ++i)
    printf ("%d\r\n", dacOut[i]);
}

static uint32_t
f2dac (float32_t x)
{
  int32_t tmp = (int32_t)(x / DAC_MAX_VOL * DAC_RESOLUTION + DAC_OFFSET);

  if (tmp > DAC_MAX_VAL)
    tmp = DAC_MAX_VAL;
  else if (tmp < DAC_MIN_VAL)
    tmp = DAC_MIN_VAL;
  
  return (uint32_t)tmp;
}
