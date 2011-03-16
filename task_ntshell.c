/**
 * \file task_ntshell.c
 * \brief アプリケーションの本体ファイル。
*/

#include <kernel.h>
#include <t_syslog.h>
#include <target_syssvc.h>
#include <syssvc/serial.h>
#include <logtrace/trace_config.h>

#include "kernel_cfg.h"
#include "task_ntshell.h"
#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"
#include "pff.h"
#include "diskio.h"
#include "oled.h"

char curdir[32];
FATFS fs;
DIR dir;
FILINFO fno;
vtparse_t parser;
text_editor_t editor;
text_history_t history;
int ledspd = 100;

void cmd_intval(int argc, char **argv);
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
    {"intval", "Set interval time.", cmd_intval},
    {"mount", "Mount a SD card.", cmd_mount},
    {"ls", "List contents on a SD card.", cmd_ls},
    {"cd", "Change the current directory.", cmd_cd},
    {"trace", "Trace the kernel conditions.", cmd_trace},
    {"exit", "Exit the kernel.", cmd_exit},
    {"help", "Display help.", cmd_help},
    {NULL, NULL, NULL}
};

void cmd_intval(int argc, char **argv) {
    switch (argc) {
        case 1:
            syslog(LOG_NOTICE, "%d", ledspd);
            break;
        case 2:
            if (ntopt_compare(argv[1], "up") == 0) {
                if (ledspd < 500) {
                    ledspd++;
                    snd_dtq(DTQ_LEDSPD, (intptr_t)ledspd);
                }
                syslog(LOG_NOTICE, "%d", ledspd);
            } else if (ntopt_compare(argv[1], "down") == 0) {
                if (ledspd > 1) {
                    ledspd--;
                    snd_dtq(DTQ_LEDSPD, (intptr_t)ledspd);
                }
                syslog(LOG_NOTICE, "%d", ledspd);
            } else {
                syslog(LOG_NOTICE, "Unknown sub command.");
            }
            break;
    }
}

void cmd_mount(int argc, char **argv) {
    int a = disk_initialize();
    int b = pf_mount(&fs);
    int c = pf_opendir(&dir, "");
    strcpy(curdir, "");
    if ((a == 0) && (b == 0) && (c == 0)) {
        syslog(LOG_NOTICE, "Mounted the SD card.");
    } else {
        syslog(LOG_NOTICE, "Failure to mount the SD card.");
    }
}

void cmd_ls(int argc, char **argv) {
    int r;
    r = pf_opendir(&dir, curdir);
    if (r) {
        syslog(LOG_NOTICE, "Failure to open the director. (code=%d)", r);
        return;
    }
    while (1) {
        r = pf_readdir(&dir, &fno);
        if (r != FR_OK) {
            syslog(LOG_NOTICE, "Failure to read the director. (code=%d)", r);
            return;
        }
        if (!fno.fname[0]) {
            break;
        }
        if (fno.fattrib & AM_DIR) {
            syslog(LOG_NOTICE, "<DIR>\t%s", fno.fname);
        } else {
            syslog(LOG_NOTICE, "%d\t%s", fno.fsize, fno.fname);
        }
        tslp_tsk(10);
    }
}

void cmd_cd(int argc, char **argv) {
    int r;
    switch (argc) {
        case 1:
            r = pf_opendir(&dir, "");
            if (!r) {
                strcpy(curdir, "");
            }
            break;
        case 2:
            r = pf_opendir(&dir, argv[1]);
            if (!r) {
                strcpy(curdir, argv[1]);
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
        if (ntopt_compare((const char *)argv[0], p->command) == 0) {
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
    Color c1, c2;

    c1.r = 0x60;
    c1.g = 0x60;
    c1.b = 0x60;

    oled_init();
    oled_clear(0, 0, 95, 63);

    c2.r = 0x10; c2.g = 0x10; c2.b = 0x10;
    oled_fill_box(0, 0, 20, 20, c1, c2);

    c2.r = 0x20; c2.g = 0x20; c2.b = 0x20;
    oled_fill_box(20, 20, 40, 40, c1, c2);

    c2.r = 0x30; c2.g = 0x30; c2.b = 0x30;
    oled_fill_box(40, 40, 60, 60, c1, c2);

    ntshell_execute(&parser,
            &editor, &history,
            func_read, func_write, func_ntshell);
}

