
#include "ntlibc.h"

size_t strlen(const char *s)
{
    const char *p = s;
    int cnt = 0;
    while (*p) {
        cnt++;
        p++;
    }
    return cnt;
}

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

