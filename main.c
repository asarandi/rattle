#include "notes.h"
#include "ringtone.h"
#include "samples.h"
#include <SDL.h>
#include <stdint.h>
#include <time.h>

#define SFREQ 44100

void adjust_next_note(struct ringtone *o) {
    struct note *n, *m;
    int i;

    i = o->num_played;
    if (o->num_notes - 1 <= i)
        return;
    n = &(o->notes[i]);
    m = &(o->notes[i + 1]);
    if (n->steps > 0.0 && m->steps > 0.0) {
        m->mpos = calc_pos_b(n->steps, n->mpos, m->steps);
    }
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
    int16_t *ptr;
    struct ringtone *o;
    struct note *n;
    int i;

    o = (struct ringtone *)userdata;
    ptr = (int16_t *)stream;

    i = 0;
    while ((i < len) && (o->num_played < o->num_notes)) {
        n = &(o->notes[o->num_played]);

        if (n->pos == 0) {
            fprintf(stdout, "%s\n", n->raw);
        }

        while ((i < len) && (n->pos < n->dur)) {
            if (n->frequency > 0.0) {
                o->last_sample = o->wave(n->steps, n->mpos);
                n->mpos = (n->mpos + 1) % (uint32_t)n->steps;
                o->last_steps = n->steps;
                o->last_mpos = n->mpos;
                o->last_volume = 1.0;
            } else {
                if (o->last_volume > 0.0)
                    o->last_volume -= 0.02;

                o->last_sample = o->wave(o->last_steps, o->last_mpos);
                o->last_sample *= o->last_volume;
                o->last_mpos = (o->last_mpos + 1) % (uint32_t)o->last_steps;
            }
            *ptr++ = o->last_sample;
            n->pos = n->pos + 1;
            i = i + 2;
        }

        if (n->pos == n->dur) {
            adjust_next_note(o);
            o->num_played++;
        }
    }
    (void)memset(stream + i, 0, len - i);
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
