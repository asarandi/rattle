#include <math.h>
#include <stdint.h>

/*
** "steps" = sampling frequency / note frequency
** number of samples per oscillation/period
*/

int16_t triangle(double steps, uint32_t pos) {
    double quarter, inc;

    pos = pos % (uint32_t)steps;
    quarter = steps / 4.0;
    inc = (double)INT16_MAX / quarter;

    if (pos < quarter) {
        return inc * pos;
    } else if (pos < 3 * quarter) {
        return INT16_MAX - (inc * (pos - quarter));
    }
    return -INT16_MAX + (inc * (pos - 3 * quarter));
}

int16_t sawtooth(double steps, uint32_t pos) {
    double half, inc;

    pos = pos % (uint32_t)steps;
    half = steps / 2.0;
    inc = (double)INT16_MAX / half;

    if (pos < half) {
        return inc * pos;
    }
    return inc * (pos - half) - INT16_MAX;
}

int16_t sine(double steps, uint32_t pos) {
    double inc;

    pos = pos % (uint32_t)steps;
    inc = (2.0 * M_PI) / steps;
    return sin(inc * pos) * INT16_MAX;
}

int16_t square(double steps, uint32_t pos) {
    pos = pos % (uint32_t)steps;
    return pos < steps / 2 ? INT16_MAX : -INT16_MAX;
}
