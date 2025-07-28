#ifndef BASIC_H
#define BASIC_H

// #include <stdio.h>
#include "string.h"
#include "arm_math.h"
#include "arm_const_structs.h"

/* Math settings. */
#define SIMAL               1e-10           // A tiny quantity.
#define ATAN_INT_THRESHOLD  0.1             // Threshold to activate integral method of arctan.
#define ATAN_INT_STEP       1000            // Step length for integral method of arctan.
#define MAX_VOL             1650
#define SCREEN_OFFSET       120

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

typedef struct
{
  float32_t *hist;
  int num_taps;
  int idx;
} FIRState;

/* Function prototypes. */
float32_t f32abs (float32_t x);                                   // Absolute value of x.
float32_t f32pow (float32_t x, uint8_t n);                        // x to the n_th power.
float32_t arctan (float32_t x);                                   // arctan(x) by integral method.
float32_t mean (float32_t arrIn[], int N);
float32_t squdev (float32_t arrIn[], int N, float32_t meanVal);
float32_t rms (float32_t arrIn[], int N);

void f32_map_u8 (float32_t arrIn[], uint8_t arrOut[], int N);

int is_equal_f (float32_t a, float32_t b, 
                float32_t tolerance);
int is_spectrum_leak (float32_t fftIn[], int N, float32_t thresholdVal, int thresholdNum);
int sgn (float32_t x);

void init_qsort (float32_t arrIn[], SortIns insts[], int N);
void qsort (SortIns insts[], int N, QsortType);

void print_arr_f (float32_t arrIn[], int N);
void print_arr_u32 (uint32_t arrIn[], int N);

#endif /* basic.h */
