/**
 * @file task_display.c
 * @author Shinichiro Nakamura
 * @brief ディスプレイタスクの実装(task_display)
 * @details
 * ディスプレイタスクは有機ELディスプレイを制御するタスクである。
 * ディスプレイの制御は以下のマクロを使って行うことができる。
 *
 * DISP_CLEAR(r, g, b);
 * DISP_LINE(x1, y1, x2, y2, r, g, b);
 * DISP_BOX(x1, y1, x2, y2, r, g, b);
 * DISP_FILLBOX(x1, y1, x2, y2, r1, g1, b1, r2, g2, b2);
 * DISP_TEXT(x, y, r, g, b, "text");
 * DISP_BMPFILE("filename");
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <itron.h>

#include "kernel_cfg.h"
#include "task_display.h"
#include "oled.h"
#include "ff.h"
#include "bmplowio.h"

static const uint8_t FONT_X = 5;
static const uint8_t FONT_Y = 7;
static const uint16_t FONT_MIN_CODE = 0x20;
static const uint16_t FONT_MAX_CODE = 0x7F;
static const uint8_t font5x7_data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // (white space)
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F
    0x3E, 0x41, 0x41, 0x51, 0x32, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x7F, 0x20, 0x18, 0x20, 0x7F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // /
    0x41, 0x41, 0x7F, 0x00, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x14, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x08, 0x2A, 0x1C, 0x08, // ->
    0x08, 0x1C, 0x2A, 0x08, 0x08  // <-
};

display_bmpfile_t param_bmpfile;

void disp_clear(const uint8_t r, const uint8_t g, const uint8_t b)
{
    VP vp;
    get_mpf(MPF_DISPLAY, &vp);
    ((display_msg_t*)vp)->cmd = DISPLAY_CMD_CLEAR;
    display_clear_t *param = (display_clear_t *)((display_msg_t*)vp)->param;
    param->r = r;
    param->g = g;
    param->b = b;
    snd_mbx(MBX_DISPLAY, vp);
}

void disp_line(
        const uint8_t x1,
        const uint8_t y1,
        const uint8_t x2,
        const uint8_t y2,
        const uint8_t r,
        const uint8_t g,
        const uint8_t b)
{
    VP vp;
    get_mpf(MPF_DISPLAY, &vp);
    ((display_msg_t*)vp)->cmd = DISPLAY_CMD_LINE;
    display_line_t *param =
        (display_line_t *)((display_msg_t*)vp)->param;
    param->x1 = x1;
    param->y1 = y1;
    param->x2 = x2;
    param->y2 = y2;
    param->r = r;
    param->g = g;
    param->b = b;
    snd_mbx(MBX_DISPLAY, vp);
}

void disp_box(
        const uint8_t x1,
        const uint8_t y1,
        const uint8_t x2,
        const uint8_t y2,
        const uint8_t r,
        const uint8_t g,
        const uint8_t b)
{
    VP vp;
    get_mpf(MPF_DISPLAY, &vp);
    ((display_msg_t*)vp)->cmd = DISPLAY_CMD_BOX;
    display_box_t *param =
        (display_box_t *)((display_msg_t*)vp)->param;
    param->x1 = x1;
    param->y1 = y1;
    param->x2 = x2;
    param->y2 = y2;
    param->r = r;
    param->g = g;
    param->b = b;
    snd_mbx(MBX_DISPLAY, vp);
}

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
        const uint8_t b2)
{
    VP vp;
    get_mpf(MPF_DISPLAY, &vp);
    ((display_msg_t*)vp)->cmd = DISPLAY_CMD_FILLBOX;
    display_fillbox_t *param =
        (display_fillbox_t *)((display_msg_t*)vp)->param;
    param->x1 = x1;
    param->y1 = y1;
    param->x2 = x2;
    param->y2 = y2;
    param->r1 = r1;
    param->g1 = g1;
    param->b1 = b1;
    param->r2 = r2;
    param->g2 = g2;
    param->b2 = b2;
    snd_mbx(MBX_DISPLAY, vp);
}

void disp_text(
        const uint8_t x,
        const uint8_t y,
        const uint8_t r,
        const uint8_t g,
        const uint8_t b,
        const char *text)
{
    int i;
    const char *strp;
    VP vp;
    get_mpf(MPF_DISPLAY, &vp);
    ((display_msg_t*)vp)->cmd = DISPLAY_CMD_TEXT;
    display_text_t *param =
        (display_text_t *)((display_msg_t*)vp)->param;
    param->x = x;
    param->y = y;
    param->r = r;
    param->g = g;
    param->b = b;
    strp = text;
    i = 0;
    while (*strp) {
        param->text[i] = *strp;
        i++;
        strp++;
    }
    param->text[i] = '\0';
    snd_mbx(MBX_DISPLAY, vp);
}

void disp_bmpfile(const char *filename)
{
    int i;
    const char *strp;
    VP vp;
    get_mpf(MPF_DISPLAY, &vp);
    ((display_msg_t*)vp)->cmd = DISPLAY_CMD_BMPFILE;
    strp = filename;
    i = 0;
    while (*strp) {
        param_bmpfile.filename[i] = *strp;
        i++;
        strp++;
    }
    param_bmpfile.filename[i] = '\0';
    ((display_msg_t*)vp)->param = &param_bmpfile;
    snd_mbx(MBX_DISPLAY, vp);
}

void disp_audio_levelmeter(const int left, const int right)
{
    VP vp;
    get_mpf(MPF_DISPLAY, &vp);
    ((display_msg_t*)vp)->cmd = DISPLAY_CMD_AUDIO_LEVELMETER;
    display_audio_levelmeter_t *param =
        (display_audio_levelmeter_t *)((display_msg_t*)vp)->param;
    param->left = left;
    param->right = right;
    snd_mbx(MBX_DISPLAY, vp);
}

void cmd_clear(display_clear_t *p)
{
    Color c;
    c.r = p->r;
    c.g = p->g;
    c.b = p->b;

    oled_fill_box(0, 0, OLED_X - 1, OLED_Y - 1, c, c);
}

void cmd_line(display_line_t *p)
{
    Color c;
    c.r = p->r;
    c.g = p->g;
    c.b = p->b;

    oled_draw_line(p->x1, p->y1, p->x2, p->y2, c);
}

void cmd_box(display_box_t *p)
{
    Color c;
    c.r = p->r;
    c.g = p->g;
    c.b = p->b;

    oled_draw_line(p->x1, p->y1, p->x2, p->y1, c);
    oled_draw_line(p->x2, p->y1, p->x2, p->y2, c);
    oled_draw_line(p->x2, p->y2, p->x1, p->y2, c);
    oled_draw_line(p->x1, p->y2, p->x1, p->y1, c);
}

void cmd_fillbox(display_fillbox_t *p)
{
    Color c1, c2;
    c1.r = p->r1;
    c1.g = p->g1;
    c1.b = p->b1;
    c2.r = p->r2;
    c2.g = p->g2;
    c2.b = p->b2;

    oled_fill_box(p->x1, p->y1, p->x2, p->y2, c1, c2);
}

void draw_char(uint8_t x, uint8_t y, char c, Color *col) {
    int i, j;
    if ((FONT_MIN_CODE <= c) &&(c <= FONT_MAX_CODE)) {
        int aofs = (c - FONT_MIN_CODE) * FONT_X;
        for (i = 0; i < FONT_X; i++) {
            uint8_t pat = font5x7_data[aofs + i];
            for (j = 0; j < FONT_Y; j++) {
                if (pat & (1 << j)) {
                    oled_draw_pixel(x + i, y + j, *col);
                }
            }
        }
    } else {
        for (i = 0; i < FONT_X; i++) {
            uint8_t pat = (i % 2) ? 0x55 : 0xAA;
            for (j = 0; j < FONT_Y; j++) {
                if (pat & (1 << j)) {
                    oled_draw_pixel(x + i, y + j, *col);
                }
            }
        }
    }
}

void cmd_text(display_text_t *p)
{
    Color col;
    col.r = p->r;
    col.g = p->g;
    col.b = p->b;

    char *strp = p->text;
    int cnt = 0;
    while (*strp) {
        draw_char(p->x + (FONT_X * cnt), p->y, *strp, &col);
        strp++;
        cnt++;
    }
}

FATFS fatfs[_VOLUMES];
DIR dir;
FIL fil;
FILINFO filinfo;

int ff_getc(void)
{
    uint8_t c;
    UINT n;
    FRESULT fr = f_read(&fil, &c, 1, &n);
    return ((fr == FR_OK) ? c : -1);
}

void dispfunc(int x, int y, int r, int g, int b)
{
    Color c;
    if ((0 <= x) && (0 <= y) && (x < OLED_X) && (y < OLED_Y)) {
        c.r = r;
        c.g = g;
        c.b = b;
        oled_draw_pixel(x, y, c);
    }
}

void cmd_bmpfile(display_bmpfile_t *p)
{
    bmp_file_t bmpfile;
    bmp_info_t bmpinfo;
    bmp_rgbquad_t bmprgbquad;

    int a = f_mount(0, &fatfs[0]);
    int b = f_opendir(&dir, "");
    if ((a == 0) && (b == 0)) {
        FRESULT res = f_open(&fil, p->filename, FA_OPEN_EXISTING|FA_READ);
        if (res == FR_OK) {
            bmplowio_header_read(ff_getc, &bmpfile, &bmpinfo);
            if (have_palette(&bmpinfo)) {
                bmplowio_palette_read(
                        ff_getc,
                        &bmprgbquad,
                        (1 << bmpinfo.biBitCount));
            }
            bmplowio_image_read(
                    ff_getc,
                    &bmpfile,
                    &bmpinfo,
                    dispfunc);
        } else {
            syslog(LOG_NOTICE, "file open failed.(%d)", res);
        }
    } else {
        syslog(LOG_NOTICE, "mount failed.");
    }
}

void cmd_audio_levelmeter(display_audio_levelmeter_t *p)
{
    Color c1, c2;
    c1.r = 0xFF;
    c1.g = 0xFF;
    c1.b = 0xFF;
    c2.r = 0x00;
    c2.g = 0x00;
    c2.b = 0x00;

    static const int LVW = OLED_X;
    static const int LVH = 2;
    static const int LVOFSX = 0;
    static const int LVOFSY = 10;

    unsigned int PL = ((((uint32_t)p->left ) ^ 0x80000000) >> 24) - 128;
    unsigned int PR = ((((uint32_t)p->right) ^ 0x80000000) >> 24) - 128;
    PL = PL / 5;
    PR = PR / 5;
    if (PL < 0) { PL = 0; }
    if (PR < 0) { PR = 0; }
    if (LVW <= PL) { PL = LVW - 1; }
    if (LVW <= PR) { PR = LVW - 1; }

    /*
     * Left side.
     */
    oled_fill_box(
            LVOFSX + 0, LVOFSY + 0,
            LVOFSX + PL - 1, LVOFSY + LVH - 1,
            c1, c1);
    oled_fill_box(
            LVOFSX + PL, LVOFSY + 0,
            LVOFSX + OLED_X - 1, LVOFSY + LVH - 1,
            c2, c2);

    /*
     * Right side.
     */
    oled_fill_box(
            LVOFSX + 0, LVOFSY + LVH,
            LVOFSX + PR - 1, LVOFSY + LVH + LVH - 1,
            c1, c1);
    oled_fill_box(
            LVOFSX + PR, LVOFSY + LVH,
            LVOFSX + OLED_X - 1, LVOFSY + LVH + LVH - 1,
            c2, c2);
}

