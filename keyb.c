#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

/*
    http://pages.mtu.edu/~suits/notefreqs.html
    Frequencies for equal-tempered scale, A4 = 440 Hz
    Note | Frequency(Hz)
    Cs0 = C#0 = Db0
    ("Middle C" is C4 )
*/

typedef struct  s_note
{
    char    *name;
    double  freq;
}               t_note;

t_note  notes[] = {
    {"c0",  16.35},
    {"c#0", 17.32},
    {"db0", 17.32},
    {"d0",  18.35},
    {"d#0", 19.45},
    {"eb0", 19.45},
    {"e0",  20.60},
    {"f0",  21.83},
    {"f#0", 23.12},
    {"gb0", 23.12},
    {"g0",  24.50},
    {"ab0", 25.96},
    {"g#0", 25.96},
    {"a0",  27.50},
    {"a#0", 29.14},
    {"bb0", 29.14},
    {"b0",  30.87},
    {"c1",  32.70},
    {"c#1", 34.65},
    {"db1", 34.65},
    {"d1",  36.71},
    {"d#1", 38.89},
    {"eb1", 38.89},
    {"e1",  41.20},
    {"f1",  43.65},
    {"f#1", 46.25},
    {"gb1", 46.25},
    {"g1",  49.00},
    {"ab1", 51.91},
    {"g#1", 51.91},
    {"a1",  55.00},
    {"a#1", 58.27},
    {"bb1", 58.27},
    {"b1",  61.74},
    {"c2",  65.41},
    {"c#2", 69.30},
    {"db2", 69.30},
    {"d2",  73.42},
    {"d#2", 77.78},
    {"eb2", 77.78},
    {"e2",  82.41},
    {"f2",  87.31},
    {"f#2", 92.50},
    {"gb2", 92.50},
    {"g2",  98.00},
    {"ab2", 103.83},
    {"g#2", 103.83},
    {"a2",  110.00},
    {"a#2", 116.54},
    {"bb2", 116.54},
    {"b2",  123.47},
    {"c3",  130.81},
    {"c#3", 138.59},
    {"db3", 138.59},
    {"d3",  146.83},
    {"d#3", 155.56},
    {"eb3", 155.56},
    {"e3",  164.81},
    {"f3",  174.61},
    {"f#3", 185.00},
    {"gb3", 185.00},
    {"g3",  196.00},
    {"ab3", 207.65},
    {"g#3", 207.65},
    {"a3",  220.00},
    {"a#3", 233.08},
    {"bb3", 233.08},
    {"b3",  246.94},
    {"c4",  261.63},
    {"c#4", 277.18},
    {"db4", 277.18},
    {"d4",  293.66},
    {"d#4", 311.13},
    {"eb4", 311.13},
    {"e4",  329.63},
    {"f4",  349.23},
    {"f#4", 369.99},
    {"gb4", 369.99},
    {"g4",  392.00},
    {"ab4", 415.30},
    {"g#4", 415.30},
    {"a4",  440.00},
    {"a#4", 466.16},
    {"bb4", 466.16},
    {"b4",  493.88},
    {"c5",  523.25},
    {"c#5", 554.37},
    {"db5", 554.37},
    {"d5",  587.33},
    {"d#5", 622.25},
    {"eb5", 622.25},
    {"e5",  659.25},
    {"f5",  698.46},
    {"f#5", 739.99},
    {"gb5", 739.99},
    {"g5",  783.99},
    {"ab5", 830.61},
    {"g#5", 830.61},
    {"a5",  880.00},
    {"a#5", 932.33},
    {"bb5", 932.33},
    {"b5",  987.77},
    {"c6",  1046.50},
    {"c#6", 1108.73},
    {"db6", 1108.73},
    {"d6",  1174.66},
    {"d#6", 1244.51},
    {"eb6", 1244.51},
    {"e6",  1318.51},
    {"f6",  1396.91},
    {"f#6", 1479.98},
    {"gb6", 1479.98},
    {"g6",  1567.98},
    {"ab6", 1661.22},
    {"g#6", 1661.22},
    {"a6",  1760.00},
    {"a#6", 1864.66},
    {"bb6", 1864.66},
    {"b6",  1975.53},
    {"c7",  2093.00},
    {"c#7", 2217.46},
    {"db7", 2217.46},
    {"d7",  2349.32},
    {"d#7", 2489.02},
    {"eb7", 2489.02},
    {"e7",  2637.02},
    {"f7",  2793.83},
    {"f#7", 2959.96},
    {"gb7", 2959.96},
    {"g7",  3135.96},
    {"ab7", 3322.44},
    {"g#7", 3322.44},
    {"a7",  3520.00},
    {"a#7", 3729.31},
    {"bb7", 3729.31},
    {"b7",  3951.07},
    {"c8",  4186.01},
    {"c#8", 4434.92},
    {"db8", 4434.92},
    {"d8",  4698.63},
    {"d#8", 4978.03},
    {"eb8", 4978.03},
    {"e8",  5274.04},
    {"f8",  5587.65},
    {"f#8", 5919.91},
    {"gb8", 5919.91},
    {"g8",  6271.93},
    {"ab8", 6644.88},
    {"g#8", 6644.88},
    {"a8",  7040.00},
    {"a#8", 7458.62},
    {"bb8", 7458.62},
    {"b8",  7902.13}
};

