#include "notes.h"
#include "samples.h"
#include <SDL.h>
#include <stdint.h>
#include <time.h>

#define SFREQ 44100
#define NOTE 110

struct wave {
    double sfreq;
    double note;
    double steps;
    uint32_t pos;
    uint32_t ct;
};

void audio_callback(void *userdata, Uint8 *stream, int len) {
    (void)userdata;
    static struct wave wave, *w;
    static int fi;
    int16_t *ptr;
    int i;
    int16_t (*wfunc[])(double steps, uint32_t pos) = {&sine, &triangle, &square,
                                                      &sawtooth};

    if (!w) {
        w = &wave;
        w->sfreq = SFREQ;
        w->note = NOTE;
        w->steps = w->sfreq / w->note;
    }

    ptr = (int16_t *)stream;
    for (i = 0; i < len / 2; i++) {
        ptr[i] = wfunc[fi](w->steps, w->pos);
        w->pos = (w->pos + 1) % (uint32_t)w->steps;
        if (w->pos == 0) {
            w->note += 0.71;
            w->steps = w->sfreq / w->note;
            w->ct++;
            if (w->ct % 1 == 0) {
                fi = (fi + 1) % 4;
            }
        }
    }
}

int main() {

    for (int i = 0; i <= 96; i++) {
        char *note = get_index_name(i % 12);
        int octave = i / 12;
        double freq = get_index_frequency(i);
        double period = (double)SFREQ / freq;

        fprintf(stderr, "i=%d note=%s octave=%d freq=%.4f period=%.4f\n", i,
                note, octave, freq, period);
    }
    return 0;

    SDL_AudioDeviceID dev;
    SDL_AudioSpec desired, obtained;

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

    (void)SDL_PauseAudioDevice(dev, 0); // unpause
    (void)SDL_Delay(5000);              // millis

    (void)SDL_CloseAudioDevice(dev);
    (void)SDL_Quit();
    return 0;
}
