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
#ifndef AITENDO_OLED_H
#define AITENDO_OLED_H

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

/**
 * Draw pixel.
 *
 * @param x X.
 * @param y Y.
 * @param c Color.
 */
void oled_draw_pixel(int x, int y, Color c);

/**
 * Draw line.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 * @param c Color.
 */
void oled_draw_line(int x1, int y1, int x2, int y2, Color c);

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
void oled_fill_box(int x1, int y1, int x2, int y2, Color c1, Color c2);

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
void oled_copy(int x1, int y1, int x2, int y2, int nx, int ny);

/**
 * Darker.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 */
void oled_darker(int x1, int y1, int x2, int y2);

/**
 * Clear.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 */
void oled_clear(int x1, int y1, int x2, int y2);


/*
SPI io_spi;
DigitalOut io_res;
DigitalOut io_cs;
DigitalOut io_dc;
*/

/**
 * Reset.
 */
static void oled_reset();

/**
 * Write data.
 *
 * @param data Data.
 */
static void oled_write_data(uint8_t data);

/**
 * Set mode.
 *
 * @param fillrect Fill the rectangle.
 * @param revcopy Reverse copy.
 */
static void oled_set_mode(bool fillrect, bool revcopy);

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
static void oled_set_display_on_off(bool on);

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
static uint8_t oled_read_status_register();
};

#endif
