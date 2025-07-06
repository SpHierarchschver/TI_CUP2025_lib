#ifndef BASIC_H
#define BASIC_H

#include "string.h"
#include "arm_math.h"
#include "arm_const_structs.h"

/* ADC settings. */
#define ADC_MIN_VAL         0               // ADC min value.
#define ADC_MAX_VAL         65535           // ADC max value.
#define ADC_SAMPLE_RATE     ((int)1e6)      // ADC sample rate.

/* Math settings. */
#define SIMAL               1e-10           // A tiny quantity.
#define ATAN_INT_THRESHOLD  0.1             // Threshold to activate integral method of arctan.
#define ATAN_INT_STEP       1000            // Step length for integral method of arctan.

/* Structs typedef. */
typedef struct
{
  float32_t real;
  float32_t imag;
} complex_t;

/* Function prototypes. */
float32_t f32abs (float32_t x);             // x's absolute value.
float32_t f32pow (float32_t x, uint8_t n);  // x to n_th power.
float32_t arctan (float32_t x);

#endif /* basic.h */
