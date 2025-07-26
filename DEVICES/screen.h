#ifndef SCREEN_H
#define SCREEN_H

#include "basic.h"

#define BUFFER_MAX_LEN		100

void screen_jump_to_page (char pageName[]);
void screen_print_wave (uint8_t data[], int N);

#endif /* screen.h */
