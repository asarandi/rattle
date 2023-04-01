#include <SDL.h>
#include <stdint.h>
#include <time.h>

void square(void *userdata, Uint8 *stream, int len) {
  int16_t *ptr, i, v;
  static uint32_t ti;
  double steps;

  (void)userdata;

  steps = 44100.0 / 440.0;

  ptr = (int16_t *)stream;
  for (i = 0; i < len / 4; i++) {
    v = ti > (steps / 2) ? -INT16_MAX : INT16_MAX;
    ti = (ti + 1) % (uint32_t)steps;
    ptr[i * 2] = v;
    ptr[i * 2 + 1] = v;
  }
}

void sine(void *userdata, Uint8 *stream, int len) {
  int16_t *ptr, i, v;
  static uint32_t ti;
  double inc;

  (void)userdata;

  inc = (2.0 * M_PI) / (44100.0 / 440.0);

  ptr = (int16_t *)stream;
  for (i = 0; i < len / 4; i++) {
    v = sin(inc * ti++) * INT16_MAX;
    ptr[i * 2] = v;
    ptr[i * 2 + 1] = v;
  }
}

void noise(void *userdata, Uint8 *stream, int len) {
  int16_t *ptr, i, v;

  (void)userdata;

  ptr = (int16_t *)stream;
  for (i = 0; i < len / 4; i++) {
    v = rand();
    ptr[i * 2] = v;
    ptr[i * 2 + 1] = v;
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

  desired.freq = 44100;
  desired.format = AUDIO_S16;
  desired.channels = 2;
  desired.samples = 4096;
  desired.size = 4096 * 2 * 2;
  desired.callback = &sine;

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
