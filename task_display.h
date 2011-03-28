/**
 * @file task_display.h
 * @author Shinichiro Nakamura
 * @brief ディスプレイタスクの定義(task_display)
 * @details
 * ディスプレイタスクは有機ELディスプレイを制御するタスクである。
 * ディスプレイの制御は以下のマクロを使って行うことができる。
 *
 * @code
 * DISP_CLEAR(r, g, b);
 * DISP_LINE(x1, y1, x2, y2, r, g, b);
 * DISP_BOX(x1, y1, x2, y2, r, g, b);
 * DISP_FILLBOX(x1, y1, x2, y2, r1, g1, b1, r2, g2, b2);
 * DISP_TEXT(x, y, r, g, b, "text");
 * DISP_BMPFILE("filename");
 * @endcode
 */

#ifndef _TASK_DISPLAY_H_
#define _TASK_DISPLAY_H_

#include "target_test.h"

#define TSKPRI_DISPLAY    11
#define TSKSTK_DISPLAY    STACK_SIZE

#define DISPLAY_CMD_CLEAR 0
#define DISPLAY_CMD_LINE 1
#define DISPLAY_CMD_BOX 2
#define DISPLAY_CMD_FILLBOX 3
#define DISPLAY_CMD_TEXT 4
#define DISPLAY_CMD_BMPFILE 5
#define DISPLAY_CMD_AUDIO_LEVELMETER 6

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

typedef struct {
    char filename[32];
} display_bmpfile_t;

typedef struct {
    int left;
    int right;
} display_audio_levelmeter_t;

void disp_clear(const uint8_t r, const uint8_t g, const uint8_t b);
void disp_line(
        const uint8_t x1,
        const uint8_t y1,
        const uint8_t x2,
        const uint8_t y2,
        const uint8_t r,
        const uint8_t g,
        const uint8_t b);
void disp_box(
        const uint8_t x1,
        const uint8_t y1,
        const uint8_t x2,
        const uint8_t y2,
        const uint8_t r,
        const uint8_t g,
        const uint8_t b);
void disp_fillbox(
        const uint8_t x1,
        const uint8_t y1,
        const uint8_t x2,
        const uint8_t y2,
        const uint8_t r1,
        const uint8_t g1,
        const uint8_t b1,
        const uint8_t r2,
        const uint8_t g2,
        const uint8_t b2);
void disp_text(
        const uint8_t x,
        const uint8_t y,
        const uint8_t r,
        const uint8_t g,
        const uint8_t b,
        const char *text);
void disp_bmpfile(const char *filename);
void disp_audio_levelmeter(const int left, const int right);

#define DISP_CLEAR(R,G,B) \
    disp_clear((R),(G),(B))
#define DISP_LINE(X1,Y1,X2,Y2,R,G,B) \
    disp_line((X1),(Y1),(X2),(Y2),(R),(G),(B))
#define DISP_BOX(X1,Y1,X2,Y2,R,G,B) \
    disp_box((X1),(Y1),(X2),(Y2),(R),(G),(B))
#define DISP_FILLBOX(X1,Y1,X2,Y2,R1,G1,B1,R2,G2,B2) \
    disp_fillbox((X1),(Y1),(X2),(Y2),(R1),(G1),(B1),(R2),(G2),(B2))
#define DISP_TEXT(X,Y,R,G,B,TEXT) \
    disp_text((X),(Y),(R),(G),(B),(TEXT))
#define DISP_BMPFILE(FN) \
    disp_bmpfile((FN))
#define DISP_AUDIO_LEVELMETER(L,R) \
    disp_audio_levelmeter((L),(R))

#ifndef TOPPERS_MACRO_ONLY

/**
 * @defgroup BLACKTANK_LPC1769_TASK_DISPLAY
 * @ingroup BLACKTANK_LPC1769_TASK
 * @{
 */

void task_display(intptr_t exinf);

/**
 * @}
 */

#endif

#endif

