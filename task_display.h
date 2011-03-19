#ifndef _TASK_DISPLAY_H_
#define _TASK_DISPLAY_H_

#include "target_test.h"

#define TSKPRI_DISPLAY    13

#define DISPLAY_CMD_CLEAR 0
#define DISPLAY_CMD_LINE 1
#define DISPLAY_CMD_BOX 2
#define DISPLAY_CMD_FILLBOX 3
#define DISPLAY_CMD_TEXT 4

typedef struct {
    T_MSG header;
    uint8_t cmd;
    void *param;
} display_msg_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} display_clear_t;

typedef struct {
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} display_line_t;

typedef struct {
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} display_box_t;

typedef struct {
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
    uint8_t r1;
    uint8_t g1;
    uint8_t b1;
    uint8_t r2;
    uint8_t g2;
    uint8_t b2;
} display_fillbox_t;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    char text[32];
} display_text_t;

#define DISP_CLEAR(R,G,B) \
    do { \
        VP vp; \
        display_clear_t param; \
        get_mpf(MPF_DISPLAY, &vp); \
        ((display_msg_t*)vp)->cmd = DISPLAY_CMD_CLEAR; \
        param.r = (R); \
        param.g = (G); \
        param.b = (B); \
        ((display_msg_t*)vp)->param = &param; \
        snd_mbx(MBX_DISPLAY, vp); \
    } while(0)

#define DISP_LINE(X1,Y1,X2,Y2,R,G,B) \
    do { \
        VP vp; \
        display_line_t param; \
        get_mpf(MPF_DISPLAY, &vp); \
        ((display_msg_t*)vp)->cmd = DISPLAY_CMD_LINE; \
        param.x1 = (X1); \
        param.y1 = (Y1); \
        param.x2 = (X2); \
        param.y2 = (Y2); \
        param.r = (R); \
        param.g = (G); \
        param.b = (B); \
        ((display_msg_t*)vp)->param = &param; \
        snd_mbx(MBX_DISPLAY, vp); \
    } while(0)

#define DISP_BOX(X1,Y1,X2,Y2,R,G,B) \
    do { \
        VP vp; \
        display_box_t param; \
        get_mpf(MPF_DISPLAY, &vp); \
        ((display_msg_t*)vp)->cmd = DISPLAY_CMD_BOX; \
        param.x1 = (X1); \
        param.y1 = (Y1); \
        param.x2 = (X2); \
        param.y2 = (Y2); \
        param.r = (R); \
        param.g = (G); \
        param.b = (B); \
        ((display_msg_t*)vp)->param = &param; \
        snd_mbx(MBX_DISPLAY, vp); \
    } while(0)

#define DISP_FILLBOX(X1,Y1,X2,Y2,R1,G1,B1,R2,G2,B2) \
    do { \
        VP vp; \
        display_fillbox_t param; \
        get_mpf(MPF_DISPLAY, &vp); \
        ((display_msg_t*)vp)->cmd = DISPLAY_CMD_FILLBOX; \
        param.x1 = (X1); \
        param.y1 = (Y1); \
        param.x2 = (X2); \
        param.y2 = (Y2); \
        param.r1 = (R1); \
        param.g1 = (G1); \
        param.b1 = (B1); \
        param.r2 = (R2); \
        param.g2 = (G2); \
        param.b2 = (B2); \
        ((display_msg_t*)vp)->param = &param; \
        snd_mbx(MBX_DISPLAY, vp); \
    } while(0)

#define DISP_TEXT(X,Y,R,G,B,TEXT) \
    do { \
        int i; \
        char *strp; \
        VP vp; \
        display_text_t param; \
        get_mpf(MPF_DISPLAY, &vp); \
        ((display_msg_t*)vp)->cmd = DISPLAY_CMD_TEXT; \
        param.x = (X); \
        param.y = (Y); \
        param.r = (R); \
        param.g = (G); \
        param.b = (B); \
        strp = (TEXT); \
        i = 0; \
        while (*strp) { \
            param.text[i] = *strp; \
            i++; \
            strp++; \
        } \
        param.text[i] = '\0'; \
        ((display_msg_t*)vp)->param = &param; \
        snd_mbx(MBX_DISPLAY, vp); \
    } while(0)

#ifndef TOPPERS_MACRO_ONLY

void task_display(intptr_t exinf);

#endif

#endif

