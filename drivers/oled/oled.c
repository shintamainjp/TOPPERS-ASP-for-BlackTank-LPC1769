/**
 * =============================================================================
 * Aitendo OLED device driver library. (Version 0.0.2)
 *
 * [The product]
 * OLED-2P-095BWNN-SPI : http://www.aitendo.co.jp/product/2099
 * ALO-095BWNN-J9      : http://www.aitendo.co.jp/product/1449
 *
 * [References]
 * http://serdisplib.sourceforge.net/ser/doc/Treiber_IC-SSD1332_OLED_96x64_COLOR.pdf
 * =============================================================================
 * Copyright (c) 2010 Shinichiro Nakamura (CuBeatSystems)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * =============================================================================
 */
#include "oled.h"

#include <lpc17xx_ssp.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg.h>
#include <kernel.h>

#define CS_PORT_NUM 1
#define CS_PIN_NUM 28

#define DC_PORT_NUM 0
#define DC_PIN_NUM 21

#define RES_PORT_NUM 0
#define RES_PIN_NUM 22

#define RES_ENA() GPIO_ClearValue(RES_PORT_NUM, (1 << RES_PIN_NUM))
#define RES_DIS() GPIO_SetValue(RES_PORT_NUM, (1 << RES_PIN_NUM))

#define CS_ENA() GPIO_ClearValue(CS_PORT_NUM, (1 << CS_PIN_NUM))
#define CS_DIS() GPIO_SetValue(CS_PORT_NUM, (1 << CS_PIN_NUM))

#define DC_CMD() GPIO_ClearValue(DC_PORT_NUM, (1 << DC_PIN_NUM))
#define DC_DAT() GPIO_SetValue(DC_PORT_NUM, (1 << DC_PIN_NUM))

SSP_CFG_Type SSP_ConfigStruct;
PINSEL_CFG_Type PinCfg;
SSP_DATA_SETUP_Type xferConfig;

static void oled_spi_tx(unsigned char c);
static unsigned char oled_spi_rx(void);

#define SPI_WRITE(c) oled_spi_tx(c)
#define SPI_READ() oled_spi_rx()

#define WAIT_MS(n) tslp_tsk(n)

#define swap(a,b) {int c=a;a=b;b=c;}

static void oled_spi_tx(unsigned char c)
{
    SSP_SendData(LPC_SSP0, c);
    while (SSP_GetStatus(LPC_SSP0, SSP_SR_BSY) == SET)
    {
    }
    SSP_ReceiveData(LPC_SSP0);
}

static unsigned char oled_spi_rx(void)
{
    SSP_SendData(LPC_SSP0, 0xff);
    while (SSP_GetStatus(LPC_SSP0, SSP_SR_BSY) == SET)
    {
    }
    return SSP_ReceiveData(LPC_SSP0);
}

/**
 * Reset.
 */
static void oled_reset();

/**
 * Write data.
 *
 * @param data Data.
 */
static void oled_write_data(unsigned char data);

/**
 * Set mode.
 *
 * @param fillrect Fill the rectangle.
 * @param revcopy Reverse copy.
 */
static void oled_set_mode(unsigned char fillrect, unsigned char revcopy);

/**
 * Set column address.
 *
 * @param start Start address. (0-95)
 * @param end End address. (0-95)
 */
static void oled_set_column_address(int start, int end);

/**
 * Set row address.
 *
 * @param start Start address. (0-63)
 * @param end End address. (0-63)
 */
static void oled_set_row_address(int start, int end);

/**
 * Set contrast for Color A.
 *
 * @param contrast Contrast. (Default:0x80)
 */
static void oled_set_contrast_for_color_a(int contrast);

/**
 * Set contrast for Color B.
 *
 * @param contrast Contrast. (Default:0x80)
 */
static void oled_set_contrast_for_color_b(int contrast);

