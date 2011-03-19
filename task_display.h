#ifndef _TASK_DISPLAY_H_
#define _TASK_DISPLAY_H_

#include "target_test.h"

#define TSKPRI_DISPLAY    13

#define DISPLAY_CMD_CLEAR 0

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

#define DISP_CLEAR(x, y, z) \
    do { \
        VP vp; \
        display_clear_t param; \
        get_mpf(MPF_DISPLAY, &vp); \
        ((display_msg_t*)vp)->cmd = DISPLAY_CMD_CLEAR; \
        param.r = (x); \
        param.g = (y); \
        param.b = (z); \
        ((display_msg_t*)vp)->param = &param; \
        snd_mbx(MBX_DISPLAY, vp); \
    } while(0)

#ifndef TOPPERS_MACRO_ONLY

void task_display(intptr_t exinf);

#endif

#endif

