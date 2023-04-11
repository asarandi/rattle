/*
** https://www.mobilefish.com/tutorials/rtttl/rtttl_quickguide_specification.html
*/

#include "errors.h"
#include "mystrings.h"
#include "notes.h"
#include "ringtone.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int count_notes(char *s) {
    if (!strlen(s))
        return 0;
    return count_chars(s, ',') + 1;
}

static int isnotename(int c) {
    return ('a' <= c && c <= 'h') || (c == '#') || (c == 'p');
}

static int isdot(int c) { return c == '.'; }

static char *parse_note(char *s, struct ringtone *o, struct note *note) {
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
    i = strlen(s);
    if (i > 1 && isdigit(s[i - 1]) && isdot(s[i - 2]))
        s[i - 2] = s[i - 1], s[i - 1] = '.';

    for (i = 0; i < 4; i++) {
        *(items[i].buf) = j = 0;
        while (s[j] && items[i].f(s[j]) && j < 3) {
            items[i].buf[j] = s[j];
            items[i].buf[++j] = '\0';
        }
        s = s + j;
    }
    if (*s)
        return ERR_NOTEPARSE;

    /* dot */
    s = items[3].buf;
    i = (*s == '\0') || (*s == '.' && *(s + 1) == '\0');
    if (!i)
        return ERR_NOTEDOT;
    j = *s == '.';

    /* duration */
    s = items[0].buf;
    i = (*s == '\0') ? o->duration : atoi(s);
    note->type = i, note->dotted = j;
    note->duration = get_note_duration(o->bpm, i, j);
    if (note->duration == -1.0)
        return ERR_NOTEDUR;

    note->dur = (uint32_t)(note->duration * (double)o->sfreq);

    /* octave */
    s = items[2].buf;
    i = (*s == '\0') ? o->octave : atoi(s);
    if (!(4 <= i && i <= 7))
        return ERR_NOTEOCT;
    note->octave = i;

    /* name/index */
    s = items[1].buf;
    note->index = get_note_index(s);

    /* freq/pause */
    if (note->index == -1) {
        if (!(*s == 'p' && *(s + 1) == '\0'))
            return ERR_NOTENAM;
        note->steps = note->frequency = 0.0;
    } else {
        i = note->octave * 12 + note->index + o->transpose;
        note->octave = i / 12, note->index = i % 12;
        note->frequency = get_index_frequency(i);
    }

    if (note->frequency > 0.0) {
        note->steps = (double)o->sfreq / note->frequency;
    }

    return NULL;
}

static char *parse_notes(char *s, struct ringtone *o) {
    char **arr, *error;
    int i;

    if (!(o->num_notes = count_notes(s)))
        return ERR_NONOTES;

    if (!(o->notes = calloc(o->num_notes, sizeof(struct note))))
        return ERR_MALLOC;

    if (!(arr = str_split(s, ','))) {
        return ERR_MALLOC;
    }

    error = NULL;
    for (i = 0; arr[i]; i++) {
        error = parse_note(arr[i], o, &(o->notes[i]));
        if (error)
            break;
    }
    (void)free(arr);
    return error;
}

static char *parse_settings(char *s, struct ringtone *o) {
    struct {
        char *k;
        int *v;
    } items[] = {
        {"d=", &(o->duration)},
        {"o=", &(o->octave)},
        {"b=", &(o->bpm)},
    };
    char **arr, *error;
    int i;

    if (count_chars(s, ',') != 2)
        return ERR_NUMCOMMAS;

    if (!(arr = str_split(s, ',')))
        return ERR_MALLOC;

    error = NULL;
    for (i = 0; i < 3; i++) {
        s = arr[i];
        if (strncmp(items[i].k, s, 2)) {
            error = ERR_BADKVSETT;
            break;
        }
        if (!*(s + 2) || !is_all_digits(s + 2)) {
            error = ERR_BADKVSETT;
            break;
        }
        *(items[i].v) = atoi(s + 2);
    }
    (void)free(arr);
    return error;
}

char *parse_ringtone(char *s, struct ringtone *o) {
    char *settings, *notes;

    if (!(s = copy_without_spaces(s)))
        return ERR_MALLOC;
    o->name = s;

    if (count_chars(s, ':') != 2) {
        return ERR_NUMCOLONS;
    }

    (void)str_tolower(strchr(s, ':'));
    o->copy = strdup(s);

    settings = strchr(s, ':');
    *settings++ = '\0';
    notes = strchr(settings, ':');
    *notes++ = '\0';

    if ((s = parse_settings(settings, o))) {
        return s;
    }
    return parse_notes(notes, o);
}

/* free notes first because "raw" is offset in "name" */
void free_ringtone_data(struct ringtone *o) {
    if (o->notes)
        (void)free(o->notes);
    o->notes = NULL;

    if (o->copy)
        (void)free(o->copy);
    o->copy = NULL;

    if (o->name)
        (void)free(o->name);
    o->name = NULL;
}
