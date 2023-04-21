#include <math.h>
#include <string.h>

/* match two character notes before single character notes */
/* nb: preference order, sharps before flats */

/* clang-format off */
struct {
    char *name;
    int len;
    int index;
} note_indices[] = {
    {"a#", 2, 10},
    {"bb", 2, 10},
    {"hb", 2, 10},
    {"c#", 2,  1},
    {"db", 2,  1},
    {"d#", 2,  3},
    {"eb", 2,  3},
    {"f#", 2,  6},
    {"gb", 2,  6},
    {"g#", 2,  8},
    {"ab", 2,  8},
    { "a", 1,  9},
    { "b", 1, 11},
    { "h", 1, 11},
    { "c", 1,  0},
    { "d", 1,  2},
    { "e", 1,  4},
    { "f", 1,  5},
    { "g", 1,  7},
};
/* clang-format on */

#define NUM_NOTE_INDICES 19

int get_note_index(char *s) {
    int i;

    for (i = 0; i < NUM_NOTE_INDICES; i++) {
        if (!strncmp(s, note_indices[i].name, note_indices[i].len)) {
            return note_indices[i].index;
        }
    }
    return -1;
};

char *get_index_name(int index) {
    int i;

    for (i = 0; i < NUM_NOTE_INDICES; i++) {
        if (note_indices[i].index == index) {
            return note_indices[i].name;
        }
    }
    return NULL;
}

/*
** index 0 = C0 = 16.3516 Hz
*/
double get_index_frequency(int index) {
    return pow(2.0, (double)index / 12.0) * 16.3516;
}

/* clang-format off */
double get_note_duration(int bpm, int note_type, int is_dotted) {
    double spb;

    spb = 60.0 / (double)bpm;
    spb *= is_dotted ? 1.5 : 1.0;
    switch (note_type) {
        case  1: return spb * 4.0;  // whole
        case  2: return spb * 2.0;  // half
        case  4: return spb;        // quarter
        case  8: return spb / 2.0;  // eighth
        case 16: return spb / 4.0;  // sixteenth
        case 32: return spb / 8.0;  // thirty-second
    }
    return -1.0;
}
/* clang-format on */
