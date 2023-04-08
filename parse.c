/*
** https://www.mobilefish.com/tutorials/rtttl/rtttl_quickguide_specification.html
*/

#include "notes.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* clang-format off */
#define ERR_NUMCOLONS "ringtone: expecting 2 colons as delimiters - name:settings:notes"
#define ERR_NUMCOMMAS "settings: expecting 2 commas as delimiters - d=4,o=6,b=112"
#define ERR_BADKVSETT "settings: bad key value pair"
#define ERR_NOTEPARSE "note parse"
#define ERR_NOTEDOT   "note dot"
#define ERR_NOTEDUR   "note duration"
#define ERR_NOTEOCT   "note octave"
#define ERR_NOTENAM   "note name"
#define ERR_NONOTES   "notes: empty"
#define ERR_MALLOC    "malloc() failed"
/* clang-format on */

struct settings {
    int duration;
    int octave;
    int bpm;
};

struct note {
    char *raw;
    int octave;
    int index;
    double duration;
    double frequency;
    double steps;
    uint32_t pos;
};

char *remove_spaces(char *s) {
    char *p, *q;

    s = p = q = strdup(s);
    if (!s)
        return NULL;
    while (*p) {
        while ((*p) && (isspace(*p)))
            p++;
        while ((*p) && (!isspace(*p)))
            *q++ = *p++;
    }
    *q = '\0';
    return s;
}

int count_chars(char *s, int c) {
    int n;

    n = 0;
    while (*s)
        n += (int)(*s++ == c);
    return n;
}

char *str_tolower(char *s) {
    char *p;

    p = s;
    while (*p) {
        *p = tolower(*p);
        p++;
    }
    return s;
}

char **str_split(char *s, int c) {
    char **arr;
    int i, n;

    n = count_chars(s, c);
    if (!(arr = malloc((n + 2) * sizeof(char *))))
        return NULL;
    for (i = 0; i < n + 1; i++) {
        arr[i] = s;
        if ((s = strchr(s, c)))
            *s++ = '\0';
        arr[i + 1] = s;
    }
    return arr;
}

int is_numeric(char *s) {
    while (*s) {
        if (!isdigit(*s++))
            return 0;
    }
    return 1;
}

char *parse_settings(char *s, struct settings *o) {
    struct {
        char *k;
        int *v;
    } items[] = {
        {"d=", &(o->duration)},
        {"o=", &(o->octave)},
        {"b=", &(o->bpm)},
    };
    char **arr;
    int i;

    if (count_chars(s, ',') != 2)
        return ERR_NUMCOMMAS;

    if (!(arr = str_split(s, ',')))
        return ERR_MALLOC;

    for (i = 0; i < 3; i++) {
        s = arr[i];
        if (strncmp(items[i].k, s, 2) || !is_numeric(s + 2)) {
            (void)free(arr);
            return ERR_BADKVSETT;
        }
        *(items[i].v) = atoi(s + 2);
    }
    (void)free(arr);
    return NULL;
}

static int count_notes(char *s) {
    if (!strlen(s))
        return 0;
    return count_chars(s, ',') + 1;
}

static int isnotename(int c) {
    return ('a' <= c && c <= 'h') || (c == '#') || (c == 'p');
}

static int isdot(int c) { return c == '.'; }

