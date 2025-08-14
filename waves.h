#ifndef WAVES_H
#define WAVES_H

#include <stdint.h>

int16_t triangle(double steps, uint32_t pos);
int16_t sawtooth(double steps, uint32_t pos);
int16_t sine(double steps, uint32_t pos);
int16_t square(double steps, uint32_t pos);

#endif
