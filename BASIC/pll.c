#include "pll.h"
#include "filter.h"

float32_t phase;
float32_t freq = 50e3;

void
costas (float32_t signalIn[], float32_t signalOut[], int N)
{
  FIRState Ist, Qst;
  fir_single_init (&Ist, LPF_Fs1024k_Fc10k_O20_ORDER+1);
  fir_single_init (&Qst, LPF_Fs1024k_Fc10k_O20_ORDER+1);

  for (int i = 0; i < N; ++i)
  {
    float32_t Iraw = signalIn[i] * arm_cos_f32 (phase);
    float32_t Qraw = -signalIn[i] * arm_sin_f32 (phase);
    float32_t Ilp = fir_single (Iraw, LPF_Fs1024k_Fc10k_O20_COEF, &Ist);
    float32_t Qlp = fir_single (Qraw, LPF_Fs1024k_Fc10k_O20_COEF, &Qst);

    float32_t err = Ilp * Qlp;
    freq += BETA * err;
    phase += freq + ALPHA * err;

    while (phase > 2 * PI)
      phase -= 2 * PI;
    while (phase < 0)
      phase += 2 * PI;

    signalOut[i] = Ilp;
  }

  fir_single_free (&Ist);
  fir_single_free (&Qst);
}
