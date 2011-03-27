/**
 * @file task_ntshell.c
 * @author Shinichiro Nakamura
 * @brief ナチュラルタイニーシェルタスクの実装(task_ntshell)
 * @details
 * システムをコンソールから制御することのできるインターフェースタスク。
 * 主に開発者のために提供される。
 */

#include <kernel.h>
#include <t_syslog.h>
#include <target_syssvc.h>
#include <syssvc/serial.h>
#include <logtrace/trace_config.h>

#include "kernel_cfg.h"

#include "task_ntshell.h"
#include "task_led.h"
#include "task_display.h"

#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"
#include "diskio.h"
#include "ff.h"

char curdir[32];
FATFS fatfs[_VOLUMES];
DIR dir;
FIL fil;
FILINFO filinfo;
vtparse_t parser;
text_editor_t editor;
text_history_t history;
int ledspd = 100;

void cmd_lcd(int argc, char **argv);
void cmd_led(int argc, char **argv);
void cmd_mount(int argc, char **argv);
void cmd_ls(int argc, char **argv);
void cmd_cd(int argc, char **argv);
void cmd_trace(int argc, char **argv);
void cmd_exit(int argc, char **argv);
void cmd_help(int argc, char **argv);

int text_puts(const char *str);
int func_read(void *buf, int cnt);
int func_write(const void *buf, int cnt);
void func_ntopt(int argc, char **argv);
int func_ntshell(const unsigned char *text);

typedef struct {
    char *command;
    char *description;
    void (*func)(int argc, char **argv);
} command_table_t;

const command_table_t table[] = {
    {"lcd", "Control the LCD.", cmd_lcd},
    {"led", "Set state of the debug purpose LED.", cmd_led},
    {"mount", "Mount a SD card.", cmd_mount},
    {"ls", "List contents on a SD card.", cmd_ls},
    {"cd", "Change the current directory.", cmd_cd},
    {"trace", "Trace the kernel conditions.", cmd_trace},
    {"exit", "Exit the kernel.", cmd_exit},
    {"help", "Display help.", cmd_help},
    {NULL, NULL, NULL}
};

void cmd_lcd(int argc, char **argv) {
    if (argc == 2) {
        if (ntlibc_strcmp(argv[1], "cls") == 0) {
            DISP_CLEAR(0x00, 0x00, 0x00);
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
            LEDMSG(DBLED0, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(DBLED0, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "DBLED1") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            LEDMSG(DBLED1, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(DBLED1, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "DBLED2") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            LEDMSG(DBLED2, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(DBLED2, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "DBLED3") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            LEDMSG(DBLED3, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(DBLED3, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED0") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            LEDMSG(SWLED0, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(SWLED0, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED1") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            LEDMSG(SWLED1, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(SWLED1, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED2") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            LEDMSG(SWLED2, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(SWLED2, LEDOFF);
            return;
        }
    }
    if (ntlibc_strcmp(argv[1], "SWLED3") == 0) {
        if (ntlibc_strcmp(argv[2], "ON") == 0) {
            LEDMSG(SWLED3, LEDON);
            return;
        }
        if (ntlibc_strcmp(argv[2], "OFF") == 0) {
            LEDMSG(SWLED3, LEDOFF);
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
    trace_sta_log(TRACE_STOP);
    trace_sta_log(TRACE_CLEAR);
    trace_sta_log(TRACE_AUTOSTOP);
    text_puts("\r\ntrace started.");
#else
    text_puts("\r\ntrace disabled in this build.");
#endif
}

void cmd_exit(int argc, char **argv) {
    ext_ker();
}

void cmd_help(int argc, char **argv) {
    const command_table_t *p = &table[0];
    while (p->command != NULL) {
        text_puts("\r\n");
        text_puts(p->command);
        text_puts("\t:");
        text_puts(p->description);
        p++;
    }
}

int text_puts(const char *str)
{
    int cnt = 0;
    char *p = (char *)str;
    while (*p) {
        cnt++;
        p++;
    }
    return serial_wri_dat(SIO_PORTID, (const char_t *)str, cnt);
}

int func_read(void *buf, int cnt)
{
    return serial_rea_dat(SIO_PORTID, buf, cnt);
}

int func_write(const void *buf, int cnt)
{
    return serial_wri_dat(SIO_PORTID, buf, cnt);
}

void func_ntopt(int argc, char **argv)
{
    if (argc == 0) {
        return;
    }

    int execnt = 0;
    const command_table_t *p = &table[0];
    while (p->command != NULL) {
        if (ntlibc_strcmp((const char *)argv[0], p->command) == 0) {
            p->func(argc, argv);
            execnt++;
        }
        p++;
    }
    if (execnt == 0) {
        text_puts("\r\nFound unknown command. (help: display help.)");
    }
}

int func_ntshell(const unsigned char *text)
{
    return ntopt_parse((const char *)text, func_ntopt);
}

/**
 * \brief メインタスク
 * \param exinf コンフィギュレータから渡す引数。今回は利用しない
 * \details
 */
void task_ntshell(intptr_t exinf)
{
    serial_opn_por(SIO_PORTID);

    ntshell_execute(&parser,
            &editor, &history,
            func_read, func_write, func_ntshell);
}

