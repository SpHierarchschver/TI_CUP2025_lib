#ifndef BASIC_H
#define BASIC_H

#include "string.h"
#include "arm_math.h"
#include "arm_const_structs.h"

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
void adc_convert (uint32_t adcIn[], float32_t adcOut[], int N);   // Convert adc value to real voltage value.
float32_t f32abs (float32_t x);                                   // Absolute value of x.
float32_t f32pow (float32_t x, uint8_t n);                        // x to the n_th power.
float32_t arctan (float32_t x);                                   // arctan(x) by integral method.

#endif /* basic.h */
