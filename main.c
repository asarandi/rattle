#include <SDL.h>
#include <stdint.h>
#include <time.h>

#define SFREQ 44100
#define NOTE 110

void square(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int16_t *ptr, i, v;
  static uint32_t ti;
  double steps;

  steps = (double)SFREQ / (double)NOTE;
  ptr = (int16_t *)stream;
  for (i = 0; i < len / 2; i++) {
    v = ti > (steps / 2) ? -INT16_MAX : INT16_MAX;
    ti = (ti + 1) % (uint32_t)steps;
    ptr[i] = v;
  }
}

void triangle(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int16_t *ptr, i, v;
  static uint32_t ti;
  double steps, qtr, inc;

  steps = (double)SFREQ / (double)NOTE;
  qtr = (double)SFREQ / (double)NOTE / 4.0;
  inc = (double)INT16_MAX / qtr;

  ptr = (int16_t *)stream;
  for (i = 0; i < len / 2; i++) {
    v = 0;
    if (ti < qtr) {
      v = inc * ti;
    } else if (ti < 3 * qtr) {
      v = INT16_MAX - (inc * (ti - qtr));
    } else {
      v = -INT16_MAX + (inc * (ti - (3 * qtr)));
    }
    ti = (ti + 1) % (uint32_t)steps;
    ptr[i] = v;
  }
}

void sawtooth(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int16_t *ptr, i, v;
  static uint32_t ti;
  double steps, half, inc;

  steps = (double)SFREQ / (double)NOTE;
  half = (double)SFREQ / (double)NOTE / 2.0;
  inc = (double)INT16_MAX / half;

  ptr = (int16_t *)stream;
  for (i = 0; i < len / 2; i++) {
    v = ti < half ? 0 : -INT16_MAX;
    v += ti < half ? inc * ti : inc * (ti - half);
    ti = (ti + 1) % (uint32_t)steps;
    ptr[i] = v;
  }
}

void sine(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int16_t *ptr, i, v;
  static uint32_t ti;
  double inc;

  inc = (2.0 * M_PI) / ((double)SFREQ / (double)NOTE);
  ptr = (int16_t *)stream;
  for (i = 0; i < len / 2; i++) {
    v = sin(inc * ti++) * INT16_MAX;
    ptr[i] = v;
  }
}

void noise(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  int16_t *ptr, i, v;

  ptr = (int16_t *)stream;
  for (i = 0; i < len / 2; i++) {
    v = rand();
    ptr[i] = v;
  }
}

int main() {
  SDL_AudioDeviceID dev;
  SDL_AudioSpec desired, obtained;

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
  desired.callback = &triangle;

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

  (void)srand(time(NULL));
  (void)SDL_PauseAudioDevice(dev, 0); // unpause
  (void)SDL_Delay(3000);              // millis

  (void)SDL_CloseAudioDevice(dev);
  (void)SDL_Quit();
  return 0;
}
