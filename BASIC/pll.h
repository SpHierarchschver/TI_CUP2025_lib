#ifndef PLL_H
#define PLL_H

#include "basic.h"

#define ALPHA         0.0009201400896501423
// #define ALPHA         0.01
#define BETA          4.23651730577984e-07
// #define BETA          0.001
#define PLL_MAX_LEN   8192

void costas (float32_t signalIn[], float32_t signalOut[], int N);

#endif /* pll.h */