#define num_notes 153


typedef struct  s_key
{
    uint32_t    keysym;
    char        *note;
    bool        is_pressed;
    uint32_t    ticks;

}               t_key;

/*
 ________________________________________________________
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |
|  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |  |_| |_| |_|  |
|   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|___|___|___|___|___|___|___|___|___|___|___|___|___|___|

    2   3       5   6   7       9   0       =   BS
  q   w   e   r   t   y   u   i   o   p   [   ]   \

 _______________________________________
|  | | | |  |  | | | | | |  |  | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |
|  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |
|   |   |   |   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |   |   |   |
|___|___|___|___|___|___|___|___|___|___|

    s   d       g   h   j       l   ;
  z   x   c   v   b   n   m   ,   .   /

*/

t_key   keys[] = {
    {SDLK_z, "c4", false, 0},
    {SDLK_s, "c#4", false, 0},
    {SDLK_x, "d4", false, 0},
    {SDLK_d, "d#4", false, 0},
    {SDLK_c, "e4", false, 0},
    {SDLK_v, "f4", false, 0},
    {SDLK_g, "f#4", false, 0},
    {SDLK_b, "g4", false, 0},
    {SDLK_h, "g#4", false, 0},
    {SDLK_n, "a4", false, 0},
    {SDLK_j, "a#4", false, 0},
    {SDLK_m, "b4", false, 0},


    {SDLK_COMMA, "c5", false, 0},
    {SDLK_l, "c#5", false, 0},
    {SDLK_PERIOD, "d5", false, 0},
    {SDLK_SEMICOLON, "d#5", false, 0},
    {SDLK_SLASH, "e5", false, 0},

    {SDLK_q, "c5", false, 0},
    {SDLK_2, "c#5", false, 0},
    {SDLK_w, "d5", false, 0},
    {SDLK_3, "d#5", false, 0},
    {SDLK_e, "e5", false, 0},
    {SDLK_r, "f5", false, 0},
    {SDLK_5, "f#5", false, 0},
    {SDLK_t, "g5", false, 0},
    {SDLK_6, "g#5", false, 0},
    {SDLK_y, "a5", false, 0},
    {SDLK_7, "a#5", false, 0},
    {SDLK_u, "b5", false, 0},


    {SDLK_i, "c6", false, 0},
    {SDLK_9, "c#6", false, 0},
    {SDLK_o, "d6", false, 0},
    {SDLK_0, "d#6", false, 0},
    {SDLK_p, "e6", false, 0},

    {SDLK_LEFTBRACKET, "f6", false, 0},
    {SDLK_EQUALS, "f#6", false, 0},
    {SDLK_RIGHTBRACKET, "g6", false, 0},
    {SDLK_BACKSPACE, "g#6", false, 0},
    {SDLK_BACKSLASH, "a6", false, 0},
};

#define WND_WIDTH   400
#define WND_HEIGHT  300

#define num_notes 153

#define num_channels 2
#define num_samples 1024
#define sampling_frequency  44100

#define usage_msg "usage: keyb [--sine | --square | --sawtooth | --triangle]"

#define num_keys    (sizeof(keys) / sizeof(t_key))
static bool    done = false;

int16_t (*wave_gen)(double time, double freq, double amp);

/* https://www.youtube.com/watch?v=5xd9BMxoXqo */
int16_t SineWave(double time, double freq, double amp) {
    int16_t result;
    double tpc = sampling_frequency / freq; // ticks per cycle
    double cycles = time / tpc;
    double rad = M_PI * 2 * cycles;
    int16_t amplitude = INT16_MAX * amp;
    result = amplitude * sin(rad);
    return result;
}

