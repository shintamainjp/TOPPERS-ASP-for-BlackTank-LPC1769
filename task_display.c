#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_display.h"
#include "oled.h"

void task_display(intptr_t exinf)
{
    oled_init();
    oled_clear(0, 0, 95, 63);

    while(1)
    {
#if 0
        Color c1, c2;
        int x, y;
        const int BS = 8;
        for (x = 0; x < 96 / BS; x++) {
            for (y = 0; y < 64 / BS; y++) {
                c1.r = 0x60;
                c1.g = 0x60;
                c1.b = 0x60;
                c2.r = x * BS;
                c2.g = y * BS;
                c2.b = x * 2 + y * 2;
                oled_fill_box(
                        (x * BS), (y * BS),
                        (x * BS) + BS, (y * BS) + BS,
                        c1, c2);
            }
        }
#else
        Color c1;
        int x;
        for (x = 0; x < 96; x++) {
            c1.r = x;
            c1.g = x;
            c1.b = x;
            oled_draw_line(x, 0, x, 64, c1);
        }
#endif
        tslp_tsk(1000);

        oled_clear(0, 0, 95, 63);
        tslp_tsk(1000);
    }
}

