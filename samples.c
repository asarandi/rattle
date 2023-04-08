#include <math.h>
#include <stdint.h>

/*
** calculate starting position when changing notes
** do this in order to avoid "clicks" during playback
*/
uint32_t calc_pos_b(double steps_a, uint32_t pos_a, double steps_b) {
    double pct;

    pct = (double)pos_a / steps_a * 100.0;
    return ceil(steps_b / 100.0 * pct);
}

/*
** "steps" = sampling frequency / note frequency
** number of samples per oscillation/period
** NB: pos %= steps
*/

int16_t triangle(double steps, uint32_t pos) {
    double quarter, inc;

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

    half = steps / 2.0;
    inc = (double)INT16_MAX / half;

    if (pos < half) {
        return inc * pos;
    }
    return inc * (pos - half) - INT16_MAX;
}

int16_t sine(double steps, uint32_t pos) {
    double inc;

    inc = (2.0 * M_PI) / steps;
    return sin(inc * pos) * INT16_MAX;
}

int16_t square(double steps, uint32_t pos) {
    return pos < steps / 2 ? -INT16_MAX : INT16_MAX;
}
