#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char *remove_spaces(char *s) {
    char *p, *q;

    p = q = s;
    while (*p) {
        while ((*p) && (isspace(*p)))
            p++;
        while ((*p) && (!isspace(*p)))
            *q++ = *p++;
    }
    *q = '\0';
    return s;
}

char *copy_without_spaces(char *s) {
    s = strdup(s);
    if (!s)
        return NULL;
    return remove_spaces(s);
}

int count_chars(char *s, int c) {
    int n;

    n = 0;
    while (*s)
        n += (int)(*s++ == c);
    return n;
}

char *str_tolower(char *s) {
    char *p;

    p = s;
    while (*p) {
        *p = tolower(*p);
        p++;
    }
    return s;
}

/* NB: alters input - replaces all "c" with '\0' */
char **str_split(char *s, int c) {
    char **arr;
    int i, n;

    n = count_chars(s, c);
    if (!(arr = malloc((n + 2) * sizeof(char *))))
        return NULL;
    for (i = 0; i < n + 1; i++) {
        arr[i] = s;
        if ((s = strchr(s, c)))
            *s++ = '\0';
        arr[i + 1] = s;
    }
    return arr;
}

bool is_all_digits(char *s) {
    while (*s) {
        if (!isdigit(*s++))
            return false;
    }
    return true;
}
