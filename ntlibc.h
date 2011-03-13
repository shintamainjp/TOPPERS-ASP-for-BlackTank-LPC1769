#ifndef NTLIBC_H
#define NTLIBC_H

#ifndef size_t
typedef unsigned int size_t;
#endif

size_t strlen(const char *s);
char *strcpy(char *des, const char *src);

#endif

