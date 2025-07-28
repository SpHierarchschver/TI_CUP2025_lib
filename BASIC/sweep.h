#ifndef SWEEP_H
#define SWEEP_H

#include "basic.h"
#include "AD9959.h"

#define SWEEP_MAX_LEN   100

void sweep_freq (float32_t begin, float32_t end, float32_t step, int N);
void sweep_amp (float32_t begin, float32_t end, float32_t step, int N);

#endif /* sweep.h */
