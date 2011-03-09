/*
 * ===============================================================
 *  Natural Tiny Shell (NT-Shell)
 *  Version 0.0.3
 * ===============================================================
 * Copyright (c) 2010-2011 Shinichiro Nakamura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * ===============================================================
 */

#include "ntopt.h"

#define IS_DELIM(c) \
    (((c) == '\r') || ((c) == '\n') || ((c) == '\t') || ((c) == '\0') || ((c) == ' '))

int ntopt_get_count(const char *str)
{
    int cnt = 0;
    int wc = 0;
    char *p = (char *)str;
    while (*p) {
        if (!IS_DELIM(*p)) {
            wc++;
            if (wc == 1) {
                cnt++;
            }
        } else {
            wc = 0;
        }
        p++;
    }
    return cnt;
}

char *ntopt_get_text(const char *str, const int n, char *buf, int siz)
{
    int cnt = 0;
    int wc = 0;
    char *p = (char *)str;
    while (*p) {
        if (!IS_DELIM(*p)) {
            wc++;
            if ((wc == 1)) {
                if (cnt == n) {
                    char *des = buf;
                    while (!IS_DELIM(*p)) {
                        *des = *p;
                        des++;
                        p++;
                    }
                    *des = '\0';
                    return buf;
                }
                cnt++;
            }
        } else {
            wc = 0;
        }
        p++;
    }
    return '\0';
}

int ntopt_compare(const char *s1, const char *s2)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;
    while (*p1) {
        if (*p1 != *p2) {
            return (*p1 < *p2) ? 1 : -1;
        }
        p1++;
        p2++;
    }
    if (*p1 == *p2) {
        return 0;
    } else {
        return (*p1 < *p2) ? 1 : -1;
    }
}

#if 0
int main(int argc, char **argv)
{
    char *str1 = "  This is a test.\n   ";
    char *str2 = "This is a test.\t  \r  \n  \t  It's good for you.  \n \n";

    int n1 = ntopt_get_count(str1);
    for (int i = 0; i < n1; i++) {
        char buf[BUFSIZ];
        printf("%d: %s\n", i, ntopt_get_text(str1, i, buf, sizeof(buf)));
    }
    printf("\n");

    int n2 = ntopt_get_count(str2);
    for (int i = 0; i < n2; i++) {
        char buf[BUFSIZ];
        printf("%d: %s\n", i, ntopt_get_text(str2, i, buf, sizeof(buf)));
    }
    printf("\n");

    printf("Match1&2=%d\n", ntopt_match(str1, str2));
    printf("Match1&1=%d\n", ntopt_match(str1, str1));
    printf("Match2&2=%d\n", ntopt_match(str2, str2));
    printf("Match2&1=%d\n", ntopt_match(str2, str1));

    return 0;
}
#endif

