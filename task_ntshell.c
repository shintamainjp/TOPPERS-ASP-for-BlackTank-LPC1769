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
#include "pff.h"
#include "diskio.h"

FATFS fs;
DIR dir;
FILINFO fno;
vtparse_t parser;
text_editor_t editor;
text_history_t history;
int ledspd = 100;

void cmd_intval_up(void);
void cmd_intval_down(void);
void cmd_fs_init(void);
void cmd_fs_ls(void);
void cmd_trace(void);
void cmd_exit(void);
void cmd_help(void);

int text_puts(const char *str);
int func_read(void *buf, int cnt);
int func_write(const void *buf, int cnt);
int func_cb(const unsigned char *text);

typedef struct {
    char *command;
    char *description;
    void (*func)(void);
} command_table_t;

const command_table_t table[] = {
    {"intval up", "Increse task interval time.", cmd_intval_up},
    {"intval down", "Decrese task interval time.", cmd_intval_down},
    {"fs init", "Initialize disk and mount it.", cmd_fs_init},
    {"fs ls", "List the FAT file system contents.", cmd_fs_ls},
    {"trace", "Trace the kernel conditions.", cmd_trace},
    {"exit", "Exit the kernel.", cmd_exit},
    {"help", "Display help.", cmd_help},
    {NULL, NULL, NULL}
};

void cmd_intval_up(void) {
    if (ledspd < 500) {
        ledspd++;
        snd_dtq(DTQ_LEDSPD, (intptr_t)ledspd);
    }
}

void cmd_intval_down(void) {
    if (ledspd > 1) {
        ledspd--;
        snd_dtq(DTQ_LEDSPD, (intptr_t)ledspd);
    }
}

void cmd_fs_init(void) {
    int a = disk_initialize();
    int b = pf_mount(&fs);
    syslog(LOG_NOTICE, "a=%d, b=%d", a, b);
}

void cmd_fs_ls(void) {
    int r;
    r = pf_opendir(&dir, "");
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
    }
}

void cmd_trace(void) {
#ifdef TOPPERS_ENABLE_TRACE
    trace_sta_log(TRACE_STOP);
    trace_sta_log(TRACE_CLEAR);
    trace_sta_log(TRACE_AUTOSTOP);
    text_puts("\r\ntrace started.");
#else
    text_puts("\r\ntrace disabled in this build.");
#endif
}

void cmd_exit(void) {
    ext_ker();
}

void cmd_help(void) {
    command_table_t *p = &table[0];
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

int func_cb(const unsigned char *text)
{
    int execnt = 0;
    command_table_t *p = &table[0];
    while (p->command != NULL) {
        if (ntopt_compare((const char *)text, p->command) == 0) {
            p->func();
            execnt++;
        }
        p++;
    }
    if ((execnt == 0) && (ntopt_get_count(text) > 0)) {
        text_puts("\r\nFound unknown command. (help: display help.)");
    }

    return 0;
}

/**
 * \brief メインタスク
 * \param exinf コンフィギュレータから渡す引数。今回は利用しない
 * \details
 */
void task_ntshell(intptr_t exinf)
{
    // syslog(LOG_NOTICE, "task_ntshell: Started.");
    serial_opn_por(SIO_PORTID);

    ntshell_execute(&parser,
            &editor, &history,
            func_read, func_write, func_cb);
}

