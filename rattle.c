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
#define sampling_frequency  44100

typedef struct s_notedata {
    char    *name;
    double  frequency;
    double  duration;
} t_notedata;

t_notedata  *notedata;

static char pause[] = "pause";

static int  duration = 4;
static int  octave = 4;
static int  beats = 100;
static bool is_parse_error = false;
static bool done = false;

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

void MyAudioCallback(void *userdata, Uint8 *stream, int len)
{
    static uint64_t note_idx, time_idx;
    static bool print_flag = true;
    short   data;

    (void)userdata;
    (void)len;

    if (time_idx >= notedata[note_idx].duration * sampling_frequency)
    {
        note_idx++;
        time_idx = 0;
        print_flag = true;
    }

    if (!notedata[note_idx].name)
        done = true;

    if (done)
        return ;

   data = wave_gen(time_idx++, notedata[note_idx].frequency, 1.0);
   *(int16_t *)&stream[0] = data;
   *(int16_t *)&stream[2] = data;

   if (print_flag) {
       printf("  note name: %.5s,\tfrequency: %7.2f Hz,\tduration %2.4f s\n",
               notedata[note_idx].name, notedata[note_idx].frequency, notedata[note_idx].duration);
       print_flag = false;
   }
}

char *str_trim(char *s)
{
    for (int i = 0; s[i]; i++) {
        if (isspace(s[i])) {
            int j = i;
            while (isspace(s[j]))
                j++;
            memmove(&s[i], &s[j], strlen(&s[j]) + 1);
        }
    }
    return s;
}

/*
 * if bpm = 60, a full note is 4 beats, or 4 seconds
 *      half note is 2 beats or 2 seconds,
 *      quarter note is 1 beat or 1 seconds
 *
 * if bpm = 102, a full note is 4 beats or, (60 (s) / 102) * 4 = 2.3529 seconds
*/

double  get_note_freq(char *note)
{
    for (int i = 0; i < num_notes; i++) {
        if (!strcmp(notes[i].name, note))
            return notes[i].freq;
    }
    if (note[0] != 'p')
        is_parse_error = true;
    return 0;
}


char *get_note_name_ptr(char *note)
{
    for (int i = 0; i < num_notes; i++) {
        if (!strcmp(notes[i].name, note))
            return notes[i].name;
    }
    if (note[0] == 'p')
        return pause;
    return NULL;
}

double get_note_duration(char *token)
{
    double result;

    int dur = atoi(token);
    if (!dur)
        dur = duration;
    if (dur & (dur - 1))    //must be power of 2
        is_parse_error = true;
    result = 60.0 / (double)beats * 4.0 / dur;
    if (strchr(token, '.'))
        result *= 1.5;
    return result ;
}

char *get_note_name(char *token, char *buf)
{
    char valid_chars[] = "pabcdefg#012345678";
    int i = 0, j = 0;
    while (isdigit(token[i]))
        i++;
    if (strlen(&token[i]) > 4) /*   a#4.  */
        is_parse_error = true;
    while ((j < 3) && (token[i]))
    {
        if (strchr(valid_chars, token[i]))
            buf[j++] = token[i];
        i++;
    }
    if (!j)
        is_parse_error = true;
    if (!isdigit(buf[j - 1]))
        sprintf(&buf[j], "%d", octave);
    return buf;
}


void quit_msg(char *s)
{
    puts(s);
    exit(EXIT_FAILURE);
}

#define usage_msg "usage: rattle [--sine | --square | --sawtooth | --triangle] 'ring_tone_text_string'"

void    parse(int argc, char **argv)
{
    char *s, *name, *defaults, *data;
    char delim1[] = ":";
    char delim2[] = ",";
    int k;

    if (argc < 2)
        return quit_msg(usage_msg);

    wave_gen = &SineWave;
    if (argc == 3)
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
            return quit_msg(usage_msg);
    }

    s = strchr(str_trim(argv[argc - 1]), ':');
    if (!s)
        return quit_msg("parse error");
    for (int i = 0; s[i]; i++)
        s[i] = (char)tolower(s[i]);
    s = argv[argc - 1];
    name = strtok(s, delim1);
    defaults = strtok(NULL, delim1);
    data = strtok(NULL, delim1);
    if ((!defaults) || (!data) || (strtok(NULL, delim1)))
        return quit_msg("parse error");
    if (sscanf(defaults, "d=%d,o=%d,b=%d", &duration, &octave, &beats) != 3)
        return quit_msg("parse error");

    printf("    name: %s\n", name);
    printf("defaults: duration = %d, octave = %d, beats = %d\n", duration, octave, beats);
    printf("    data: %s\n", data);

    for (size_t j = 0; j < strlen(data); j++)
        if (data[j] == '_') data[j] = '#';

    k = 0;
    s = data;
    while (true) {
        s = strchr(s, ',');
        if (!s)
            break ;
        s++;
        k++;
    }

    notedata = malloc((k + 2) * sizeof(t_notedata));
    (void)memset(notedata, 0, (k + 2) * sizeof(t_notedata));

    k = 0;
    s = strtok(data, delim2);
    char buf[8];
    while (true)
    {
        if (!s) break ;
        memset(buf, 0, 8);
        (void)get_note_name(s, buf);
        notedata[k].duration = get_note_duration(s);
        notedata[k].frequency = get_note_freq(buf);
        notedata[k].name = get_note_name_ptr(buf);
        if (is_parse_error)
        {
            printf("token: %s,  note name: %.5s,\tfrequency: %7.2f Hz,\tduration %2.4f s\n",
                s, notedata[k].name, notedata[k].frequency, notedata[k].duration);
            free(notedata);
            return quit_msg("parse error");
        }
        s = strtok(NULL, delim2);
        k++;
    }
}

int main(int argc, char **argv)
{
    SDL_AudioSpec want, have;
    SDL_AudioDeviceID dev;

    parse(argc, argv);

    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        puts("Failed to initialise SDL");
        return EXIT_FAILURE;
    }

    SDL_memset(&want, 0, sizeof(want));
    want.freq = sampling_frequency;
    want.format = AUDIO_S16;
    want.channels = 2;
    want.samples = 1;
    want.callback = MyAudioCallback;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (dev == 0)
    {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (have.format != want.format) {
        SDL_Log("We didn't get Signed16 audio format.");
    }
    SDL_PauseAudioDevice(dev, 0);
    while (!done)
        SDL_Delay(1);

    SDL_CloseAudioDevice(dev);
    SDL_Quit();

    return EXIT_SUCCESS;
}