char *parse_note(char *s, struct settings *o, struct note *note) {
    int i, j;
    struct {
        char buf[4];
        int (*f)(int);
    } items[] = {
        {{0, 0, 0, 0}, &isdigit},
        {{0, 0, 0, 0}, &isnotename},
        {{0, 0, 0, 0}, &isdigit},
        {{0, 0, 0, 0}, &isdot},
    };

    note->raw = s;
    for (i = 0; i < 4; i++) {
        j = 0;
        while (s[j] && items[i].f(s[j]) && j < 3) {
            items[i].buf[j] = s[j];
            j++;
        }
        s = s + j;
    }

    if (*s)
        return ERR_NOTEPARSE;

    s = items[3].buf;
    i = (*s == '\0') || (*s == '.' && *(s + 1) == '\0');
    if (!i)
        return ERR_NOTEDOT;
    j = *s == '.';

    s = items[0].buf;
    i = (*s == '\0') ? o->duration : atoi(s);
    note->duration = get_note_duration(o->bpm, i, j);
    if (note->duration == -1.0)
        return ERR_NOTEDUR;

    s = items[2].buf;
    i = (*s == '\0') ? o->octave : atoi(s);
    if (!(4 <= i && i <= 7))
        return ERR_NOTEOCT;
    note->octave = i;

    s = items[1].buf;
    i = *s == 'p' && *(s + 1) == '\0';
    note->index = get_note_index(s);

    if (note->index == -1) {
        if (!i)
            return ERR_NOTENAM;
        note->frequency = 0.0; /* pause */
    } else {
        note->frequency = get_index_frequency(note->octave * 12 + note->index);
    }

    return NULL;
}

char *parse_notes(char *s, struct settings *o, struct note **note_arr) {
    struct note *res;
    char **arr, *error;
    int i;

    if (!(i = count_notes(s)))
        return ERR_NONOTES;

    if (!(res = malloc(i * sizeof(struct note))))
        return ERR_MALLOC;

    if (!(arr = str_split(s, ','))) {
        (void)free(res);
        return ERR_MALLOC;
    }

    for (i = 0; arr[i]; i++) {
        error = parse_note(arr[i], o, &(res[i]));
        if (error) {
            (void)free(res);
            (void)free(arr);
            return error;
        }
    }

    (void)free(arr);
    *note_arr = res;
    return NULL;
}

char *parse_ringtone(char *s) {
    char **arr, *error;
    char *s1, *s2, *s3;
    struct settings opts;
    struct note *notes;
    int i, num_notes;

    if (!(s = remove_spaces(s))) /* TODO: free */
        return ERR_MALLOC;

    (void)fprintf(stdout, "   input = [%s]\n", s);

    i = count_chars(s, ':');
    if (i != 2) {
        (void)free(s);
        return ERR_NUMCOLONS;
    }

    if (!(arr = str_split(s, ':'))) {
        (void)free(s);
        return ERR_MALLOC;
    }

    s1 = arr[0], s2 = arr[1], s3 = arr[2];
    (void)fprintf(stdout, "    name = [%s]\n", s1);
    (void)fprintf(stdout, "settings = [%s]\n", s2);
    (void)fprintf(stdout, "   notes = [%s]\n", s3);
    (void)free(arr);

    s2 = str_tolower(s2);
    if ((error = parse_settings(s2, &opts))) {
        (void)free(s);
        return error;
    }
    (void)fprintf(stdout, "settings: d=%d o=%d b=%d\n", opts.duration,
                  opts.octave, opts.bpm);

    num_notes = count_notes(s3);
    s3 = str_tolower(s3);
    if ((error = parse_notes(s3, &opts, &notes))) {
        (void)free(s);
        return error;
    }

    for (i = 0; i < num_notes; i++) {
        (void)fprintf(stderr, "raw=[%s] oct=%d index=%d dur=%.4f freq=%.4f\n",
                      notes[i].raw, notes[i].octave, notes[i].index,
                      notes[i].duration, notes[i].frequency);
    }

    return NULL;
}

int main() {
    char *s = "JingleBell : d=8, o=5, b=112 : 32p, "
              "a,a,4a,a,a,4a,a,c6,f.,16g,2a,a#,a#,a#.,16a#,a#,a,a.,16a,  "
              "a,g,g,a,4g,4c6 ";
    char *error;

    error = parse_ringtone(s);
    if (error) {
        (void)fprintf(stderr, "%s\n", error);
    }
    return 0;
}
