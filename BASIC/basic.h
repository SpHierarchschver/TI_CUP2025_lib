#ifndef BASIC_H
#define BASIC_H

#include "string.h"
#include "arm_math.h"
#include "arm_const_structs.h"

/* Math settings. */
#define SIMAL               1e-10           // A tiny quantity.
#define ATAN_INT_THRESHOLD  0.1             // Threshold to activate integral method of arctan.
#define ATAN_INT_STEP       1000            // Step length for integral method of arctan.

typedef enum
{
  ASCENDING,
  DESCENDING,
} QsortType;

/* Structs typedef. */
typedef struct
{
  float32_t real;
  float32_t imag;
} complex_t;

typedef struct
{
  float32_t val;
  int       idx;
} SortIns;

/* Function prototypes. */
float32_t f32abs (float32_t x);                                   // Absolute value of x.
float32_t f32pow (float32_t x, uint8_t n);                        // x to the n_th power.
float32_t arctan (float32_t x);                                   // arctan(x) by integral method.

int is_equal_f (float32_t a, float32_t b, 
                float32_t tolerance);

void init_qsort (float32_t arrIn[], SortIns insts[], int N);
void qsort (SortIns insts[], int N, QsortType);

#endif /* basic.h */
