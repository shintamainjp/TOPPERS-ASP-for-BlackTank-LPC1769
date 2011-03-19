#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <itron.h>

#include "kernel_cfg.h"
#include "task_display.h"
#include "oled.h"

void cmd_clear(display_clear_t *p)
{
    syslog(LOG_NOTICE, "CLEAR Color:[%d,%d,%d]", p->r, p->g, p->b);

    Color c;
    c.r = p->r;
    c.g = p->g;
    c.b = p->b;

    oled_fill_box(0, 0, OLED_X - 1, OLED_Y - 1, c, c);
}

void cmd_line(display_line_t *p)
{
    syslog(LOG_NOTICE, "LINE (%d,%d)-(%d,%d), Color:[%d,%d,%d]",
            p->x1, p->y1, p->x2, p->y2,
            p->r, p->g, p->b);

    Color c;
    c.r = p->r;
    c.g = p->g;
    c.b = p->b;

    oled_draw_line(p->x1, p->y1, p->x2, p->y2, c);
}

void cmd_box(display_box_t *p)
{
    syslog(LOG_NOTICE, "BOX (%d,%d)-(%d,%d), Color:[%d,%d,%d]",
            p->x1, p->y1, p->x2, p->y2,
            p->r, p->g, p->b);

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
    syslog(LOG_NOTICE, "FILLBOX (%d,%d)-(%d,%d), Color:[%d,%d,%d],[%d,%d,%d]",
            p->x1, p->y1, p->x2, p->y2,
            p->r1, p->g1, p->b1,
            p->r2, p->g2, p->b2);

    Color c1, c2;
    c1.r = p->r1;
    c1.g = p->g1;
    c1.b = p->b1;
    c2.r = p->r2;
    c2.g = p->g2;
    c2.b = p->b2;

    oled_fill_box(p->x1, p->y1, p->x2, p->y2, c1, c2);
}

void task_display(intptr_t exinf)
{
    display_msg_t *p;

    oled_init();
    oled_clear(0, 0, OLED_X - 1, OLED_Y - 1);

    while(1)
    {
        if (rcv_mbx(MBX_DISPLAY, (T_MSG**)&p) == E_OK) {
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
                default:
                    syslog(LOG_NOTICE, "Unknown command number %d.", cmd);
                    break;
            }
            rel_mpf(MPF_DISPLAY, (VP)p);
        }
    }
}

