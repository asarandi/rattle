#include "errors.h"
#include "notes.h"
#include "ringtone.h"
#include "samples.h"
#include <SDL.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define SFREQ 44100
#define FADE 2.0

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
    int16_t *ptr;

    ptr = (int16_t *)stream;
    while (len) {
        (void)print_details((struct ringtone *)userdata);
        *ptr++ = next_sample((struct ringtone *)userdata);
        len -= 2;
    }
}

int main(int argc, char *argv[]) {
    SDL_AudioDeviceID dev;
    SDL_AudioSpec desired, obtained;
    SDL_Event event;
    struct ringtone *ringtone;
    volatile int done;
    char *s;
    int opt, verbose, nofade, transpose;
    extern int optind;
    int16_t (*wave)(double, uint32_t);

    wave = &sine;
    verbose = nofade = transpose = 0;

    while ((opt = getopt(argc, argv, "0123vft:")) != -1) {
        switch (opt) {
        case '0':
            wave = &sine;
            break;
        case '1':
            wave = &square;
            break;
        case '2':
            wave = &triangle;
            break;
        case '3':
            wave = &sawtooth;
            break;
        case 'v':
            verbose = 1;
            break;
        case 'f':
            nofade = 1;
            break;
        case 't':
            transpose = atoi(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "usage: %s [-0|-1|-2|-3] [-f] [-t n] song\n",
                    argv[0]);
            fprintf(stderr, "   -0    sine\n");
            fprintf(stderr, "   -1    square\n");
            fprintf(stderr, "   -2    triangle\n");
            fprintf(stderr, "   -3    sawtooth\n");
            fprintf(stderr, "   -v    verbose\n");
            fprintf(stderr, "   -f    disable fade\n");
            fprintf(stderr, "   -t n  transpose n steps\n");
            return 1;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "expecting rtttl song after options\n");
        return 1;
    }

    ringtone = calloc(1, sizeof(struct ringtone));
    if (!ringtone) {
        (void)fprintf(stderr, "%s\n", ERR_MALLOC);
        return 1;
    }

    ringtone->sfreq = SFREQ;
    ringtone->wave = wave;
    ringtone->verbose = verbose;
    ringtone->nofade = nofade;
    ringtone->transpose = transpose;

    if ((s = parse_ringtone(argv[optind], ringtone))) {
        (void)free_ringtone_data(ringtone);
        (void)free(ringtone);
        (void)fprintf(stderr, "%s\n", s);
        return 1;
    }

    (void)srand(time(NULL));

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        (void)free_ringtone_data(ringtone);
        (void)free(ringtone);
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
    desired.userdata = ringtone;

    dev = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
    if (!dev) {
        (void)free_ringtone_data(ringtone);
        (void)free(ringtone);
        (void)SDL_Log("%s", SDL_GetError());
        (void)SDL_Quit();
        return 1;
    }

    if (SDL_memcmp(&desired, &obtained, sizeof(SDL_AudioSpec)) != 0) {
        (void)free_ringtone_data(ringtone);
        (void)free(ringtone);
        (void)SDL_Log("did not get desired audio spec");
        (void)SDL_CloseAudioDevice(dev);
        (void)SDL_Quit();
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

    (void)SDL_CloseAudioDevice(dev);
    (void)SDL_Quit();

    (void)free_ringtone_data(ringtone);
    (void)free(ringtone);
    return 0;
}
