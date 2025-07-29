#ifndef SIGNAL_PROCESS_H
#define SIGNAL_PROCESS_H

#include "basic.h"

#define SIGPROCESS_MAX_LEN  8192
#define LOG_E_10            2.302585092994046

float32_t sigprocess_filter_attenuation (uint32_t adcDoubleIn[], int N);

#endif /* signal_process.h */
