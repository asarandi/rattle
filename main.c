#include "notes.h"
#include "ringtone.h"
#include "samples.h"
#include <SDL.h>
#include <stdint.h>
#include <time.h>

#define SFREQ 44100
#define FADE 2.0

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

    if (n->pos < (n->steps * FADE))
        s *= (double)n->pos / (n->steps * FADE);

    v = (double)(n->dur - n->pos);
    if (v < (n->steps * FADE))
        s *= v / (n->steps * FADE);

    o->num_played += (++(n->pos) == n->dur);
    return s;
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
    int16_t *ptr;

    ptr = (int16_t *)stream;
    while (len) {
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

    if (argc != 2)
        return 1;

    ringtone = calloc(1, sizeof(struct ringtone));
    if (!ringtone) {
        return 1;
    }
    ringtone->wave = &square;
    ringtone->sfreq = SFREQ;

    if ((s = parse_ringtone(argv[1], ringtone))) {
        (void)free_ringtone_data(ringtone);
        (void)fprintf(stderr, "%s\n", s);
        return 1;
    }

    (void)srand(time(NULL));

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
    desired.userdata = ringtone;

    dev = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
    if (!dev) {
        (void)SDL_Log("%s", SDL_GetError());
        (void)SDL_Quit();
        return 1;
    }

    if (SDL_memcmp(&desired, &obtained, sizeof(SDL_AudioSpec)) != 0) {
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

    (void)free(ringtone);
    return 0;
}