int16_t SquareWave(double time, double freq, double amp) {
    int16_t result = 0;
    int tpc = sampling_frequency / freq; // ticks per cycle
    int cyclepart = (int)time % tpc;
    int halfcycle = tpc / 2;
    int16_t amplitude = INT16_MAX * amp;
    if (cyclepart < halfcycle) {
        result = amplitude;
    }
    return result;
}

int16_t SawtoothWave(double time, double freq, double amp) {
    int tpc = sampling_frequency / freq; // ticks per cycle
    double cyclepart = fmod(time, tpc);
    double percent = cyclepart / tpc;
    int16_t result = INT16_MAX * amp * percent;
    return result;
}

int16_t TriangleWave(double time, double freq, double amp) {
    int tpc = sampling_frequency / freq; // ticks per cycle
    double cyclepart = fmod(time, tpc);
    double halfcycle = tpc / 2;
    double percent;
    percent = cyclepart / halfcycle;
    if (cyclepart >= halfcycle)
        percent = 2.0 - percent;
    int16_t result = INT16_MAX * amp * percent;
    return result;
}

double  get_note_freq(char *note)
{
    for (int i = 0; i < num_notes; i++) {
        if (!strcmp(notes[i].name, note))
            return notes[i].freq;
    }
    return 0;
}

void MyAudioCallback(void *userdata, Uint8 *stream, int len)
{
    int16_t data;
    int32_t calc;
    double  freq;

    (void)userdata;

    memset(stream, 0, len);

    for (uint32_t i = 0; i < num_keys; i++)
    {
        if (!keys[i].is_pressed)
            continue ;

        freq = get_note_freq(keys[i].note);
        for (uint32_t j = 0; j < num_samples; j++)
        {
            data = wave_gen(keys[i].ticks++, freq, 1.0);
            calc = (*(int16_t *)&stream[j * 4] + data) >> 1;
            data = calc;
            *(int16_t *)&stream[j * 4] = data;
            *(int16_t *)&stream[j * 4 + 2] = data;
        }
    }
}

void set_key_states(uint32_t key, bool state)
{
    for (uint32_t i = 0; i < num_keys; i++)
    {
        if (keys[i].keysym == key)
        {
            keys[i].is_pressed = state;
        }
    }
}

int main(int argc, char **argv)
{
    SDL_Window          *gui_window;
    SDL_Renderer        *gui_renderer;
    SDL_Event           event;
    SDL_AudioSpec       want;
    SDL_AudioDeviceID   dev;

    wave_gen = &SineWave;
    if (argc > 1)
    {        
        if (!strcmp(argv[1], "--sine"))
            wave_gen = &SineWave;
        else if (!strcmp(argv[1], "--square"))
            wave_gen = &SquareWave;        
        else if (!strcmp(argv[1], "--sawtooth"))
            wave_gen = &SawtoothWave;
        else if (!strcmp(argv[1], "--triangle"))
            wave_gen = &TriangleWave;
        else
        {
            puts(usage_msg);
            return EXIT_FAILURE;
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Failed to initialise SDL\n");
        return EXIT_FAILURE;
    }

    SDL_memset(&want, 0, sizeof(want));
    want.freq = sampling_frequency;
    want.format = AUDIO_S16;
    want.channels = num_channels;
    want.samples = num_samples;
    want.callback = MyAudioCallback;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    if (dev == 0)
    {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_PauseAudioDevice(dev, 0);
    
    if ((gui_window = SDL_CreateWindow("sdl2-keyb-synth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WND_WIDTH, WND_HEIGHT,0)) == NULL) {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if ((gui_renderer = SDL_CreateRenderer(gui_window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }


    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
                case SDL_QUIT:
                    done = true;
                    break ;
                case SDL_KEYDOWN:
                    set_key_states(event.key.keysym.sym, true);
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break ;
                case SDL_KEYUP:
                    set_key_states(event.key.keysym.sym, false);
                    break ;
            }
        }

        SDL_RenderClear(gui_renderer);
        SDL_RenderPresent(gui_renderer);
        SDL_Delay(10);        
    }

    SDL_CloseAudioDevice(dev);
    SDL_DestroyRenderer(gui_renderer);
    SDL_DestroyWindow(gui_window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
