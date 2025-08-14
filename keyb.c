#include "notes.h"
#include "waves.h"
#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct s_key {
    uint32_t keysym;
    int index;
    char *note;
    bool is_pressed;
    uint32_t ticks;
} t_key;

t_key keys[] = {
    {SDLK_z, 48, "c4", false, 0},
    {SDLK_s, 49, "c#4", false, 0},
    {SDLK_x, 50, "d4", false, 0},
    {SDLK_d, 51, "d#4", false, 0},
    {SDLK_c, 52, "e4", false, 0},
    {SDLK_v, 53, "f4", false, 0},
    {SDLK_g, 54, "f#4", false, 0},
    {SDLK_b, 55, "g4", false, 0},
    {SDLK_h, 56, "g#4", false, 0},
    {SDLK_n, 57, "a4", false, 0},
    {SDLK_j, 58, "a#4", false, 0},
    {SDLK_m, 59, "b4", false, 0},

    {SDLK_COMMA, 60, "c5", false, 0},
    {SDLK_l, 61, "c#5", false, 0},
    {SDLK_PERIOD, 62, "d5", false, 0},
    {SDLK_SEMICOLON, 63, "d#5", false, 0},
    {SDLK_SLASH, 64, "e5", false, 0},

    {SDLK_q, 60, "c5", false, 0},
    {SDLK_2, 61, "c#5", false, 0},
    {SDLK_w, 62, "d5", false, 0},
    {SDLK_3, 63, "d#5", false, 0},
    {SDLK_e, 64, "e5", false, 0},
    {SDLK_r, 65, "f5", false, 0},
    {SDLK_5, 66, "f#5", false, 0},
    {SDLK_t, 67, "g5", false, 0},
    {SDLK_6, 68, "g#5", false, 0},
    {SDLK_y, 69, "a5", false, 0},
    {SDLK_7, 70, "a#5", false, 0},
    {SDLK_u, 71, "b5", false, 0},

    {SDLK_i, 72, "c6", false, 0},
    {SDLK_9, 73, "c#6", false, 0},
    {SDLK_o, 74, "d6", false, 0},
    {SDLK_0, 75, "d#6", false, 0},
    {SDLK_p, 76, "e6", false, 0},

    {SDLK_LEFTBRACKET, 77, "f6", false, 0},
    {SDLK_EQUALS, 78, "f#6", false, 0},
    {SDLK_RIGHTBRACKET, 79, "g6", false, 0},
    {SDLK_BACKSPACE, 80, "g#6", false, 0},
    {SDLK_BACKSLASH, 81, "a6", false, 0},
};

#define NUM_KEYS (sizeof(keys) / sizeof(t_key))
#define NUM_SAMPLES 1024
#define SFREQ 44100
#define USAGE "usage: keyb [--sine | --square | --sawtooth | --triangle]"

int16_t (*wave)(double, uint32_t);

void audio_callback(void *userdata, Uint8 *stream, int len) {
    int16_t sample, *ptr;
    double freq;
    uint32_t i, j, k;

    (void)userdata;

    memset(stream, 0, len);

    for (i = k = 0; i < NUM_KEYS; i++) {
        k += keys[i].is_pressed;
    }

    for (i = 0; i < NUM_KEYS; i++) {
        if (!keys[i].is_pressed) {
            continue;
        }

        ptr = (int16_t *)stream;
        freq = get_index_frequency(keys[i].index);
        for (j = len >> 1; j > 0; --j) {
            sample = wave(SFREQ / freq, keys[i].ticks++);
            sample = sample * (1.0 / (double)k);
            sample = (*ptr) + sample;
            *ptr++ = sample;
        }
    }
}

void set_key_states(uint32_t key, bool state) {
    uint32_t i;

    for (i = 0; i < NUM_KEYS; i++) {
        if (keys[i].keysym == key) {
            keys[i].is_pressed = state;
        }
    }
}

int main(int argc, char **argv) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_AudioSpec want;
    SDL_AudioDeviceID dev;
    volatile bool quit = false;

    wave = &sine;
    if (argc > 1) {
        if (!strcmp(argv[1], "--sine"))
            wave = &sine;
        else if (!strcmp(argv[1], "--square"))
            wave = &square;
        else if (!strcmp(argv[1], "--sawtooth"))
            wave = &sawtooth;
        else if (!strcmp(argv[1], "--triangle"))
            wave = &triangle;
        else {
            puts(USAGE);
            return EXIT_FAILURE;
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Failed to initialise SDL\n");
        return EXIT_FAILURE;
    }

    SDL_memset(&want, 0, sizeof(want));
    want.freq = SFREQ;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = NUM_SAMPLES;
    want.callback = audio_callback;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    if (dev == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_PauseAudioDevice(dev, 0);

    window = SDL_CreateWindow("keyb", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 400, 300, 0);
    if (!window) {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                set_key_states(event.key.keysym.sym, true);
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
                break;
            case SDL_KEYUP:
                set_key_states(event.key.keysym.sym, false);
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_CloseAudioDevice(dev);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