/**
 * Set contrast for Color C.
 *
 * @param contrast Contrast. (Default:0x80)
 */
static void oled_set_contrast_for_color_c(int contrast);

/**
 * Set master current control value.
 *
 * @param current Current value. (0x00-0x0F)
 */
static void oled_set_master_current_control(int current);

/**
 * Set remapping mode and the data format.
 *
 * @param mode See the document.
 */
static void oled_set_remap_and_data_format(int mode);

/**
 * Set display start line.
 *
 * @param line Start line number. (0-63)
 */
static void oled_set_display_start_line(int line);

/**
 * Set display offset line.
 *
 * @param offset Offset line number. (0-63)
 */
static void oled_set_display_offset(int offset);

typedef enum {
    NormalDisplay,
    EntireDisplayOn,
    EntireDisplayOff,
    InverseDisplay
} DisplayMode;

/**
 * Set display mode.
 *
 * @param mode Display mode.
 */
static void oled_set_display_mode(DisplayMode mode);

/**
 * Set multiplex ratio.
 *
 * @param ratio Ratio.
 */
static void oled_set_multiplex_ratio(int ratio);

/**
 * Set display on/off.
 *
 * @param on On.
 */
static void oled_set_display_on_off(unsigned char on);

typedef struct gray_scale_table {
    char data[32];
} gray_scale_table_t;

/**
 * Set gray scale table.
 *
 * @param p A pointer to the look up table.
 */
static void oled_set_gray_scale_table(gray_scale_table_t *p);

/**
 * NOP.
 */
static void oled_nop();

/**
 * Set power saving mode.
 *
 * @param value Value. (0x00:None, 0x12:power saving)
 */
static void oled_set_power_saving_mode(int value);

/**
 * Set phase period.
 *
 * @param value Value. (Default:0x74)
 */
static void oled_set_phase_period(int value);

/**
 * Set display clock divide ratio.
 *
 * @param value Value. (Default:0x00)
 */
static void oled_set_display_clock_divide_ratio(int value);

/**
 * Set second pre-charge speed for color A.
 *
 * @param value Value.
 */
static void oled_set_second_pre_charge_speed_for_color_a(int value);

/**
 * Set second pre-charge speed for color B.
 *
 * @param value Value.
 */
static void oled_set_second_pre_charge_speed_for_color_b(int value);

/**
 * Set second pre-charge speed for color C.
 *
 * @param value Value.
 */
static void oled_set_second_pre_charge_speed_for_color_c(int value);

/**
 * Set pre charge level for color A.
 *
 * @param value The value.
 */
static void oled_set_pre_charge_level_for_color_a(int value);

/**
 * Set pre charge level for color B.
 *
 * @param value The value.
 */
static void oled_set_pre_charge_level_for_color_b(int value);

/**
 * Set pre charge level for color C.
 *
 * @param value The value.
 */
static void oled_set_pre_charge_level_for_color_c(int value);

/**
 * Set VCOMH.
 *
 * @param value VCOMH value. (0x00:0.43 * Vref, 0x3F:0x83 * Vref)
 */
static void oled_set_vcomh(int value);

/**
 * Read the status register.
 *
 * @return the value.
 */
static unsigned char oled_read_status_register();

