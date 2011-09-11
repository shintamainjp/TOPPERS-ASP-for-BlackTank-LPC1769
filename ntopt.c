/**
 * @file ntopt.c
 * @author Shinichiro Nakamura
 * @brief NT-Shell用オプション解析モジュールの実装。
 */

/*
 * ===============================================================
 *  Natural Tiny Shell (NT-Shell)
 *  Version 0.0.7
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

static int ntopt_get_count(const char *str);
static char *ntopt_get_text(const char *str, const int n, char *buf, int siz);

static int ntopt_get_count(const char *str)
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

static char *ntopt_get_text(const char *str, const int n, char *buf, int siz)
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
                    int cc = 0;
                    while (!IS_DELIM(*p)) {
                        cc++;
                        if (siz <= cc) {
                            break;
                        }
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

int ntopt_parse(
        const char *str,
        void *extobj,
        int (*func)(int argc, char **argv, void *extobj))
{
    int argc;
    char argv[NTOPT_MAXCNT_ARGC][NTOPT_MAXLEN_ARGV];
    char *argvp[NTOPT_MAXCNT_ARGC];
    int i;

    argc = ntopt_get_count(str);
    if (NTOPT_MAXCNT_ARGC <= argc) {
        argc = NTOPT_MAXCNT_ARGC;
    }

    for (i = 0; i < argc; i++) {
        argvp[i] = ntopt_get_text(str, i, argv[i], sizeof(argv[i]));
    }

    return func(argc, &argvp[0], extobj);
}

