#ifndef ERRORS_H
#define ERRORS_H

/* clang-format off */
#define ERR_NUMCOLONS "ringtone: expecting 2 colons as delimiters - name:settings:notes"
#define ERR_NUMCOMMAS "settings: expecting 2 commas as delimiters - d=4,o=6,b=112"
#define ERR_BADKVSETT "settings: bad key value pair"
#define ERR_NOTEPARSE "note parse"
#define ERR_NOTEDOT   "note dot"
#define ERR_NOTEDUR   "note duration"
#define ERR_NOTEOCT   "note octave"
#define ERR_NOTENAM   "note name"
#define ERR_NONOTES   "notes: empty"
#define ERR_MALLOC    "malloc() failed"
/* clang-format on */

#endif