void oled_init(void)
{
    /*
     * Initialize SPI pin connect
     * P1.20 - SCLK;
     * P1.23 - MISO
     * P1.24 - MOSI
     */
    PinCfg.Funcnum = 3;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 20;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 23;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 24;
    PINSEL_ConfigPin(&PinCfg);

    // initialize SSP configuration structure to default
    SSP_ConfigStructInit(&SSP_ConfigStruct);
    SSP_ConfigStruct.ClockRate = 1000;
    // Initialize SSP peripheral with parameter given in structure above
    SSP_Init(LPC_SSP0, &SSP_ConfigStruct);
    // Enable SSP peripheral
    SSP_Cmd(LPC_SSP0, ENABLE);

    // CS(Chip Select)
    GPIO_SetDir(CS_PORT_NUM, (1 << CS_PIN_NUM), 1);
    GPIO_SetValue(CS_PORT_NUM, (1 << CS_PIN_NUM));

    // DC(Data/Command)
    GPIO_SetDir(DC_PORT_NUM, (1 << DC_PIN_NUM), 1);
    GPIO_SetValue(DC_PORT_NUM, (1 << DC_PIN_NUM));

    // RES(Reset)
    GPIO_SetDir(RES_PORT_NUM, (1 << RES_PIN_NUM), 1);
    GPIO_SetValue(RES_PORT_NUM, (1 << RES_PIN_NUM));

    oled_reset();
}

/**
 * Draw pixel.
 *
 * @param x X.
 * @param y Y.
 * @param c Color.
 */
void oled_draw_pixel(int x, int y, Color c) {
    oled_set_column_address(x, x);
    oled_set_row_address(y, y);
    c.b = c.b >> 3;
    c.g = c.g >> 2;
    c.r = c.r >> 3;
    oled_write_data((((c.b << 3) & 0xF8) | ((c.g >> 3) & 0x07)));
    oled_write_data((((c.g << 5) & 0xE0) | ((c.r >> 0) & 0x1F)));
}

/**
 * Draw line.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 * @param c Color.
 */
void oled_draw_line(int x1, int y1, int x2, int y2, Color c) {
    /*
     * Bresenham's line algorithm
     */
    unsigned char steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) {
        swap(x1, y1);
        swap(x2, y2);
    }
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    int deltax = x2 - x1;
    int deltay = abs(y2 - y1);
    int error = deltax / 2;
    int ystep;
    int y = y1;
    if (y1 < y2) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    int x;
    for (x = x1; x <= x2; x++) {
        if (steep) {
            oled_draw_pixel(y, x, c);
        } else {
            oled_draw_pixel(x, y, c);
        }
        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
}

/**
 * Fill box.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 * @param c1 Color1.
 * @param c2 Color2.
 */
void oled_fill_box(int x1, int y1, int x2, int y2, Color c1, Color c2) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x22);
    SPI_WRITE(x1);
    SPI_WRITE(y1);
    SPI_WRITE(x2);
    SPI_WRITE(y2);
    SPI_WRITE(c1.b >> 3);
    SPI_WRITE(c1.g >> 2);
    SPI_WRITE(c1.r >> 3);
    SPI_WRITE(c2.b >> 3);
    SPI_WRITE(c2.g >> 2);
    SPI_WRITE(c2.r >> 3);
    CS_DIS();
    WAIT_MS(1);
}

/**
 * Copy.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 * @param nx X of the destination.
 * @param ny Y of the destination.
 */
void oled_copy(int x1, int y1, int x2, int y2, int nx, int ny) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x23);
    SPI_WRITE(x1);
    SPI_WRITE(y1);
    SPI_WRITE(x2);
    SPI_WRITE(y2);
    SPI_WRITE(nx);
    SPI_WRITE(ny);
    CS_DIS();
    WAIT_MS(1);
}

/**
 * Darker.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 */
void oled_darker(int x1, int y1, int x2, int y2) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x24);
    SPI_WRITE(x1);
    SPI_WRITE(y1);
    SPI_WRITE(x2);
    SPI_WRITE(y2);
    CS_DIS();
    WAIT_MS(1);
}

/**
 * Clear.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 */
void oled_clear(int x1, int y1, int x2, int y2) {
#if 1
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x25);
    SPI_WRITE(x1);
    SPI_WRITE(y1);
    SPI_WRITE(x2);
    SPI_WRITE(y2);
    CS_DIS();
    WAIT_MS(1);
#else
    oled_Color c;
    c.r = 0x00;
    c.g = 0x00;
    c.b = 0x00;
    for (int x = x1; x <= x2; x++) {
        for (int y = y1; y <= y2; y++) {
            oled_draw_pixel(x, y, c);
        }
    }
