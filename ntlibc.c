
#include "ntlibc.h"

char *strcpy(char *des, const char *src)
{
    char *d = des;
    char *s = src;
    while (*s) {
        *d = *s;
        d++;
        s++;
    }
    *d = '\0';
    return des;
}

