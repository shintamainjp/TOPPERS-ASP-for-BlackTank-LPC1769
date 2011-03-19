#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <itron.h>

#include "kernel_cfg.h"
#include "task_display.h"
#include "oled.h"

void cmd_clear(display_clear_t *p)
{
    syslog(LOG_NOTICE, "CLEAR (%d, %d, %d)", p->r, p->g, p->b);
    Color c;
    c.r = p->r;
    c.g = p->g;
    c.b = p->b;
    oled_fill_box(0, 0, 95, 63, c, c);
}

void task_display(intptr_t exinf)
{
    display_msg_t *p;

    oled_init();
    oled_clear(0, 0, 95, 63);

    while(1)
    {
        if (rcv_mbx(MBX_DISPLAY, (T_MSG**)&p) == E_OK) {
            uint8_t cmd = ((display_msg_t*)p)->cmd;
            void*param = ((display_msg_t*)p)->param;
            switch (cmd) {
                case DISPLAY_CMD_CLEAR:
                    cmd_clear((display_clear_t *)param);
                    break;
            }
            rel_mpf(MPF_DISPLAY, (VP)p);
        }
    }
}

