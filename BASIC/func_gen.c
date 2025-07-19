#include "func_gen.h"

void
gen_sine (float32_t signalOut[], int N, float32_t amp, float32_t freq, float32_t phi, float32_t Fs)
{
  float32_t Ts = 1.0f / Fs;
  float32_t phi_rad = phi * (PI / 180.0f);

  for (int i = 0; i < N; ++i)
    signalOut[i] = amp * arm_sin_f32 (2.0f * PI * freq * i * Ts + phi_rad);
}

/*
void
gen_triangular (float32_t signalOut[], int N, float32_t amp, float32_t freq, float32_t phi, float32_t Fs)
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
    diff = f32abs (diff);
    tri = 4.0f * diff - 1.0f;
    signalOut[i] = amp * tri;
  }
}
*/


void
gen_square (float32_t signalOut[], int N, float32_t amp, float32_t freq, float32_t phi, float32_t duty, float32_t Fs, int aboveZero)
{
  float32_t Ts = 1.0f / Fs;
  float32_t T = 1.0f / freq;
  float32_t phi_sec = (phi / 360.0f) * T;
  float32_t threshold = duty * T;
  
  for (int i = 0; i < N; ++i)
  {
    float32_t t = i * Ts + phi_sec;
    float32_t cycles = t * freq;
    float32_t fractional = cycles - (int)cycles;
    if (fractional < 0)
      fractional += 1.0f; 
    
    float32_t t_mod = fractional * T;

    if (aboveZero)
      signalOut[i] = (t_mod < threshold) ? amp : 0;
    else
    signalOut[i] = (t_mod < threshold) ? amp : -amp;
  }
}

void
gen_AM (float32_t signalOut[], int N, float32_t amp, float32_t modDepth, float32_t carrierFreq, float32_t transRate, float32_t Fs)
{
  for (int i = 0; i < N; ++i)
  {
    float32_t t = i / Fs;
    float32_t m_t = arm_sin_f32 (2 * PI * transRate * t); 
    signalOut[i] = amp * (1.0f + modDepth * m_t) * arm_cos_f32 (2 * PI * carrierFreq * t);
  }
}

void
gen_BASK (float32_t signalOut[], int N, float32_t amp, float32_t carrierFreq, float32_t transRate, float32_t Fs, uint8_t seq[], int seqLen)
{
  const float32_t Ts = 1.0f / Fs;
  const float32_t symbolDuration = 1.0f / transRate;
  const int samplesPerSymbol = (int)(Fs / transRate + 0.5f);
  const float32_t phaseIncrement = 2 * PI * carrierFreq * Ts;
  
  float32_t phase = 0.0f;
  int symbolIndex = 0;
  int sampleCount = 0;
  
  for (int i = 0; i < N; ++i)
  {
    const uint8_t currentBit = seq[symbolIndex];
    
    signalOut[i] = (currentBit == 1) ? (amp * arm_sin_f32 (phase)) : 0.0f;
    phase += phaseIncrement;
    ++sampleCount;

    if (sampleCount >= samplesPerSymbol)
    {
      sampleCount = 0;
      symbolIndex = (symbolIndex + 1) % seqLen;
    }
    
    if (phase >= 2 * PI)
      phase -= 2 * PI;
  }
}

void
gen_FM (float32_t signalOut[], int N, float32_t amp, float32_t freqDev, float32_t carrierFreq, float32_t Fs)
{
  float32_t phase = 0.0f;
  
  for (int i = 0; i < N; ++i)
  {
    float32_t t = i / Fs;
    float32_t m_t = arm_sin_f32 (2 * PI * freqDev * t);
    float32_t f_inst = carrierFreq + freqDev * m_t;

    phase += 2 * PI * f_inst / Fs;

    while (phase >= 2 * PI)
      phase -= 2 * PI;
    while (phase < 0.0f)
      phase += 2 * PI;

    signalOut[i] = amp * arm_cos_f32 (phase);
  }
}

void
gen_BFSK (float32_t signalOut[], int N, float32_t amp, float32_t carrierFreq, float32_t hopFreq, float32_t transRate, float32_t Fs, uint8_t seq[], int seqLen)
{
  float32_t phase = 0.0f;
  float32_t Tbit = 1.0f / transRate;
  int samplesPerBit = (int)(Fs * Tbit + 0.5f);

  for (int i = 0; i < N; ++i)
  {
    int bitIndex = (i / samplesPerBit) % seqLen;
    uint8_t bit = seq[bitIndex];
    float32_t f_inst = bit ? hopFreq : carrierFreq;

    phase += 2 * PI * f_inst / Fs;

    while (phase >= 2 * PI)
      phase -= 2 * PI;
    while (phase < 0.0f) 
      phase += 2 * PI;

    signalOut[i] = amp * arm_cos_f32 (phase);
  }
}

void
gen_PM (float32_t signalOut[], int N, float32_t amp, float32_t modCoef, float32_t carrierFreq, float32_t transRate, float32_t Fs)
{
  for (int i = 0; i < N; ++i)
  {
    float32_t t = i / Fs;
    float32_t m_t = arm_sin_f32 (2 * PI * transRate * t);
    float32_t phase = 2 * PI * carrierFreq * t + modCoef * m_t;

    signalOut[i] = amp * arm_cos_f32 (phase);
  }
}

void
gen_BPSK (float32_t signalOut[], int N, float32_t amp, float32_t carrierFreq, float32_t transRate, float32_t Fs, uint8_t seq[], int seqLen)
{
  float32_t phase = 0.0f;
  float32_t Tbit = 1.0f / transRate;
  int samplesPerBit = (int)(Fs * Tbit + 0.5f);

  for (int i = 0; i < N; ++i)
  {
    int bitIndex = (i / samplesPerBit) % seqLen;
    uint8_t bit = seq[bitIndex];
    float32_t symbol = (bit ? 1.0f : -1.0f);

    phase = 2 * PI * carrierFreq * i / Fs;
    if (symbol < 0)
      phase += PI;

    while (phase >= 2 * PI)
      phase -= 2 * PI;
    while (phase < 0.0f)
      phase += 2 * PI;

    signalOut[i] = amp * arm_cos_f32 (phase);
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
