#ifndef NOTES_H
#define NOTES_H

/*
** in: note name
** out: note index in range 0..11
** returns -1 if name not found
** example: in: "c#", out: 1
*/
int get_note_index(char *note_name);

/*
** in: index
** out: note name
** returns NULL if index not found
** example: in: 3, out: "d#"
** nb: sharps ordered before flats
*/
char *get_index_name(int index);

/*
** in: index
** out: frequency
** notes: index zero is C0 at 16.3516 Hz
*/
double get_index_frequency(int index);

/*
** in: beats per minute, note type, is_dotted
** out: duration is seconds
** notes: handles 1, 2, 4, 8, 16, 32 types
** if note is dotted, then add extra half duration
** returns -1.0 for other note types
*/
double get_note_duration(int bpm, int note_type, int is_dotted);

#endif
