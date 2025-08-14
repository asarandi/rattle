#ifndef RINGTONE_H
#define RINGTONE_H

#include <stdint.h>

#define SFREQ 44100
#define FADE 2.0

struct note {
    char *raw;
    int octave;
    int index;

    int type;
    int dotted;

    double duration;
    double frequency;
    double steps;

    uint32_t dur;
    uint32_t pos;
};

struct ringtone {
    int16_t (*wave)(double, uint32_t);

    char *name; /* free */
    char *copy; /* free */

    int duration;
    int octave;
    int bpm;

    int verbose;
    int nofade;
    int transpose;

    volatile int num_notes;
    volatile int num_played;

    struct note *notes; /* free */
};

char *parse_ringtone(char *s, struct ringtone *o);
void free_ringtone_data(struct ringtone *o);

#endif
