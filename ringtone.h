#ifndef RINGTONE_H
#define RINGTONE_H

#include <stdint.h>

struct note {
    char *raw;
    int octave;
    int index;

    double duration;
    double frequency;
    double steps;

    uint32_t mpos;

    uint32_t dur;
    uint32_t pos;
};

struct ringtone {
    int16_t (*wave)(double, uint32_t);
    int16_t last_sample;

    double last_volume;
    double last_steps;
    uint32_t last_mpos;

    int sfreq;

    char *name; // free
    char *copy; // free

    int duration;
    int octave;
    int bpm;

    volatile int num_notes;
    volatile int num_played;

    struct note *notes; // free
};

char *parse_ringtone(char *s, struct ringtone *o);
void free_ringtone_data(struct ringtone *o);

#endif
