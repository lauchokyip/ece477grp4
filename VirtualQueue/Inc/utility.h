#ifndef _MYUTILITY_H
#define _MYUTILITY_H

#include <stdio.h>
#include "stm32l4xx_hal.h"

void clear_buf(uint8_t*, int);
int count_digits(int);
void print_length(uint8_t*, int);
void str_to_uint(char*, uint8_t*, int);

#endif
