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
#include "task_audio.h"

#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"
#include "cmd.h"

ntshell_t ntshell;

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
    {"taskinfo", "Task information.", cmd_taskinfo},
    {"audio", "Audio task parameter.", cmd_audio},
    {"lcd", "Control the LCD.", cmd_lcd},
    {"led", "Set state of the debug purpose LED.", cmd_led},
    {"mount", "Mount a SD card.", cmd_mount},
    {"ls", "List contents on a SD card.", cmd_ls},
    {"cd", "Change the current directory.", cmd_cd},
    {"trace", "Trace the kernel conditions.", cmd_trace},
    {"exit", "Exit the kernel.", cmd_exit},
    {NULL, NULL, NULL}
};

void cmd_help(void) {
    const command_table_t *p = &table[0];
    char buf[64];
    while (p->command != NULL) {
        ntlibc_strcpy(buf, p->command);
        ntlibc_strcat(buf, "\t:");
        ntlibc_strcat(buf, p->description);
        syslog(LOG_NOTICE, buf);
        tslp_tsk(10);
        p++;
    }
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
            syslog(LOG_NOTICE, "");
            p->func(argc, argv);
            execnt++;
        }
        p++;
    }
    if (execnt == 0) {
        if (argc > 0) {
            syslog(LOG_NOTICE, "");
            cmd_help();
        } else {
            syslog(LOG_NOTICE,
                    "Found unknown command. (help: display help.)");
        }
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

    ntshell_execute(&ntshell, func_read, func_write, func_ntshell);
}

