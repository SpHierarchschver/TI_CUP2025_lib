#ifndef FUNC_GEN_H
#define FUNC_GEN_H

#include "basic.h"

/* Generate a sine wave given its amplitude `amp`(mV), frequency `freq`(Hz), and initial phase `phi`(degree). The output is stored in `signalOut[]` and the output length is `N`. */
void gen_sine (float32_t signalOut[], int N,
               float32_t amp, float32_t freq, 
               float32_t phi, float32_t Fs);

/* Generate a triangular wave given its amplitude `amp`(mV), frequency `freq`(Hz), and initial phase `phi`(degree). The output is stored in `signalOut[]` and the output length is `N`. */
void gen_triangular (float32_t signalOut[], int N,
                     float32_t amp, float32_t freq, 
                     float32_t phi, float32_t Fs);

/* Generate a triangular wave given its amplitude `amp`(mV), frequency `freq`(Hz), initial phase `phi`(degree), and duty ratio `duty`. The output is stored in `signalOut[]` and the output length is `N`. */
void gen_square (float32_t signalOut[], int N,
                 float32_t amp, float32_t freq, 
                 float32_t phi, float32_t duty, 
                 float32_t Fs);

/* Given an array of pointers, each pointing to a float32_t array, add all these signals of length N and output to signalOut. */
void adder (float32_t ** signals, float32_t signalOut[], int signalNum, int N);

/* Given an array of pointers, each pointing to a float32_t array, multiply all these signals of length N and output to signalOut. */
void multiplier (float32_t ** signals, float32_t signalOut[], int signalNum, int N);

#endif /* func_gen.h */
