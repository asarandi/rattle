#include "errors.h"
#include "notes.h"
#include "ringtone.h"
#include "waves.h"
#include <SDL.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

/* globals */
SDL_AudioDeviceID dev;
struct ringtone *ringtone;

void print_details(struct ringtone *o) {
    struct note *n;
    char buf[8];

    if (!o->verbose)
        return;
    if (o->num_played == o->num_notes)
        return;

    n = &(o->notes[o->num_played]);
    if (n->pos)
        return;

    if (!o->num_played) {
        (void)fprintf(stdout, "    name: %s\n", o->name);
        (void)fprintf(stdout,
                      "defaults: duration = %d, octave = %d, beats = %d\n",
                      o->duration, o->octave, o->bpm);
        (void)fprintf(stdout, "    data: %s\n", strrchr(o->copy, ':') + 1);
    }

    (void)snprintf(buf, 8, "%d%s%d%s,", n->type, get_index_name(n->index),
                   n->octave, n->dotted ? "." : "");
    if (n->frequency == 0.0)
        (void)snprintf(buf, 8, "%d%s%s,", n->type, "p", n->dotted ? "." : "");
    (void)fprintf(stdout,
                  "  note name: %-7s frequency: %7.2f Hz, duration: %2.4f s\n",
                  buf, n->frequency, n->duration);
}

int16_t next_sample(struct ringtone *o) {
    struct note *n;
    int16_t s;
    double v;

    if (o->num_played == o->num_notes)
        return 0;

    n = &(o->notes[o->num_played]);

    if (n->steps == 0.0) {
        o->num_played += (++(n->pos) == n->dur);
        return 0;
    }

    s = o->wave(n->steps, n->pos);

    if (!o->nofade && n->pos < (n->steps * FADE))
        s *= (double)n->pos / (n->steps * FADE);

    v = (double)(n->dur - n->pos);
    if (!o->nofade && v < (n->steps * FADE))
        s *= v / (n->steps * FADE);

    o->num_played += (++(n->pos) == n->dur);
    return s;
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
    (void)userdata;
    int16_t *ptr;

    ptr = (int16_t *)stream;
    while (len) {
        (void)print_details(ringtone);
        *ptr++ = next_sample(ringtone);
        len -= 2;
    }
}

void show_usage(char *p) {
    (void)fprintf(stderr, "usage: %s [-0|-1|-2|-3] [-v] [-f] [-t n] song\n", p);
    (void)fprintf(stderr, "    -0    sine\n");
    (void)fprintf(stderr, "    -1    square\n");
    (void)fprintf(stderr, "    -2    triangle\n");
    (void)fprintf(stderr, "    -3    sawtooth\n");
    (void)fprintf(stderr, "    -v    verbose\n");
    (void)fprintf(stderr, "    -f    disable fade\n");
    (void)fprintf(stderr, "    -t n  transpose n steps\n");
}

void cleanup(void) {
    if (dev) {
        (void)SDL_CloseAudioDevice(dev);
        dev = 0;
    }

    (void)SDL_Quit();

    if (ringtone) {
        (void)free_ringtone_data(ringtone);
        (void)free(ringtone);
        ringtone = NULL;
    }
}

void init_or_quit(int argc, char *argv[]) {
    extern int optind;
    int opt;
    char *s;

    if (atexit(&cleanup)) {
        (void)fprintf(stderr, "%s\n", ERR_ATEXIT);
        (void)exit(1);
    }

    ringtone = calloc(1, sizeof(struct ringtone));
    if (!ringtone) {
        (void)fprintf(stderr, "%s\n", ERR_MALLOC);
        (void)exit(1);
    }
    ringtone->wave = &sine;

    while ((opt = getopt(argc, argv, "0123vft:")) != -1) {
        switch (opt) {
        case '0':
            ringtone->wave = &sine;
            break;
        case '1':
            ringtone->wave = &square;
            break;
        case '2':
            ringtone->wave = &triangle;
            break;
        case '3':
            ringtone->wave = &sawtooth;
            break;
        case 'v':
            ringtone->verbose = 1;
            break;
        case 'f':
            ringtone->nofade = 1;
            break;
        case 't':
            ringtone->transpose = atoi(optarg);
            break;
        default: /* '?' */
            (void)show_usage(argv[0]);
            (void)exit(1);
        }
    }

    if (optind >= argc) {
        (void)show_usage(argv[0]);
        (void)exit(1);
    }

    if ((s = parse_ringtone(argv[optind], ringtone))) {
        (void)fprintf(stderr, "%s\n", s);
        (void)exit(1);
    }
}

int main(int argc, char *argv[]) {
    SDL_AudioSpec desired, obtained;
    SDL_Event event;
    volatile int done;

    (void)init_or_quit(argc, argv);

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        (void)SDL_Log("%s", SDL_GetError());
        return 1;
    }

    (void)SDL_memset(&desired, 0, sizeof(SDL_AudioSpec));
    (void)SDL_memset(&obtained, 0, sizeof(SDL_AudioSpec));

    desired.freq = SFREQ;
    desired.format = AUDIO_S16;
    desired.channels = 1;
    desired.samples = 4096;
    desired.size = 4096 * 2 * 1;
    desired.callback = &audio_callback;

    dev = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
    if (!dev) {
        (void)SDL_Log("%s", SDL_GetError());
        return 1;
    }

    if (SDL_memcmp(&desired, &obtained, sizeof(SDL_AudioSpec)) != 0) {
        (void)SDL_Log("did not get desired audio spec");
        return 1;
    }

    /* unpause */
    (void)SDL_PauseAudioDevice(dev, 0);

    for (done = 0; !done;) {
        done = ringtone->num_played == ringtone->num_notes;
        (void)SDL_PollEvent(&event);
        done |= event.type == SDL_QUIT;
        /* millis */
        (void)SDL_Delay(20);
    }

    return 0;
}