#endif
}

/**
 * Reset.
 */
void oled_reset() {

    RES_ENA();
    WAIT_MS(200);
    RES_DIS();
    WAIT_MS(200);

    oled_set_display_on_off(0);
    oled_set_row_address(0, 63);
    oled_set_column_address(0, 95);
    oled_set_remap_and_data_format(0x70);
    oled_set_display_start_line(0);
    oled_set_display_offset(0);
    oled_set_display_mode(NormalDisplay);
    oled_set_multiplex_ratio(0x3f);
    oled_set_master_current_control(0x8f);
    oled_set_power_saving_mode(0x00);
    oled_set_phase_period(0x74);
    oled_set_display_clock_divide_ratio(0xD0);
    oled_set_second_pre_charge_speed_for_color_a(0x80);
    oled_set_second_pre_charge_speed_for_color_b(0x80);
    oled_set_second_pre_charge_speed_for_color_c(0x80);
    oled_set_pre_charge_level_for_color_a(0x3F);
    oled_set_pre_charge_level_for_color_b(0x3F);
    oled_set_pre_charge_level_for_color_c(0x3F);
    oled_set_vcomh(0x3E);
    oled_set_master_current_control(0x0F);
    // A:Blue, B:Green, C:Red
    oled_set_contrast_for_color_a(0x80);
    oled_set_contrast_for_color_b(0x80);
    oled_set_contrast_for_color_c(0x80);
    oled_set_mode(1, 0);
    oled_set_display_on_off(1);
}

/**
 * Write data.
 *
 * @param data Data.
 */
void oled_write_data(unsigned char data) {
    DC_DAT();
    CS_ENA();
    SPI_WRITE(data);
    CS_DIS();
}

/**
 * Set mode.
 *
 * @param fillrect Fill the rectangle.
 * @param revcopy Reverse copy.
 */
void oled_set_mode(unsigned char fillrect, unsigned char revcopy) {
    unsigned char val = 0x00;
    if (fillrect) {
        val |= 1 << 0;
    }
    if (revcopy) {
        val |= 1 << 4;
    }
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x26);
    SPI_WRITE(val);
    CS_DIS();
}

/**
 * Set column address.
 *
 * @param start Start address. (0-95)
 * @param end End address. (0-95)
 */
void oled_set_column_address(int start, int end) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x15);
    SPI_WRITE(start);
    SPI_WRITE(end);
    CS_DIS();
}

/**
 * Set row address.
 *
 * @param start Start address. (0-63)
 * @param end End address. (0-63)
 */
void oled_set_row_address(int start, int end) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x75);
    SPI_WRITE(start);
    SPI_WRITE(end);
    CS_DIS();
}

/**
 * Set contrast for Color A.
 *
 * @param contrast Contrast. (Default:0x80)
 */
void oled_set_contrast_for_color_a(int contrast) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x81);
    SPI_WRITE(contrast);
    CS_DIS();
}

/**
 * Set contrast for Color B.
 *
 * @param contrast Contrast. (Default:0x80)
 */
void oled_set_contrast_for_color_b(int contrast) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x82);
    SPI_WRITE(contrast);
    CS_DIS();
}

/**
 * Set contrast for Color C.
 *
 * @param contrast Contrast. (Default:0x80)
 */
void oled_set_contrast_for_color_c(int contrast) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x83);
    SPI_WRITE(contrast);
    CS_DIS();
}

/**
 * Set master current control value.
 *
 * @param current Current value. (0x00-0x0F)
 */
void oled_set_master_current_control(int current) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x87);
    SPI_WRITE(current);
    CS_DIS();
}

/**
 * Set remapping mode and the data format.
 *
 * @param mode See the document.
 */
