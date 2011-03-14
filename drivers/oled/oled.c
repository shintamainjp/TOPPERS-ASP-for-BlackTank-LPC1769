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
#include "AitendoOLED.h"

#define RES_ENA()       io_res = 0
#define RES_DIS()       io_res = 1
#define CS_ENA()        io_cs = 0
#define CS_DIS()        io_cs = 1
#define DC_CMD()        io_dc = 0
#define DC_DAT()        io_dc = 1
#define SPI_WRITE(a)    io_spi.write((a))
#define SPI_READ()      io_spi.write(0x00)
#define WAIT_US(a)      wait_us((a))
#define swap(a,b) {int c=a;a=b;b=c;}

#if 0
        io_spi(pin_mosi, pin_miso, pin_sclk),
        io_res(pin_res),
        io_cs(pin_cs),
        io_dc(pin_dc) {
#endif

/**
 * Draw pixel.
 *
 * @param x X.
 * @param y Y.
 * @param c Color.
 */
void oled_draw_pixel(int x, int y, Color c) {
    setColumnAddress(x, x);
    setRowAddress(y, y);
    c.b = c.b >> 3;
    c.g = c.g >> 2;
    c.r = c.r >> 3;
    writeData((((c.b << 3) & 0xF8) | ((c.g >> 3) & 0x07)));
    writeData((((c.g << 5) & 0xE0) | ((c.r >> 0) & 0x1F)));
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
#if 0
    DC_CMD();
    CS_ENA();
    SPI_WRITE(0x21);
    SPI_WRITE(x1);
    SPI_WRITE(y1);
    SPI_WRITE(x2);
    SPI_WRITE(y2);
    SPI_WRITE(c.b >> 3);
    SPI_WRITE(c.g >> 2);
    SPI_WRITE(c.r >> 3);
    CS_DIS();
    WAIT_US(100);
#else
    /*
     * Bresenham's line algorithm
     */
    bool steep = abs(y2 - y1) > abs(x2 - x1);
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
    for (int x = x1; x <= x2; x++) {
        if (steep) {
            drawPixel(y, x, c);
        } else {
            drawPixel(x, y, c);
        }
        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
#endif
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
    WAIT_US(400);
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
    WAIT_US(400);
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
    WAIT_US(400);
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
    WAIT_US(400);
#else
    oled_Color c;
    c.r = 0x00;
    c.g = 0x00;
    c.b = 0x00;
    for (int x = x1; x <= x2; x++) {
        for (int y = y1; y <= y2; y++) {
            drawPixel(x, y, c);
        }
    }
#endif
}

/**
 * Reset.
 */
void oled_reset() {

    RES_ENA();
    WAIT_US(200 * 1000);
    RES_DIS();
    WAIT_US(200 * 1000);

    setDisplayOnOff(false);
    setRowAddress(0, 63);
    setColumnAddress(0, 95);
    setRemapAndDataFormat(0x70);
    setDisplayStartLine(0);
    setDisplayOffset(0);
    setDisplayMode(AitendoOLED::NormalDisplay);
    setMultiplexRatio(0x3f);
    setMasterCurrentControl(0x8f);
    setPowerSavingMode(0x00);
    setPhasePeriod(0x74);
    setDisplayClockDivideRatio(0xD0);
    setSecondPreChargeSpeedForColorA(0x80);
    setSecondPreChargeSpeedForColorB(0x80);
    setSecondPreChargeSpeedForColorC(0x80);
    setPreChargeLevelForColorA(0x3F);
    setPreChargeLevelForColorB(0x3F);
    setPreChargeLevelForColorC(0x3F);
    setVCOMH(0x3E);
    setMasterCurrentControl(0x0F);
    // A:Blue, B:Green, C:Red
    setContrastForColorA(0x80);
    setContrastForColorB(0x80);
    setContrastForColorC(0x80);
    setMode(true, false);
    setDisplayOnOff(true);
}

/**
 * Write data.
 *
 * @param data Data.
 */
void oled_write_data(uint8_t data) {
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
void oled_set_mode(bool fillrect, bool revcopy) {
    uint8_t val = 0x00;
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
void oled_set_display_on_off(bool on) {
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
    for (int i = 0; i < 32; i++) {
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
uint8_t oled_readStatusRegister() {
    uint8_t s;
    DC_CMD();
    CS_ENA();
    s = SPI_READ();
    CS_DIS();
    return s;
}