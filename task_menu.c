/**
 * @file task_menu.c
 * @author Shinichiro Nakamura
 * @brief メニュータスクの実装(task_menu)
 * @details
 * メニュータスクはユーザにシステムのサービスを提示し、
 * ユーザからの要求をシステムに伝達する役目を果たす。
 * ユーザからの入力はそれらを管理するタスクからの通知に依存する。
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_menu.h"
#include "task_led.h"
#include "task_display.h"
#include "task_audio.h"
#include "oled.h"
#include "config.h"

#define MSG_DEVICE(n) (((n) & 0xF000) >> 12)
#define MSG_VALUE(n)  (((n) & 0x0FFF) >>  0)

/*
 * メニュータスクの雛形。
 * 簡単に階層化ができる。
 */

typedef enum {
    PAGE_TOP,
    PAGE_100,
    PAGE_200,
    PAGE_300,
    PAGE_VER
} PAGEID;

typedef enum {
    PAGE_IN,
    PAGE_TICK,
    PAGE_OUT
} ACTION;

typedef struct {
    PAGEID curr_page;
    PAGEID next_page[4];
    void (*func)(ACTION act);
} menu_t;

static PAGEID curr_page = PAGE_TOP;

typedef struct {
    int val[4];
} menu_work_t;

menu_work_t menu_work;

#define DISP_MENUNAME(NAME) \
    do { \
        TSKAPI_DISPLAY_FILLBOX(0, 0, OLED_X - 1, 8, \
                0x00, 0x00, 0x00, \
                0x00, 0x00, 0x00); \
        TSKAPI_DISPLAY_TEXT(0, 0, 0xFF, 0xFF, 0xFF, (NAME)); \
    } while(0)

#define DISP_MENUTAG(A,B,C,D) \
    do { \
        TSKAPI_DISPLAY_FILLBOX(0, 52, OLED_X - 1, OLED_Y - 1, \
                0x00, 0x00, 0x00, \
                0x00, 0x00, 0x00); \
        TSKAPI_DISPLAY_TEXT(2 + (24 * 0), 52, 0xFF, 0xFF, 0xFF, (A)); \
        TSKAPI_DISPLAY_TEXT(2 + (24 * 1), 52, 0xFF, 0xFF, 0xFF, (B)); \
        TSKAPI_DISPLAY_TEXT(2 + (24 * 2), 52, 0xFF, 0xFF, 0xFF, (C)); \
        TSKAPI_DISPLAY_TEXT(2 + (24 * 3), 52, 0xFF, 0xFF, 0xFF, (D)); \
    } while(0)

void page_splash(ACTION act)
{
    /* 他のタスクが初期化状態を表示するため
     * このタスクでは何も表示しない。
     */
}

void page_top(ACTION act)
{
    if (act == PAGE_IN) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
        DISP_MENUNAME("[TOP]");
        DISP_MENUTAG("M-1", "M-2", "M-3", "VER");
    }
    if (act == PAGE_TICK) {
        int i;
        for (i = 0; i < 4; i++) {
            static const int MAXVAL = (1024/ 32);
            static const int XOFS = 10;
            static const int YOFS = 15;
            int val = MAXVAL - (menu_work.val[i] / 32);
            TSKAPI_DISPLAY_FILLBOX(
                    XOFS + i * 20, YOFS + 0,
                    XOFS + i * 20 + 10, YOFS + val,
                    0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00);
            TSKAPI_DISPLAY_FILLBOX(
                    XOFS + i * 20, YOFS + val,
                    XOFS + i * 20 + 10, YOFS + MAXVAL,
                    0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF);
        }
        if (!TASK_AUDIO_ENABLED()) {
            TSKAPI_DISPLAY_TEXT(10, 8, 0xFF, 0xFF, 0xFF, "AUDIO DISABLED");
        }
    }
    if (act == PAGE_OUT) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
    }
}

void page_100(ACTION act)
{
    if (act == PAGE_IN) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
        DISP_MENUNAME("[MENU-1]");
        TSKAPI_DISPLAY_TEXT(20, 30, 0xFF, 0xFF, 0xFF, "-- MENU 1 --");
        DISP_MENUTAG("", "", "", "RET");
    }
    if (act == PAGE_OUT) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
    }
}

void page_200(ACTION act)
{
    if (act == PAGE_IN) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
        DISP_MENUNAME("[MENU-2]");
        TSKAPI_DISPLAY_TEXT(20, 30, 0xFF, 0xFF, 0xFF, "-- MENU 2 --");
        DISP_MENUTAG("", "", "", "RET");
    }
    if (act == PAGE_OUT) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
    }
}

void page_300(ACTION act)
{
    if (act == PAGE_IN) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
        DISP_MENUNAME("[MENU-3]");
        TSKAPI_DISPLAY_TEXT(20, 30, 0xFF, 0xFF, 0xFF, "-- MENU 3 --");
        DISP_MENUTAG("", "", "", "RET");
    }
    if (act == PAGE_OUT) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
    }
}

void page_ver(ACTION act)
{
    if (act == PAGE_IN) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
        DISP_MENUNAME("[Version]");
        TSKAPI_DISPLAY_TEXT(20, 30, 0xFF, 0xFF, 0xFF, "Preliminary");
        DISP_MENUTAG("", "", "", "RET");
    }
    if (act == PAGE_OUT) {
        TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
    }
}