void oled_set_remap_and_data_format(int mode) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xA0);
    SPI_WRITE(mode);
    CS_DIS();
}

/**
 * Set display start line.
 *
 * @param line Start line number. (0-63)
 */
void oled_set_display_start_line(int line) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xA1);
    SPI_WRITE(line);
    CS_DIS();
}

/**
 * Set display offset line.
 *
 * @param offset Offset line number. (0-63)
 */
void oled_set_display_offset(int offset) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xA2);
    SPI_WRITE(offset);
    CS_DIS();
}

/**
 * Set display mode.
 *
 * @param mode Display mode.
 */
void oled_set_display_mode(DisplayMode mode) {
    int val = 0;
    switch (mode) {
        case NormalDisplay:
            val = 0xA4;
            break;
        case EntireDisplayOn:
            val = 0xA5;
            break;
        case EntireDisplayOff:
            val = 0xA6;
            break;
        case InverseDisplay:
            val = 0xA7;
            break;
        default:
            break;
    }
    if (val != 0) {
        DC_CMD();
        CS_ENA();
        SPI_WRITE(val);
        CS_DIS();
    }
}

/**
 * Set multiplex ratio.
 *
 * @param ratio Ratio.
 */
void oled_set_multiplex_ratio(int ratio) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xA8);
    SPI_WRITE(ratio);
    CS_DIS();
}

/**
 * Set display on/off.
 *
 * @param on On.
 */
void oled_set_display_on_off(unsigned char on) {
    if (on) {
        DC_CMD();
        CS_ENA();
        SPI_WRITE(0xAF);
        CS_DIS();
    } else {
        DC_CMD();
        CS_ENA();
        SPI_WRITE(0xAE);
        CS_DIS();
    }
}

/**
 * Set gray scale table.
 *
 * @param p A pointer to the look up table.
 */
void oled_set_gray_scale_table(gray_scale_table_t *p) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xB8);
    int i;
    for (i = 0; i < 32; i++) {
        SPI_WRITE(p->data[i]);
    }
    CS_DIS();
}

/**
 * NOP.
 */
void oled_nop() {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xE3);
    CS_DIS();
}

/**
 * Set power saving mode.
 *
 * @param value Value. (0x00:None, 0x12:power saving)
 */
void oled_set_power_saving_mode(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xB0);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set phase period.
 *
 * @param value Value. (Default:0x74)
 */
void oled_set_phase_period(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xB1);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set display clock divide ratio.
 *
 * @param value Value. (Default:0x00)
 */
void oled_set_display_clock_divide_ratio(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xB3);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set second pre-charge speed for color A.
 *
 * @param value Value.
 */
void oled_set_second_pre_charge_speed_for_color_a(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x8A);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set second pre-charge speed for color B.
 *
 * @param value Value.
 */
void oled_set_second_pre_charge_speed_for_color_b(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x8B);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set second pre-charge speed for color C.
 *
 * @param value Value.
 */
void oled_set_second_pre_charge_speed_for_color_c(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x8C);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set pre charge level for color A.
 *
 * @param value The value.
 */
void oled_set_pre_charge_level_for_color_a(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xBB);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set pre charge level for color B.
 *
 * @param value The value.
 */
void oled_set_pre_charge_level_for_color_b(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xBC);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set pre charge level for color C.
 *
 * @param value The value.
 */
void oled_set_pre_charge_level_for_color_c(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xBD);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Set VCOMH.
 *
 * @param value VCOMH value. (0x00:0.43 * Vref, 0x3F:0x83 * Vref)
 */
void oled_set_vcomh(int value) {
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0xBE);
    SPI_WRITE(value);
    CS_DIS();
}

/**
 * Read the status register.
 *
 * @return the value. (0x80:CommandLock)
 */
unsigned char oled_readStatusRegister() {
    unsigned char s;
    DC_CMD();
    CS_ENA();
    s = SPI_READ();
    CS_DIS();
    return s;
}
