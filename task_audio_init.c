
#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>

#include "task_audio.h"
#include "task_audio_init.h"
#include "kernel_cfg.h"
#include "audio_common.h"
#include "audio_effect.h"
#include "i2s_subsystem.h"
#include "i2c_subsystem.h"
#include "codec_subsystem.h"
#include "testpin.h"
#include "task_display.h"

void task_audio_init(intptr_t exinf)
{
    DISP_CLEAR(0x00, 0x00, 0x00);

    i2c_init();
    DISP_TEXT(0, 10 * 0, 0xFF, 0xFF, 0xFF, "I2C init done.");
    tslp_tsk(200);

    codec_init();
    DISP_TEXT(0, 10 * 1, 0xFF, 0xFF, 0xFF, "Codec init done.");
    tslp_tsk(200);

    i2s_init();
    DISP_TEXT(0, 10 * 2, 0xFF, 0xFF, 0xFF, "I2S init done.");
    tslp_tsk(200);

    i2s_dma_init(get_audio_data());
    DISP_TEXT(0, 10 * 3, 0xFF, 0xFF, 0xFF, "I2S DMA init done.");
    tslp_tsk(200);

    /*
     * メニュータスクとユーザインプットタスクを起動する。
     */
    act_tsk(TASK_MENU);
    act_tsk(TASK_USERINPUT);

    /*
     * メニューが表示された頃合いを見てオーディオ処理を開始する。
     */
    tslp_tsk(500);

    /*
     * オーディオタスクを起動し、DMAによるI2S転送を始める。
     */
    act_tsk(TASK_AUDIO);
    i2s_start();

    /*
     * @TODO
     * i2s_start()を呼ぶとDMA割り込みの無限ループに陥る。
     * 現象が発生するのはコールドスタートの時だけ。
     * 現在調査中。
     */

    int cnt = 0;
    while (1) {
        /*
         * 動作を示すマーキング.
         */
        DISP_FILLBOX(90, 0, 95, 6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        switch (cnt % 4) {
            case 0:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "-");
                break;
            case 1:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "\\");
                break;
            case 2:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "|");
                break;
            case 3:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "/");
                break;
        }
        cnt++;
        tslp_tsk(200);
    }
}

