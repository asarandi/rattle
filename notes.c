#include <math.h>
#include <string.h>

/* clang-format off */
struct {
    char *name;
    int index;
} note_indices[] = {
    { "a", 0},
    {"a#", 1},
    {"bb", 1},
    { "b", 2},
    { "c", 3},
    {"c#", 4},
    {"db", 4},
    { "d", 5},
    {"d#", 6},
    {"eb", 6},
    { "e", 7},
    { "f", 8},
    {"f#", 9},
    {"gb", 9},
    { "g", 10},
    {"g#", 11},
    {"ab", 11},
};
/* clang-format on */

#define NUM_NOTE_INDICES 17

int get_note_index(char *note_name) {
    int i;

    for (i = 0; i < NUM_NOTE_INDICES; i++) {
        if (!strcmp(note_name, note_indices[i].name)) {
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
** index 0 = A0 = 27.5 Hz
*/
double get_index_frequency(int index) {
    return pow(2.0, (double)index / 12.0) * 27.5;
}

/* clang-format off */
double get_note_duration(int bpm, int note_type) {
    double spb = 60.0 / (double)bpm;
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
