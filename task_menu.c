
#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <itron.h>

#include "kernel_cfg.h"
#include "task_menu.h"
#include "task_led.h"
#include "task_display.h"

#define MSG_DEVICE(n) (((n) & 0xE0) >> 5)
#define MSG_VALUE(n) ((n) & 0x1F)

typedef enum {
    PAGE_SPLASH,
    PAGE_TOP,
    PAGE_100,
    PAGE_200,
    PAGE_300,
    PAGE_400
} PAGEID;

typedef struct {
    PAGEID curr_page;
    PAGEID next_page[4];
    void (*func)(void);
} menu_t;

static PAGEID page = PAGE_SPLASH;
static uint32_t life = 0;

#define DISP_MENUTAG(A,B,C,D) \
    do { \
        DISP_TEXT(2 + (24 * 0), 52, 0xFF, 0xFF, 0xFF, (A)); \
        DISP_TEXT(2 + (24 * 1), 52, 0xFF, 0xFF, 0xFF, (B)); \
        DISP_TEXT(2 + (24 * 2), 52, 0xFF, 0xFF, 0xFF, (C)); \
        DISP_TEXT(2 + (24 * 3), 52, 0xFF, 0xFF, 0xFF, (D)); \
    } while(0)

void page_splash(void)
{
    /* 他のタスクが初期化状態を表示するため
     * このタスクでは何も表示しない。
     */
}

void page_top(void)
{
    if (life == 0) {
        DISP_CLEAR(0x00, 0x00, 0x00);
        DISP_MENUTAG("SEL1", "SEL2", "SEL3", "SEL4");
    }
}

void page_100(void)
{
    if (life == 0) {
        DISP_CLEAR(0xFF, 0x00, 0x00);
        DISP_MENUTAG("110", "120", "130", "140");
    }
}

void page_200(void)
{
    if (life == 0) {
        DISP_CLEAR(0x00, 0xFF, 0x00);
        DISP_MENUTAG("210", "220", "230", "240");
    }
}

void page_300(void)
{
    if (life == 0) {
        DISP_CLEAR(0x00, 0x00, 0xFF);
        DISP_MENUTAG("310", "320", "330", "340");
    }
}

void page_400(void)
{
    if (life == 0) {
        DISP_CLEAR(0x00, 0x00, 0x00);
        DISP_MENUTAG("410", "420", "430", "440");
    }
}

static const menu_t menu[6] = {
    {PAGE_SPLASH, {PAGE_TOP, PAGE_TOP, PAGE_TOP, PAGE_TOP}, page_splash},
    {PAGE_TOP, {PAGE_100, PAGE_200, PAGE_300, PAGE_400}, page_top},
    {PAGE_100, {PAGE_100, PAGE_200, PAGE_300, PAGE_400}, page_100},
    {PAGE_200, {PAGE_100, PAGE_200, PAGE_300, PAGE_400}, page_200},
    {PAGE_300, {PAGE_100, PAGE_200, PAGE_300, PAGE_400}, page_300},
    {PAGE_400, {PAGE_100, PAGE_200, PAGE_300, PAGE_400}, page_400}
};

void execute_pagefunc(PAGEID page)
{
    int i;
    for (i = 0; i < sizeof(menu) / sizeof(menu[0]); i++) {
        if (menu[i].curr_page == page) {
            if (menu[i].func != NULL) {
                menu[i].func();
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

void task_menu(intptr_t exinf)
{
    uint8_t msg;
    while(1)
    {
        execute_pagefunc(page);
        life++;
        while (prcv_dtq(DTQ_USERINPUT, (intptr_t *)&msg) == E_OK) {
            if ((SW0 <= MSG_DEVICE(msg)) && (MSG_DEVICE(msg) <= SW3)) {
                page = get_next_page(page, MSG_DEVICE(msg));
                life = 0;
            }
        }
        tslp_tsk(100);
    }
}