void task_display(intptr_t exinf)
{
    display_msg_t *p;

    /*
     * OLEDを初期化する。
     */
    oled_init();
    oled_clear(0, 0, OLED_X - 1, OLED_Y - 1);

    while(1)
    {
        /*
         * メールボックスの受信を待つ。
         * 待つまでタスクは受信待ちとなる。
         */
        if (rcv_mbx(MBX_DISPLAY, (T_MSG**)&p) == E_OK) {
            /*
             * コマンドによってパラメータの型が決まる。
             * コマンドによって処理関数が決まる。
             */
            uint8_t cmd = ((display_msg_t*)p)->cmd;
            void* param = ((display_msg_t*)p)->param;
            switch (cmd) {
                case DISPLAY_CMD_CLEAR:
                    cmd_clear((display_clear_t *)param);
                    break;
                case DISPLAY_CMD_LINE:
                    cmd_line((display_line_t *)param);
                    break;
                case DISPLAY_CMD_BOX:
                    cmd_box((display_box_t *)param);
                    break;
                case DISPLAY_CMD_FILLBOX:
                    cmd_fillbox((display_fillbox_t *)param);
                    break;
                case DISPLAY_CMD_TEXT:
                    cmd_text((display_text_t *)param);
                    break;
                case DISPLAY_CMD_BMPFILE:
                    cmd_bmpfile((display_bmpfile_t *)param);
                    break;
                case DISPLAY_CMD_AUDIO_LEVELMETER:
                    cmd_audio_levelmeter((display_audio_levelmeter_t *)param);
                    break;
                default:
                    syslog(LOG_NOTICE, "Unknown command number %d.", cmd);
                    break;
            }
            rel_mpf(MPF_DISPLAY, (VP)p);
        }
    }
}

