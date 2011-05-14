/**
 * @file ntshell.c
 * @author Shinichiro Nakamura
 * @brief 小規模組み込みシステム向けのシェルシステムの実装。
 */

/*
 * ===============================================================
 *  Natural Tiny Shell (NT-Shell)
 *  Version 0.0.4
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

#include "ntshell.h"

typedef struct {
    text_editor_t *editor;
    text_history_t *history;
    int (*func_read)(void *buf, int cnt);
    int (*func_write)(const void *buf, int cnt);
    int (*func_cb)(const unsigned char *text);
} ntshell_user_data_t;

#define GET_EDITOR(vtp) \
    ((ntshell_user_data_t *)vtp->user_data)->editor
#define GET_HISTORY(vtp) \
    ((ntshell_user_data_t *)vtp->user_data)->history
#define SERIAL_READ(vtp,buf,cnt) \
    ((ntshell_user_data_t *)vtp->user_data)->func_read(buf, cnt)
#define SERIAL_WRITE(vtp,buf,cnt) \
    ((ntshell_user_data_t *)vtp->user_data)->func_write(buf, cnt)
#define CALLBACK(vtp, text) \
    ((ntshell_user_data_t *)vtp->user_data)->func_cb(text)

static void actfunc_history_prev(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_history_read_point_prev(GET_HISTORY(parser))) {
        unsigned char txt[TEXTHISTORY_MAXLEN];
        int n = text_history_read(GET_HISTORY(parser), &txt[0], sizeof(txt));
        if (0 < n) {
            SERIAL_WRITE(parser, "\x1b[2K", 4);
            SERIAL_WRITE(parser, "\x1b[80D", 5);
            SERIAL_WRITE(parser, ">", 1);
            SERIAL_WRITE(parser, txt, n);
            text_editor_set_text(GET_EDITOR(parser), txt);
        }
    }
}

static void actfunc_history_next(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_history_read_point_next(GET_HISTORY(parser))) {
        unsigned char txt[TEXTHISTORY_MAXLEN];
        int n = text_history_read(GET_HISTORY(parser), &txt[0], sizeof(txt));
        if (0 < n) {
            SERIAL_WRITE(parser, "\x1b[2K", 4);
            SERIAL_WRITE(parser, "\x1b[80D", 5);
            SERIAL_WRITE(parser, ">", 1);
            SERIAL_WRITE(parser, txt, n);
            text_editor_set_text(GET_EDITOR(parser), txt);
        }
    }
}

static void actfunc_cursor_left(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_editor_cursor_left(GET_EDITOR(parser))) {
        SERIAL_WRITE(parser, "\x1b[1D", 4);
    }
}

static void actfunc_cursor_right(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_editor_cursor_right(GET_EDITOR(parser))) {
        SERIAL_WRITE(parser, "\x1b[1C", 4);
    }
}

static void actfunc_enter(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    unsigned char txt[TEXTEDITOR_MAXLEN];
    text_editor_get_text(GET_EDITOR(parser), &txt[0], sizeof(txt));
    text_editor_clear(GET_EDITOR(parser));
    text_history_write(GET_HISTORY(parser), txt);
    CALLBACK(parser, txt);
    SERIAL_WRITE(parser, "\r\n>", 3);
}

static void actfunc_cancel(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    SERIAL_WRITE(parser, "^C\r\n", 4);
    text_editor_clear(GET_EDITOR(parser));
    SERIAL_WRITE(parser, ">", 1);
}

static void actfunc_insert(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_editor_insert(GET_EDITOR(parser), ch)) {
        unsigned char txt[TEXTEDITOR_MAXLEN];
        int len = text_editor_get_text(GET_EDITOR(parser), &txt[0], sizeof(txt));
        int pos = text_editor_cursor_get_position(GET_EDITOR(parser));
        SERIAL_WRITE(parser, &ch, sizeof(ch));
        int n = len - pos;
        if (n > 0) {
            int i;
            SERIAL_WRITE(parser, txt + pos, len - pos);
            for (i = 0; i < n; i++) {
                SERIAL_WRITE(parser, "\x1b[1D", 4);
            }
        }
    }
}

static void actfunc_backspace(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_editor_backspace(GET_EDITOR(parser))) {
        unsigned char txt[TEXTEDITOR_MAXLEN];
        SERIAL_WRITE(parser, "\x1b[1D", 4);
        int len = text_editor_get_text(GET_EDITOR(parser), &txt[0], sizeof(txt));
        int pos = text_editor_cursor_get_position(GET_EDITOR(parser));
        int n = len - pos;
        if (n > 0) {
            int i;
            SERIAL_WRITE(parser, txt + pos, len - pos);
            SERIAL_WRITE(parser, " ", 1);
            for (i = 0; i < n + 1; i++) {
                SERIAL_WRITE(parser, "\x1b[1D", 4);
            }
        } else {
            SERIAL_WRITE(parser, " ", 1);
            SERIAL_WRITE(parser, "\x1b[1D", 4);
        }
    }
}

// @todo 後で名前を変更する。
static void actfunc_hokan(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    // @todo 実装を追加する。
}

static void actfunc_text_head(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    // @todo 実装を追加する。
}

static void actfunc_text_tail(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    // @todo 実装を追加する。
}

typedef struct {
    vtparse_action_t action;
    unsigned char ch;
    void (*func)(
            vtparse_t *parser,
            vtparse_action_t action,
            unsigned char ch);
} ntshell_action_table_t;

static const ntshell_action_table_t action_table[] = {
    {VTPARSE_ACTION_PRINT, 0x7f, actfunc_backspace},
    {VTPARSE_ACTION_EXECUTE, 0x0d, actfunc_enter},
    {VTPARSE_ACTION_EXECUTE, 0x08, actfunc_backspace},
    {VTPARSE_ACTION_EXECUTE, 0x03, actfunc_cancel},
    {VTPARSE_ACTION_EXECUTE, 0x02, actfunc_cursor_left},
    {VTPARSE_ACTION_EXECUTE, 0x06, actfunc_cursor_right},
    {VTPARSE_ACTION_EXECUTE, 0x10, actfunc_history_prev},
    {VTPARSE_ACTION_EXECUTE, 0x0e, actfunc_history_next},
    {VTPARSE_ACTION_EXECUTE, 0x09, actfunc_hokan},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x44, actfunc_cursor_left},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x43, actfunc_cursor_right},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x41, actfunc_history_prev},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x42, actfunc_history_next}
};

void parser_callback(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    ntshell_action_table_t *p;
    int i;
    const int ACTTBLSIZ = sizeof(action_table) / sizeof(action_table[0]);

    p = (ntshell_action_table_t *)action_table;
    for (i = 0; i < ACTTBLSIZ; i++) {
        if ((p->action == action) && (p->ch == ch)) {
            p->func(parser, action, ch);
            return;
        }
        p++;
    }

    if ((VTPARSE_ACTION_PRINT == action) && (ch != 0x7f)) {
        actfunc_insert(parser, action, ch);
    }
}

void ntshell_version(int *major, int *minor, int *build)
{
    *major = 0;
    *minor = 0;
    *build = 1;
}

void ntshell_execute(
        vtparse_t *parser,
        text_editor_t *editor,
        text_history_t *history,
        int (*func_read)(void *buf, int cnt),
        int (*func_write)(const void *buf, int cnt),
        int (*func_cb)(const unsigned char *text))
{
    ntshell_user_data_t user_data;

    user_data.editor = editor;
    user_data.history = history;
    user_data.func_read = func_read;
    user_data.func_write = func_write;
    user_data.func_cb = func_cb;

    parser->user_data = &user_data;

    vtparse_init(parser, parser_callback);
    text_editor_init(GET_EDITOR(parser));
    text_history_init(GET_HISTORY(parser));

    SERIAL_WRITE(parser, ">", 1);
    while(1)
    {
        unsigned char c;
        SERIAL_READ(parser, &c, sizeof(c));
        vtparse(parser, &c, sizeof(c));
    }
}

