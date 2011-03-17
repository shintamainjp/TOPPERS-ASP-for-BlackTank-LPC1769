#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_userinput.h"

void task_userinput(intptr_t exinf)
{
    while(1)
    {
        // syslog(LOG_NOTICE, "[%s]", __func__);
        tslp_tsk(1000);
    }
}

