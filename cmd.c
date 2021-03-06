/**
 * @file cmd.c
 * @author Shinichiro Nakamura
 * @brief ナチュラルタイニーシェルタスクコマンドの実装
 * @details
 */

#include <kernel.h>
#include <t_syslog.h>
#include <target_syssvc.h>
#include <syssvc/serial.h>
#include <logtrace/trace_config.h>

#include "kernel_cfg.h"
#include "ntopt.h"
#include "ntlibc.h"
#include "ff.h"

#include "task_audio.h"
#include "task_led.h"
#include "task_display.h"

char curdir[32];
FATFS fatfs[_VOLUMES];
DIR dir;
FIL fil;
FILINFO filinfo;

void cmd_taskinfo(int argc, char **argv) {
    int i;
    T_ITSK itsk;
    syslog(LOG_NOTICE, "TSKID\tSTACK ADDR (HEAD:TAIL)\tSTACK USED (USED/TOTAL)");
    syslog(LOG_NOTICE, "=========================================================");
    for (i = 0; i < TNUM_TSKID; i++) {
        const int tskid = 1 + i;
        inf_tsk(tskid, &itsk);
        syslog(LOG_NOTICE, " %2d\t0x%x:0x%x\t%5d/%5d",
                tskid,
                itsk.stk_head, itsk.stk_tail,
                itsk.stk_used, itsk.stk_total);
#if 0
        syslog(LOG_NOTICE, " (%02d:%02d)",
                itsk.tsk_pri_curr, itsk.tsk_pri_base);
#endif
    }
    syslog(LOG_NOTICE, "=========================================================");
}

void cmd_audio(int argc, char **argv) {
    if (argc == 2) {
        if (ntlibc_strcmp(argv[1], "through") == 0) {
            TSKAPI_AUDIO_PARAM(AUDIO_PARAM_MODE, AUDIO_VALUE_MODE_THROUGH);
            return;
        }
        if (ntlibc_strcmp(argv[1], "vocal_cancel") == 0) {
            TSKAPI_AUDIO_PARAM(AUDIO_PARAM_MODE, AUDIO_VALUE_MODE_VOCAL_CANCEL);
            return;
        }
        if (ntlibc_strcmp(argv[1], "fir") == 0) {
            TSKAPI_AUDIO_PARAM(AUDIO_PARAM_MODE, AUDIO_VALUE_MODE_FIR);
            return;
        }
        if (ntlibc_strcmp(argv[1], "iir") == 0) {
            TSKAPI_AUDIO_PARAM(AUDIO_PARAM_MODE, AUDIO_VALUE_MODE_IIR);
            return;
        }
    }
    syslog(LOG_NOTICE, "audio [through | vocal_cancel | fir | iir]");
}

void cmd_lcd(int argc, char **argv) {
    if (argc == 2) {
        if (ntlibc_strcmp(argv[1], "cls") == 0) {
            TSKAPI_DISPLAY_CLEAR(0x00, 0x00, 0x00);
            return;
        }
    }
    syslog(LOG_NOTICE, "lcd cls : Clear the LCD.");
}

void cmd_led(int argc, char **argv) {
    if (argc != 3) {
        syslog(LOG_NOTICE, "led [DBLEDn|SWLEDn] [ON|OFF]");
        return;
    }
    if (ntlibc_strcmp(argv[1], "DBLED0") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(DBLED0, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(DBLED0, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "DBLED1") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(DBLED1, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(DBLED1, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "DBLED2") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(DBLED2, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(DBLED2, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "DBLED3") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(DBLED3, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(DBLED3, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED0") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(SWLED0, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(SWLED0, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED1") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(SWLED1, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(SWLED1, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED2") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(SWLED2, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(SWLED2, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED3") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            TSKAPI_LED_LEDMSG(SWLED3, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            TSKAPI_LED_LEDMSG(SWLED3, LEDOFF);
            return;
        }
    }
    syslog(LOG_NOTICE, "Invalid command.");
}

void cmd_mount(int argc, char **argv) {
    int a = f_mount(0, &fatfs[0]);
    int b = f_opendir(&dir, "");
    ntlibc_strcpy(curdir, "");
    if ((a == 0) && (b == 0)) {
        syslog(LOG_NOTICE, "Mounted the SD card.");
    } else {
        syslog(LOG_NOTICE, "Failure to mount the SD card.");
    }
}

void cmd_ls(int argc, char **argv) {
    int r;
    r = f_opendir(&dir, curdir);
    if (r) {
        syslog(LOG_NOTICE, "Failure to open the directory. (code=%d)", r);
        return;
    }
    while (1) {
        r = f_readdir(&dir, &filinfo);
        if (r != FR_OK) {
            syslog(LOG_NOTICE, "Failure to read the directory. (code=%d)", r);
            return;
        }
        if (!filinfo.fname[0]) {
            break;
        }
        if (filinfo.fattrib & AM_DIR) {
            syslog(LOG_NOTICE, "<DIR>\t%s", filinfo.fname);
        } else {
            syslog(LOG_NOTICE, "%d\t%s", filinfo.fsize, filinfo.fname);
        }
        tslp_tsk(10);
    }
}

void cmd_cd(int argc, char **argv) {
    int r;
    switch (argc) {
        case 1:
            r = f_opendir(&dir, "");
            if (!r) {
                ntlibc_strcpy(curdir, "");
            }
            break;
        case 2:
            r = f_opendir(&dir, argv[1]);
            if (!r) {
                ntlibc_strcpy(curdir, argv[1]);
            }
            break;
        default:
            syslog(LOG_NOTICE, "cd [dir]");
            return;
    }
    if (r) {
        syslog(LOG_NOTICE, "Failure to open the director. (code=%d)", r);
        return;
    }
}

void cmd_trace(int argc, char **argv) {
#ifdef TOPPERS_ENABLE_TRACE
    syslog(LOG_NOTICE, "trace started.");
    tslp_tsk(250);
    trace_sta_log(TRACE_STOP);
    trace_sta_log(TRACE_CLEAR);
    trace_sta_log(TRACE_AUTOSTOP);
#else
    syslog(LOG_NOTICE, "trace disabled in this build.");
#endif
}

void cmd_exit(int argc, char **argv) {
    ext_ker();
}

