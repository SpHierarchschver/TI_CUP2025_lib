#include "dacapp.h"

float32_t amplitude, frequency, initPhase, dc;
WaveType waveformOut;
uint32_t curIdx, phaseStep;
uint32_t dacOut[DAC_LEN];

static uint32_t f2dac (float32_t x);

void
dac_set_wave (float32_t signalIn[], uint32_t freq, int corr, int N)
{
  phaseStep = DAC_MAX_PHASE / DAC_SYS_CLK * freq * 1.5 + corr;

  for (int i = 0; i < N; ++i)
    dacOut[i] = f2dac (signalIn[i]);
}

void
dac_set_dc (float32_t dcVal, int N)
{
  phaseStep = 1;

  for (int i = 0; i < N; ++i)
    dacOut[i] = f2dac (dcVal - DAC_MAX_VOL / 2);
}

void
HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim)
{
  __HAL_TIM_CLEAR_IT (&htim6, TIM_IT_UPDATE);
  curIdx += phaseStep;
  HAL_DAC_SetValue (&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacOut[(curIdx >> DAC_WORD_SHIFT) & 0xFFF]);
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
