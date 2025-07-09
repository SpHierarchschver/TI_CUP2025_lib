#include "func_gen.h"

void
gen_sine (float32_t signalOut[], int N, float32_t amp, float32_t freq, float32_t phi)
{
  float32_t Ts = 1.0f / Fs;
  float32_t phi_rad = phi * (PI / 180.0f);

  for (int i = 0; i < N; ++i)
    signalOut[i] = amp * arm_sin_f32 (2.0f * PI * freq * i * Ts + phi_rad);
}

void
gen_triangular (float32_t signalOut[], int N, float32_t amp, float32_t freq, float32_t phi)
{
  float32_t Ts = 1.0f / Fs;
  float32_t T = 1.0f / freq;
  float32_t phi_sec = (phi / 360.0f) * T;

  for (int i = 0; i < N; ++i)
  {
    float32_t t = i * Ts + phi_sec;
    float32_t t_mod;
    arm_mod_f32 (t, T, &t_mod);

    float32_t normalized = t_mod / T;

    float32_t tri, diff;
    arm_sub_f32 (&normalized, &(float32_t){0.5f}, &diff);
    arm_abs_f32 (&diff, &diff);
    tri = 4.0f * diff - 1.0f;
    signalOut[n] = amp * tri;
  }
}

void
gen_square (float32_t signalOut[], int N, float32_t amp, float32_t freq, float32_t phi, float32_t duty)
{
  float32_t Ts = 1.0f / Fs;
  float32_t T = 1.0f / freq;
  float32_t phi_sec = (phi / 360.0f) * T;
  float32_t threshold = duty * T;

  for (int i = 0; i < N; ++i)
  {
    float32_t t = n * Ts + phi_sec;
    float32_t t_mod;
    arm_mod_f32 (t, T, &t_mod);
    signalOut[n] = (t_mod < threshold) ? amp : -amp;
  }
}

void
adder (float32_t ** signals, float32_t signalOut[], int signalNum, int N)
{
  arm_copy_f32 (signals[0], signalOut, (uint32_t)N);

  for (int i = 1; i < signalNum; ++i)
    arm_add_f32 (signalOut, signals[i], signalOut, (uint32_t)N);
}

void
multiplier (float32_t ** signals, float32_t signalOut[], int signalNum, int N)
{
  arm_copy_f32 (signals[0], signalOut, (uint32_t)N);

  for (int i = 1; i < signalNum; ++i)
    arm_mult_f32 (signalOut, signals[i], signalOut, (uint32_t)N);
}