static const menu_t menu[6] = {
    {PAGE_TOP, {PAGE_100, PAGE_200, PAGE_300, PAGE_VER}, page_top},
    {PAGE_100, {PAGE_100, PAGE_100, PAGE_100, PAGE_TOP}, page_100},
    {PAGE_200, {PAGE_200, PAGE_200, PAGE_200, PAGE_TOP}, page_200},
    {PAGE_300, {PAGE_300, PAGE_300, PAGE_300, PAGE_TOP}, page_300},
    {PAGE_VER, {PAGE_VER, PAGE_VER, PAGE_VER, PAGE_TOP}, page_ver}
};

void execute_pagefunc(PAGEID page, ACTION act)
{
    int i;
    for (i = 0; i < sizeof(menu) / sizeof(menu[0]); i++) {
        if (menu[i].curr_page == page) {
            if (menu[i].func != NULL) {
                menu[i].func(act);
                return;
            }
        }
    }
}

PAGEID get_next_page(PAGEID page, int sw)
{
    int i;
    for (i = 0; i < sizeof(menu) / sizeof(menu[0]); i++) {
        if (menu[i].curr_page == page) {
            return menu[i].next_page[sw];
        }
    }
    return page;
}

void splash(void)
{
    static const int XOFS = 5;
    static const int YOFS = 10;
    static const int YSTP = 12;

    TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
    TSKAPI_DISPLAY_BMPFILE("0:LOGO.BMP");
    tslp_tsk(2000);

    TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
    TSKAPI_DISPLAY_TEXT(XOFS + 0, YOFS + YSTP * 0,
            0xFF, 0xFF, 0xFF, "Special thanks to");
    TSKAPI_DISPLAY_TEXT(XOFS + 22, YOFS + YSTP * 2,
            0xFF, 0xFF, 0xFF, "Horie-san");
    TSKAPI_DISPLAY_TEXT(XOFS + 20, YOFS + YSTP * 3,
            0xFF, 0xFF, 0xFF, "Kaneko-san");
    tslp_tsk(2000);
}

void task_menu(intptr_t exinf)
{
    uint16_t msg;

    splash();

    execute_pagefunc(curr_page, PAGE_IN);
    while(1)
    {
        /*
         * ページにティックを与える。
         */
        execute_pagefunc(curr_page, PAGE_TICK);

        /*
         * データキューの受信状態を見る。
         * ここではポーリングして、データがなければスルーする。
         */
        while (prcv_dtq(DTQ_USERINPUT, (intptr_t *)&msg) == E_OK) {
            if ((DEVICE_SW0 <= MSG_DEVICE(msg))
                    && (MSG_DEVICE(msg) <= DEVICE_SW3)) {
                /*
                 * スイッチがONエッジならばページの遷移処理を実行。
                 */
                if (MSG_VALUE(msg)) {
                    PAGEID next_page = get_next_page(
                            curr_page, MSG_DEVICE(msg));
                    if (next_page != curr_page) {
                        execute_pagefunc(curr_page, PAGE_OUT);
                        curr_page = next_page;
                        execute_pagefunc(curr_page, PAGE_IN);
                    }
                }
                /*
                 * 押されたスイッチのLEDを点灯させる。
                 */
                switch (MSG_DEVICE(msg)) {
                    case DEVICE_SW0:
                        TSKAPI_LED_LEDMSG(SWLED0, MSG_VALUE(msg));
                        break;
                    case DEVICE_SW1:
                        TSKAPI_LED_LEDMSG(SWLED1, MSG_VALUE(msg));
                        break;
                    case DEVICE_SW2:
                        TSKAPI_LED_LEDMSG(SWLED2, MSG_VALUE(msg));
                        break;
                    case DEVICE_SW3:
                        TSKAPI_LED_LEDMSG(SWLED3, MSG_VALUE(msg));
                        break;
                }
            }
            if ((DEVICE_VOL0 <= MSG_DEVICE(msg))
                    && (MSG_DEVICE(msg) <= DEVICE_VOL3)) {
                /*
                 * 作業用変数に与えられたレベルを格納しておく。
                 */
                int ch = MSG_DEVICE(msg) - DEVICE_VOL0;
                menu_work.val[ch] = MSG_VALUE(msg);
                /*
                 * オーディオパラメータをオーディオタスクに伝達する。
                 */
                if (TASK_AUDIO_ENABLED()) {
                    switch (MSG_DEVICE(msg)) {
                        case DEVICE_VOL0:
                            TSKAPI_AUDIO_PARAM(
                                    AUDIO_PARAM_VAR0, MSG_VALUE(msg));
                            break;
                        case DEVICE_VOL1:
                            TSKAPI_AUDIO_PARAM(
                                    AUDIO_PARAM_VAR1, MSG_VALUE(msg));
                            break;
                        case DEVICE_VOL2:
                            TSKAPI_AUDIO_PARAM(
                                    AUDIO_PARAM_VAR2, MSG_VALUE(msg));
                            break;
                        case DEVICE_VOL3:
                            TSKAPI_AUDIO_PARAM(
                                    AUDIO_PARAM_VAR3, MSG_VALUE(msg));
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        tslp_tsk(100);
    }
}

