#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_audio.h"

void task_audio(intptr_t exinf)
{
    while (1) {
        tslp_tsk(50);
    }
}

