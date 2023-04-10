#ifndef MYSTRINGS_H
#define MYSTRINGS_H

#include <stdbool.h>

char *remove_spaces(char *s);
char *copy_without_spaces(char *s);
int count_chars(char *s, int c);
char *str_tolower(char *s);
char **str_split(char *s, int c);
bool is_all_digits(char *s);

#endif
