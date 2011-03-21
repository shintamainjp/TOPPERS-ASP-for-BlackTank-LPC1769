
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
#include "adc_subsystem.h"
#include "codec_subsystem.h"
#include "testpin.h"
#include "task_display.h"

void task_audio_init(intptr_t exinf)
{
    tslp_tsk(2000);
    DISP_CLEAR(0x00, 0x00, 0x00);

    /* リアルタイム・ステータス用のテストピンを出力にする */
    testpin_init();

    /* I2C を初期化する */
    i2c_init();
    DISP_TEXT(0, 10 * 0, 0xFF, 0xFF, 0xFF, "I2C init done.");
    tslp_tsk(500);

    /* AUDIO CODECを初期化する */
    codec_init();
    DISP_TEXT(0, 10 * 1, 0xFF, 0xFF, 0xFF, "Codec init done.");
    tslp_tsk(500);

    /* ペリフェラル群の初期化 */
    adc_init();
    DISP_TEXT(0, 10 * 2, 0xFF, 0xFF, 0xFF, "ADC init done.");
    tslp_tsk(500);

    i2s_init();
    DISP_TEXT(0, 10 * 3, 0xFF, 0xFF, 0xFF, "I2S init done.");
    tslp_tsk(500);

    i2s_dma_init(get_audio_data());
    DISP_TEXT(0, 10 * 4, 0xFF, 0xFF, 0xFF, "I2S DMA init done.");
    tslp_tsk(500);

    act_tsk(TASK_AUDIO);

    /* DMAによるI2S転送を始める */
    i2s_start();
    DISP_TEXT(0, 10 * 5, 0xFF, 0xFF, 0xFF, "Started I2S.");
    tslp_tsk(500);

    DISP_BMPFILE("logo.bmp");

    int cnt = 0;
    while (1) {
        tslp_tsk(200);
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
    }
}

