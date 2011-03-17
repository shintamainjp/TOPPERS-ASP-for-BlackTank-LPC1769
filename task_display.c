#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_display.h"

void task_display(intptr_t exinf)
{
    while(1)
    {
        // syslog(LOG_NOTICE, "[%s]", __func__);
        tslp_tsk(1000);
    }
}

