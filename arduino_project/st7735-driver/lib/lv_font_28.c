/*******************************************************************************
 * Size: 28 px
 * Bpp: 1
 * Opts: --bpp 1 --size 28 --font STSONG.TTF --range 32-127 --format lvgl -o lv_font_28.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_FONT_28
#define LV_FONT_28 1
#endif

#if LV_FONT_28

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfd, 0x55, 0x55, 0x43, 0xc0,

    /* U+0022 "\"" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xe1, 0x42,

    /* U+0023 "#" */
    0x2, 0x4, 0x6, 0x4, 0x4, 0x4, 0x4, 0xc,
    0x4, 0x8, 0x4, 0x8, 0xff, 0xff, 0x8, 0x8,
    0x8, 0x18, 0x8, 0x10, 0x18, 0x10, 0x10, 0x10,
    0xff, 0xff, 0x10, 0x20, 0x10, 0x20, 0x30, 0x20,
    0x20, 0x20, 0x20, 0x60, 0x20, 0x40,

    /* U+0024 "$" */
    0x12, 0x4, 0x83, 0xf1, 0x4b, 0x92, 0xe4, 0x89,
    0x22, 0x48, 0x72, 0x1e, 0x81, 0xe0, 0x5e, 0x13,
    0x84, 0xb1, 0x2c, 0x4b, 0xd2, 0xfc, 0xe3, 0xf0,
    0x48, 0x12, 0x4, 0x81, 0x20,

    /* U+0025 "%" */
    0x38, 0x1, 0x2, 0x20, 0x8, 0x31, 0x80, 0x81,
    0x8c, 0x8, 0xc, 0x60, 0x40, 0x63, 0x4, 0x3,
    0x18, 0x40, 0x8, 0x82, 0xc, 0x30, 0x21, 0x10,
    0x2, 0x8, 0x80, 0x10, 0xc6, 0x1, 0x6, 0x30,
    0x10, 0x31, 0x81, 0x1, 0x8c, 0x8, 0xc, 0x60,
    0x80, 0x22, 0x8, 0x0, 0xa0, 0x40, 0x2, 0x0,

    /* U+0026 "&" */
    0x7, 0x80, 0x1, 0x18, 0x0, 0x61, 0x80, 0xc,
    0x30, 0x1, 0xc6, 0x0, 0x1d, 0x80, 0x1, 0xc1,
    0xf0, 0xdc, 0x1c, 0x31, 0xc3, 0xc, 0x18, 0x43,
    0x1, 0x90, 0x60, 0x3e, 0xc, 0x3, 0x81, 0xc0,
    0x38, 0x1c, 0xb, 0x83, 0xfe, 0x3f, 0x1f, 0x3,
    0xc0,

    /* U+0027 "'" */
    0xff, 0xfa,

    /* U+0028 "(" */
    0x2, 0x8, 0x20, 0xc3, 0x4, 0x18, 0x30, 0xc1,
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0x60, 0xc0,
    0x81, 0x81, 0x1, 0x1, 0x1,

    /* U+0029 ")" */
    0x80, 0x80, 0x81, 0x81, 0x81, 0x3, 0x6, 0x4,
    0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0xc, 0x18,
    0x20, 0xc1, 0x4, 0x10, 0x40,

    /* U+002A "*" */
    0x8, 0x2, 0x8, 0x83, 0x26, 0x3e, 0x7, 0x6,
    0xb2, 0x23, 0x8, 0x2, 0x0, 0x80,

    /* U+002B "+" */
    0x1, 0x0, 0x2, 0x0, 0x4, 0x0, 0x8, 0x0,
    0x10, 0x0, 0x20, 0x0, 0x40, 0x7f, 0xff, 0x1,
    0x0, 0x2, 0x0, 0x4, 0x0, 0x8, 0x0, 0x10,
    0x0, 0x20, 0x0, 0x40, 0x0,

    /* U+002C "," */
    0x73, 0xc6, 0x33, 0x30,

    /* U+002D "-" */
    0xfe,

    /* U+002E "." */
    0x5f, 0x80,

    /* U+002F "/" */
    0x0, 0x40, 0x10, 0x8, 0x2, 0x1, 0x80, 0x40,
    0x30, 0x8, 0x2, 0x1, 0x0, 0x40, 0x30, 0x8,
    0x2, 0x1, 0x0, 0x40, 0x30, 0x8, 0x6, 0x1,
    0x0, 0x40, 0x20, 0x8, 0x0,

    /* U+0030 "0" */
    0xe, 0x6, 0x30, 0x82, 0x20, 0x24, 0x5, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x34, 0x4, 0xc0, 0x88, 0x21, 0x84, 0xf, 0x0,

    /* U+0031 "1" */
    0xc, 0x1c, 0x2c, 0x4c, 0xc, 0xc, 0xc, 0xc,
    0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
    0x3f,

    /* U+0032 "2" */
    0xf, 0x3, 0xbc, 0x20, 0xe4, 0x6, 0x40, 0x60,
    0x6, 0x0, 0x60, 0xc, 0x0, 0x80, 0x10, 0x2,
    0x0, 0x40, 0x8, 0x1, 0x0, 0x20, 0x17, 0xfe,
    0xff, 0xe0,

    /* U+0033 "3" */
    0xf, 0x3, 0x78, 0x83, 0x0, 0x60, 0x8, 0x6,
    0x0, 0xe0, 0x6e, 0x0, 0xe0, 0xc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0x80, 0x33, 0xc, 0x7e, 0x0,

    /* U+0034 "4" */
    0x0, 0x80, 0x18, 0x3, 0x80, 0x38, 0x5, 0x80,
    0x98, 0x11, 0x81, 0x18, 0x21, 0x84, 0x18, 0x41,
    0x8f, 0xff, 0x1, 0x80, 0x18, 0x1, 0x80, 0x18,
    0x1, 0x80,

    /* U+0035 "5" */
    0x0, 0x47, 0xf8, 0xfe, 0x10, 0x2, 0x0, 0x40,
    0x8, 0x1, 0xf8, 0x3f, 0x80, 0x38, 0x3, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18, 0x6, 0xe1, 0x87,
    0xc0,

    /* U+0036 "6" */
    0x0, 0xc0, 0x60, 0x30, 0xc, 0x3, 0x0, 0x60,
    0x18, 0x3, 0x38, 0xc0, 0xd8, 0x1b, 0x1, 0xe0,
    0x3c, 0x7, 0x80, 0xd0, 0x1b, 0x6, 0x30, 0x83,
    0xe0,

    /* U+0037 "7" */
    0xff, 0xff, 0xfa, 0x1, 0x0, 0x20, 0x8, 0x1,
    0x0, 0x40, 0x8, 0x1, 0x0, 0x40, 0x8, 0x2,
    0x0, 0x40, 0x8, 0x2, 0x0, 0x40, 0x8, 0x0,

    /* U+0038 "8" */
    0x1f, 0xc, 0x33, 0x3, 0x60, 0x6c, 0xd, 0x81,
    0x98, 0x41, 0xd0, 0x17, 0xc, 0x3b, 0x3, 0x60,
    0x3c, 0x7, 0x80, 0xf0, 0x13, 0x6, 0x1f, 0x0,

    /* U+0039 "9" */
    0x1f, 0x4, 0x31, 0x83, 0x60, 0x2c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0x60, 0x6c, 0xc, 0x73, 0x0,
    0x60, 0xc, 0x3, 0x0, 0xc0, 0x30, 0xc, 0xe,
    0x0,

    /* U+003A ":" */
    0xff, 0x80, 0x0, 0xff, 0x80,

    /* U+003B ";" */
    0x73, 0x98, 0x0, 0x0, 0x0, 0x3, 0x9e, 0x31,
    0x99, 0x80,

    /* U+003C "<" */
    0x0, 0x6, 0x0, 0x70, 0x3, 0x80, 0x1c, 0x1,
    0xc0, 0xe, 0x0, 0x30, 0x0, 0x38, 0x0, 0xe,
    0x0, 0x7, 0x0, 0x3, 0x80, 0x0, 0xe0, 0x0,
    0x60,

    /* U+003D "=" */
    0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0x80,

    /* U+003E ">" */
    0xc0, 0x0, 0xe0, 0x0, 0x70, 0x0, 0x1c, 0x0,
    0xe, 0x0, 0x3, 0x80, 0x1, 0x80, 0x1c, 0x0,
    0xe0, 0xf, 0x0, 0x70, 0x3, 0x80, 0x8, 0x0,
    0x0,

    /* U+003F "?" */
    0x70, 0x7c, 0x2, 0x2, 0x1, 0x1, 0x1, 0x3,
    0x3e, 0x3c, 0x20, 0x20, 0x20, 0x0, 0x20, 0x70,
    0x70,

    /* U+0040 "@" */
    0x0, 0x3f, 0x80, 0x1, 0xc0, 0x60, 0x7, 0x0,
    0x10, 0xc, 0x0, 0x8, 0x18, 0x0, 0x4, 0x30,
    0x0, 0x4, 0x30, 0xc, 0xc2, 0x60, 0x33, 0x82,
    0x60, 0x61, 0x82, 0xe0, 0xc1, 0x82, 0xc1, 0x83,
    0x82, 0xc1, 0x83, 0x2, 0xc3, 0x3, 0x4, 0xc3,
    0x7, 0x4, 0xc3, 0xe, 0x8, 0xc3, 0x16, 0x18,
    0xc3, 0xe6, 0x31, 0x61, 0xc3, 0xc1, 0x60, 0x0,
    0x2, 0x30, 0x0, 0x4, 0x30, 0x0, 0x4, 0x18,
    0x0, 0x8, 0xe, 0x0, 0x30, 0x3, 0x80, 0xc0,
    0x0, 0xff, 0x0,

    /* U+0041 "A" */
    0x0, 0x40, 0x0, 0xc, 0x0, 0x3, 0x80, 0x0,
    0x70, 0x0, 0x13, 0x0, 0x2, 0x60, 0x0, 0x44,
    0x0, 0x10, 0xc0, 0x2, 0x18, 0x0, 0x7f, 0x80,
    0x10, 0x30, 0x2, 0x6, 0x0, 0x80, 0x60, 0x10,
    0xc, 0x2, 0x1, 0xc0, 0x80, 0x18, 0x10, 0x3,
    0xf, 0x81, 0xf8,

    /* U+0042 "B" */
    0xff, 0xe0, 0x30, 0x30, 0x60, 0x30, 0xc0, 0x61,
    0x80, 0xc3, 0x3, 0x6, 0xc, 0xf, 0xfc, 0x18,
    0x1c, 0x30, 0x1c, 0x60, 0x18, 0xc0, 0x31, 0x80,
    0x63, 0x0, 0xc6, 0x3, 0xe, 0xc, 0xff, 0xe0,

    /* U+0043 "C" */
    0x3, 0xfc, 0x18, 0xc, 0x40, 0x9, 0x0, 0x16,
    0x0, 0x8, 0x0, 0x30, 0x0, 0x60, 0x0, 0xc0,
    0x1, 0x80, 0x3, 0x0, 0x6, 0x0, 0x6, 0x0,
    0xc, 0x0, 0x4c, 0x0, 0x84, 0x3, 0x7, 0xfc,

    /* U+0044 "D" */
    0xff, 0xf8, 0x1, 0x80, 0x70, 0x18, 0x3, 0x81,
    0x80, 0x1c, 0x18, 0x0, 0xe1, 0x80, 0x6, 0x18,
    0x0, 0x71, 0x80, 0x7, 0x18, 0x0, 0x71, 0x80,
    0x7, 0x18, 0x0, 0x71, 0x80, 0xe, 0x18, 0x0,
    0xe1, 0x80, 0x1c, 0x18, 0x3, 0x81, 0xc0, 0x70,
    0xff, 0xfc, 0x0,

    /* U+0045 "E" */
    0x7f, 0xff, 0xc, 0x1, 0x86, 0x0, 0x43, 0x0,
    0x21, 0x80, 0x0, 0xc0, 0x20, 0x60, 0x10, 0x30,
    0x8, 0x1f, 0xfc, 0xc, 0x6, 0x6, 0x1, 0x3,
    0x0, 0x81, 0x80, 0x0, 0xc0, 0x4, 0x60, 0x2,
    0x30, 0x3, 0xff, 0xff, 0x80,

    /* U+0046 "F" */
    0x7f, 0xfe, 0x30, 0x4, 0x60, 0x8, 0xc0, 0x11,
    0x80, 0x3, 0x0, 0x6, 0x2, 0xc, 0x4, 0x1f,
    0xf8, 0x30, 0x10, 0x60, 0x20, 0xc0, 0x1, 0x80,
    0x3, 0x0, 0x6, 0x0, 0xc, 0x0, 0x7e, 0x0,

    /* U+0047 "G" */
    0x1, 0xfe, 0x0, 0xe0, 0x38, 0x18, 0x1, 0x83,
    0x0, 0x8, 0x70, 0x0, 0x6, 0x0, 0x0, 0xe0,
    0x0, 0xe, 0x0, 0x0, 0xe0, 0x0, 0xe, 0x1,
    0xff, 0xe0, 0x1, 0x8f, 0x0, 0x18, 0x70, 0x1,
    0x87, 0x80, 0x18, 0x3c, 0x1, 0x80, 0xe0, 0x38,
    0x3, 0xfc, 0x0,

    /* U+0048 "H" */
    0x3e, 0x3, 0xe1, 0x80, 0x18, 0x18, 0x1, 0x81,
    0x80, 0x18, 0x18, 0x1, 0x81, 0x80, 0x18, 0x18,
    0x1, 0x81, 0x80, 0x18, 0x1f, 0xff, 0x81, 0x80,
    0x18, 0x18, 0x1, 0x81, 0x80, 0x18, 0x18, 0x1,
    0x81, 0x80, 0x18, 0x18, 0x1, 0x81, 0x80, 0x18,
    0x7e, 0x7, 0xe0,

    /* U+0049 "I" */
    0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0xff,

    /* U+004A "J" */
    0xf, 0x80, 0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0,
    0x30, 0xc, 0x3, 0x0, 0xc0, 0x30, 0xc, 0x3,
    0x0, 0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,
    0x8, 0x2, 0x1, 0xc, 0x83, 0xc0,

    /* U+004B "K" */
    0xfc, 0xf, 0xc3, 0x0, 0x20, 0x30, 0xc, 0x3,
    0x1, 0x0, 0x30, 0x20, 0x3, 0x4, 0x0, 0x31,
    0x80, 0x3, 0x20, 0x0, 0x3f, 0x0, 0x3, 0x38,
    0x0, 0x31, 0xe0, 0x3, 0x7, 0x0, 0x30, 0x38,
    0x3, 0x1, 0xc0, 0x30, 0xf, 0x3, 0x0, 0x38,
    0xfc, 0xf, 0xf0,

    /* U+004C "L" */
    0x7e, 0x0, 0x30, 0x0, 0x60, 0x0, 0xc0, 0x1,
    0x80, 0x3, 0x0, 0x6, 0x0, 0xc, 0x0, 0x18,
    0x0, 0x30, 0x0, 0x60, 0x0, 0xc0, 0x1, 0x80,
    0x3, 0x0, 0x46, 0x0, 0x8c, 0x3, 0x7f, 0xfe,

    /* U+004D "M" */
    0x7c, 0x0, 0x3e, 0x3c, 0x0, 0xe0, 0x58, 0x1,
    0xc0, 0xb8, 0x5, 0x81, 0x30, 0xb, 0x2, 0x70,
    0x16, 0x4, 0x60, 0x4c, 0x8, 0xc0, 0x98, 0x10,
    0xc2, 0x30, 0x21, 0x84, 0x60, 0x43, 0x90, 0xc0,
    0x83, 0x21, 0x81, 0x7, 0x43, 0x2, 0x7, 0x6,
    0x4, 0xe, 0xc, 0x8, 0x8, 0x18, 0xfe, 0x10,
    0xfe,

    /* U+004E "N" */
    0xf0, 0x1, 0xe1, 0x80, 0x4, 0x1c, 0x0, 0x41,
    0x60, 0x4, 0x13, 0x0, 0x41, 0x38, 0x4, 0x11,
    0xc0, 0x41, 0xe, 0x4, 0x10, 0x70, 0x41, 0x3,
    0x84, 0x10, 0x1c, 0x41, 0x0, 0xe4, 0x10, 0x7,
    0x41, 0x0, 0x34, 0x10, 0x1, 0xc1, 0x0, 0xc,
    0xfe, 0x0, 0x40,

    /* U+004F "O" */
    0x3, 0xf8, 0x1, 0x80, 0xc0, 0x40, 0x4, 0x10,
    0x0, 0xc6, 0x0, 0xc, 0xc0, 0x0, 0xb0, 0x0,
    0x1e, 0x0, 0x3, 0xc0, 0x0, 0x78, 0x0, 0xf,
    0x0, 0x1, 0xe0, 0x0, 0x26, 0x0, 0xc, 0x60,
    0x1, 0xc, 0x0, 0x60, 0x60, 0x10, 0x3, 0xfc,
    0x0,

    /* U+0050 "P" */
    0xff, 0xe0, 0x60, 0xe1, 0x81, 0x86, 0x3, 0x18,
    0xc, 0x60, 0x31, 0x80, 0xc6, 0x6, 0x18, 0x38,
    0x67, 0x81, 0x80, 0x6, 0x0, 0x18, 0x0, 0x60,
    0x1, 0x80, 0x6, 0x0, 0x7f, 0x0,

    /* U+0051 "Q" */
    0x1, 0xf8, 0x0, 0xe0, 0x60, 0x18, 0x3, 0x3,
    0x0, 0x18, 0x60, 0x0, 0xc6, 0x0, 0xc, 0x40,
    0x0, 0x6c, 0x0, 0x6, 0xc0, 0x0, 0x6c, 0x0,
    0x6, 0xc0, 0x0, 0x6c, 0x0, 0x6, 0x60, 0x0,
    0x46, 0x0, 0xc, 0x30, 0x0, 0x81, 0x0, 0x10,
    0xc, 0x6, 0x0, 0x3f, 0x80, 0x0, 0x70, 0x0,
    0x3, 0x80, 0x0, 0x1e, 0x0, 0x0, 0x78, 0x0,
    0x1, 0xe0,

    /* U+0052 "R" */
    0xff, 0xe0, 0xc, 0x38, 0x6, 0xe, 0x3, 0x3,
    0x1, 0x81, 0x80, 0xc0, 0xc0, 0x60, 0xc0, 0x30,
    0xc0, 0x1f, 0xc0, 0xc, 0x70, 0x6, 0x38, 0x3,
    0xe, 0x1, 0x83, 0x0, 0xc1, 0xc0, 0x60, 0x70,
    0x30, 0x38, 0x7e, 0xf, 0x80,

    /* U+0053 "S" */
    0x1f, 0xc, 0x1b, 0x3, 0x60, 0x2c, 0x1, 0xc0,
    0x1e, 0x1, 0xf0, 0x1f, 0x80, 0xf8, 0x7, 0x80,
    0x78, 0x7, 0x0, 0xf0, 0x17, 0x6, 0x7f, 0x0,

    /* U+0054 "T" */
    0x60, 0x0, 0xdf, 0xff, 0xf6, 0xc, 0xd, 0x3,
    0x3, 0x80, 0xc0, 0x40, 0x30, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc0, 0x0, 0x30, 0x0, 0xc,
    0x0, 0x3, 0x0, 0x0, 0xc0, 0x0, 0x30, 0x0,
    0xc, 0x0, 0x3, 0x0, 0x0, 0xc0, 0x1, 0xfe,
    0x0,

    /* U+0055 "U" */
    0x7e, 0x3, 0xe3, 0x0, 0x10, 0x60, 0x2, 0xc,
    0x0, 0x41, 0x80, 0x8, 0x30, 0x1, 0x6, 0x0,
    0x20, 0xc0, 0x4, 0x18, 0x0, 0x83, 0x0, 0x10,
    0x60, 0x2, 0xc, 0x0, 0x41, 0x80, 0x8, 0x30,
    0x2, 0x3, 0x0, 0x40, 0x30, 0x30, 0x1, 0xfc,
    0x0,

    /* U+0056 "V" */
    0xfe, 0x7, 0xe3, 0x0, 0x30, 0x60, 0x4, 0xe,
    0x0, 0x80, 0xc0, 0x30, 0x1c, 0x4, 0x1, 0x80,
    0x80, 0x38, 0x20, 0x3, 0x4, 0x0, 0x60, 0x80,
    0xe, 0x20, 0x0, 0xc4, 0x0, 0x1d, 0x0, 0x1,
    0xa0, 0x0, 0x3c, 0x0, 0x3, 0x0, 0x0, 0x60,
    0x0, 0x8, 0x0,

    /* U+0057 "W" */
    0xf9, 0xf9, 0xe7, 0x98, 0x38, 0x41, 0xe, 0xc,
    0x40, 0x83, 0x6, 0x20, 0x41, 0x81, 0x90, 0x40,
    0xe0, 0xd0, 0x20, 0x30, 0x78, 0x20, 0x1c, 0x18,
    0x10, 0x6, 0xc, 0x8, 0x3, 0x3, 0x8, 0x1,
    0xc3, 0x84, 0x0, 0x62, 0x62, 0x0, 0x39, 0x32,
    0x0, 0xd, 0xd, 0x0, 0x6, 0x86, 0x80, 0x1,
    0x83, 0x80, 0x0, 0xc0, 0xc0, 0x0, 0x40, 0x40,
    0x0,

    /* U+0058 "X" */
    0x7f, 0xf, 0xc3, 0x80, 0x60, 0x38, 0x8, 0x7,
    0x2, 0x0, 0x70, 0x80, 0x7, 0x20, 0x0, 0x64,
    0x0, 0xf, 0x0, 0x0, 0xe0, 0x0, 0x1e, 0x0,
    0x4, 0xc0, 0x0, 0x8c, 0x0, 0x21, 0xc0, 0x8,
    0x1c, 0x2, 0x1, 0xc0, 0xc0, 0x38, 0x7f, 0xf,
    0xe0,

    /* U+0059 "Y" */
    0x7e, 0x3, 0xe3, 0x80, 0x10, 0x30, 0x4, 0x7,
    0x1, 0x0, 0x70, 0x20, 0x6, 0x8, 0x0, 0xe2,
    0x0, 0xe, 0x80, 0x0, 0xd0, 0x0, 0xc, 0x0,
    0x1, 0x80, 0x0, 0x30, 0x0, 0x6, 0x0, 0x0,
    0xc0, 0x0, 0x18, 0x0, 0x3, 0x0, 0x3, 0xfc,
    0x0,

    /* U+005A "Z" */
    0x0, 0x0, 0x40, 0x0, 0x7f, 0xfe, 0x40, 0xe,
    0x80, 0x1c, 0x80, 0x38, 0x0, 0x30, 0x0, 0x60,
    0x0, 0xe0, 0x1, 0xc0, 0x3, 0x80, 0x3, 0x0,
    0x6, 0x0, 0xe, 0x0, 0x1c, 0x0, 0x18, 0x1,
    0x30, 0x1, 0x70, 0x3, 0xff, 0xff,

    /* U+005B "[" */
    0xff, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc,
    0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0xf, 0xc0,

    /* U+005C "\\" */
    0x80, 0x20, 0x4, 0x1, 0x0, 0x60, 0x8, 0x3,
    0x0, 0x40, 0x10, 0x2, 0x0, 0x80, 0x30, 0x4,
    0x1, 0x80, 0x20, 0x8, 0x1, 0x0, 0x40, 0x8,
    0x2, 0x0, 0xc0, 0x10, 0x4,

    /* U+005D "]" */
    0xfc, 0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30,
    0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc,
    0x3f, 0xc0,

    /* U+005E "^" */
    0x0, 0x0, 0x60, 0xf, 0x1, 0x98, 0x10, 0x82,
    0x4, 0x40, 0x2c, 0x3,

    /* U+005F "_" */
    0xff, 0xfc,

    /* U+0060 "`" */
    0xc3, 0x8, 0x20,

    /* U+0061 "a" */
    0x1f, 0x4, 0x31, 0x86, 0x30, 0xc0, 0x18, 0xf,
    0x6, 0x61, 0x8c, 0x61, 0x8c, 0x75, 0xf7, 0x0,

    /* U+0062 "b" */
    0x70, 0x3, 0x80, 0xc, 0x0, 0x60, 0x3, 0x0,
    0x18, 0x0, 0xc0, 0x6, 0xf8, 0x38, 0x71, 0x81,
    0x8c, 0x6, 0x60, 0x33, 0x1, 0x98, 0xc, 0xc0,
    0x66, 0x6, 0x38, 0x61, 0x3e, 0x0,

    /* U+0063 "c" */
    0x1f, 0x98, 0x66, 0xb, 0x0, 0xc0, 0x30, 0xc,
    0x3, 0x0, 0x60, 0x4e, 0x61, 0xe0,

    /* U+0064 "d" */
    0x0, 0xe0, 0xf, 0x0, 0x18, 0x0, 0xc0, 0x6,
    0x0, 0x30, 0x1, 0x83, 0xec, 0x30, 0xe3, 0x3,
    0x30, 0x19, 0x80, 0xcc, 0x6, 0x60, 0x33, 0x1,
    0x8c, 0xc, 0x70, 0xe0, 0xfb, 0xc0,

    /* U+0065 "e" */
    0x1f, 0x8, 0x64, 0xf, 0xff, 0xc0, 0x30, 0xc,
    0x3, 0x80, 0x60, 0x5f, 0x61, 0xf0,

    /* U+0066 "f" */
    0x7, 0xc2, 0x71, 0x0, 0x40, 0x30, 0xc, 0x3,
    0x1, 0xf8, 0xb0, 0xc, 0x3, 0x0, 0xc0, 0x30,
    0xc, 0x3, 0x0, 0xc0, 0x30, 0x3f, 0x80,

    /* U+0067 "g" */
    0x1f, 0x3, 0x1f, 0x60, 0xc6, 0xc, 0x60, 0xc6,
    0xc, 0x31, 0x80, 0xe0, 0x70, 0x6, 0x0, 0x38,
    0x1, 0xfe, 0x30, 0xf6, 0x1, 0xc0, 0x1c, 0x2,
    0x60, 0x23, 0xfc,

    /* U+0068 "h" */
    0x30, 0x3, 0xc0, 0x3, 0x0, 0xc, 0x0, 0x30,
    0x0, 0xc0, 0x3, 0x0, 0xc, 0xf0, 0x34, 0x60,
    0xe0, 0xc3, 0x3, 0xc, 0xc, 0x30, 0x30, 0xc0,
    0xc3, 0x3, 0xc, 0xc, 0x30, 0x33, 0xf1, 0xe0,

    /* U+0069 "i" */
    0x30, 0xc0, 0x0, 0x0, 0x1, 0x1c, 0x30, 0xc3,
    0xc, 0x30, 0xc3, 0xc, 0x78,

    /* U+006A "j" */
    0x77, 0x0, 0x0, 0x3f, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x32, 0x48,

    /* U+006B "k" */
    0x30, 0x7, 0x80, 0xc, 0x0, 0x60, 0x3, 0x0,
    0x18, 0x0, 0xc0, 0x6, 0x7e, 0x30, 0x81, 0x88,
    0xc, 0x80, 0x68, 0x3, 0xe0, 0x19, 0x80, 0xc6,
    0x6, 0x30, 0x30, 0xc3, 0xc7, 0x80,

    /* U+006C "l" */
    0x31, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0xc, 0x30, 0xc3, 0xc, 0x33, 0xf0,

    /* U+006D "m" */
    0x10, 0x0, 0x0, 0xcf, 0x8f, 0x7, 0x47, 0xc6,
    0xe, 0xc, 0xc, 0x30, 0x30, 0x30, 0xc0, 0xc0,
    0xc3, 0x3, 0x3, 0xc, 0xc, 0xc, 0x30, 0x30,
    0x30, 0xc0, 0xc0, 0xc3, 0x3, 0x3, 0x1f, 0x3f,
    0x3e,

    /* U+006E "n" */
    0x10, 0x0, 0x47, 0x83, 0x67, 0x3e, 0xc, 0x30,
    0x30, 0xc0, 0xc3, 0x3, 0xc, 0xc, 0x30, 0x30,
    0xc0, 0xc3, 0x3, 0x3f, 0x3e,

    /* U+006F "o" */
    0x1f, 0x82, 0xc, 0x60, 0x6c, 0x3, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0x3, 0x60, 0x62, 0xc, 0x1f,
    0x80,

    /* U+0070 "p" */
    0x10, 0x1, 0x9f, 0x1f, 0x3c, 0x60, 0x63, 0x1,
    0x98, 0xc, 0xc0, 0x66, 0x3, 0x30, 0x19, 0x81,
    0x8e, 0x8, 0x6f, 0x83, 0x0, 0x18, 0x0, 0xc0,
    0x6, 0x0, 0x30, 0x1, 0x80, 0x1f, 0x0,

    /* U+0071 "q" */
    0x0, 0x20, 0xff, 0x8, 0x38, 0xc0, 0xcc, 0x6,
    0x60, 0x33, 0x1, 0x98, 0xc, 0xe0, 0x63, 0x3,
    0xe, 0x38, 0x3e, 0xc0, 0x6, 0x0, 0x30, 0x1,
    0x80, 0xc, 0x0, 0x60, 0x3, 0x0, 0x7c,

    /* U+0072 "r" */
    0x10, 0x9, 0x9d, 0xdf, 0x3, 0x1, 0x80, 0xc0,
    0x60, 0x30, 0x18, 0xc, 0x1f, 0xc0,

    /* U+0073 "s" */
    0x3e, 0xc6, 0xc2, 0xe0, 0x78, 0x3e, 0xf, 0x83,
    0x83, 0xc2, 0x7c,

    /* U+0074 "t" */
    0x0, 0x10, 0x30, 0x7f, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x1f,

    /* U+0075 "u" */
    0x71, 0xe1, 0x83, 0xc, 0x18, 0x60, 0xc3, 0x6,
    0x18, 0x30, 0xc1, 0x86, 0xc, 0x30, 0x60, 0xdf,
    0x87, 0x98,

    /* U+0076 "v" */
    0xfc, 0xf9, 0x81, 0x4, 0x10, 0x30, 0x81, 0x84,
    0x6, 0x40, 0x32, 0x0, 0xe0, 0x7, 0x0, 0x10,
    0x0, 0x80,

    /* U+0077 "w" */
    0xf1, 0xf1, 0xce, 0x18, 0x10, 0xc1, 0x84, 0x1c,
    0x30, 0x81, 0x86, 0x10, 0x38, 0xe4, 0x3, 0x2c,
    0x80, 0x24, 0xa0, 0x7, 0x1c, 0x0, 0x61, 0x0,
    0x8, 0x20, 0x0,

    /* U+0078 "x" */
    0xf9, 0xe3, 0x8, 0x31, 0x1, 0xa0, 0xe, 0x0,
    0x60, 0xb, 0x1, 0x30, 0x11, 0x82, 0xc, 0xf1,
    0xe0,

    /* U+0079 "y" */
    0xf8, 0xe3, 0x4, 0x30, 0x81, 0x8, 0x18, 0x81,
    0x90, 0x9, 0x0, 0xd0, 0xe, 0x0, 0x60, 0x6,
    0x0, 0x40, 0x4, 0x0, 0x80, 0x8, 0x0, 0x80,
    0x70, 0x6, 0x0,

    /* U+007A "z" */
    0x40, 0x1f, 0xf4, 0x18, 0xc, 0x3, 0x1, 0x80,
    0xc0, 0x60, 0x38, 0xc, 0x16, 0xf, 0xff,

    /* U+007B "{" */
    0x6, 0x8, 0x10, 0x30, 0x30, 0x30, 0x38, 0x18,
    0x18, 0x18, 0x10, 0x60, 0x80, 0x60, 0x10, 0x18,
    0x18, 0x38, 0x38, 0x30, 0x30, 0x30, 0x10, 0x8,
    0x7,

    /* U+007C "|" */
    0xff, 0xff, 0xfe,

    /* U+007D "}" */
    0x60, 0x10, 0x8, 0xc, 0xc, 0xc, 0x1c, 0x18,
    0x18, 0x18, 0x8, 0x6, 0x1, 0x6, 0x8, 0x18,
    0x18, 0x1c, 0x1c, 0xc, 0xc, 0xc, 0x8, 0x10,
    0xe0,

    /* U+007E "~" */
    0x7c, 0x5, 0x8f, 0xa, 0x3, 0xe0,

    /* U+007F "" */
    0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 112, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 98, .box_w = 2, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 182, .box_w = 7, .box_h = 8, .ofs_x = 2, .ofs_y = 11},
    {.bitmap_index = 13, .adv_w = 299, .box_w = 16, .box_h = 19, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 51, .adv_w = 201, .box_w = 10, .box_h = 23, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 80, .adv_w = 369, .box_w = 21, .box_h = 18, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 128, .adv_w = 327, .box_w = 19, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 79, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 171, .adv_w = 131, .box_w = 7, .box_h = 24, .ofs_x = 2, .ofs_y = -6},
    {.bitmap_index = 192, .adv_w = 131, .box_w = 7, .box_h = 24, .ofs_x = 0, .ofs_y = -6},
    {.bitmap_index = 213, .adv_w = 191, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 227, .adv_w = 299, .box_w = 15, .box_h = 15, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 256, .adv_w = 98, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 260, .adv_w = 140, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 261, .adv_w = 98, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 224, .box_w = 10, .box_h = 23, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 292, .adv_w = 210, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 210, .box_w = 8, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 333, .adv_w = 210, .box_w = 12, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 359, .adv_w = 210, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 210, .box_w = 12, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 409, .adv_w = 210, .box_w = 11, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 210, .box_w = 11, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 459, .adv_w = 210, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 210, .box_w = 11, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 210, .box_w = 11, .box_h = 18, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 532, .adv_w = 98, .box_w = 3, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 537, .adv_w = 98, .box_w = 5, .box_h = 15, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 547, .adv_w = 299, .box_w = 15, .box_h = 13, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 572, .adv_w = 299, .box_w = 15, .box_h = 7, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 586, .adv_w = 299, .box_w = 15, .box_h = 13, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 611, .adv_w = 164, .box_w = 8, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 411, .box_w = 24, .box_h = 25, .ofs_x = 3, .ofs_y = -7},
    {.bitmap_index = 703, .adv_w = 303, .box_w = 19, .box_h = 18, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 746, .adv_w = 276, .box_w = 15, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 778, .adv_w = 284, .box_w = 15, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 810, .adv_w = 345, .box_w = 20, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 853, .adv_w = 294, .box_w = 17, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 890, .adv_w = 252, .box_w = 15, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 922, .adv_w = 345, .box_w = 20, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 965, .adv_w = 340, .box_w = 20, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1008, .adv_w = 159, .box_w = 8, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1025, .adv_w = 149, .box_w = 10, .box_h = 24, .ofs_x = -2, .ofs_y = -7},
    {.bitmap_index = 1055, .adv_w = 332, .box_w = 20, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1098, .adv_w = 257, .box_w = 15, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1130, .adv_w = 373, .box_w = 23, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1179, .adv_w = 345, .box_w = 20, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1222, .adv_w = 350, .box_w = 19, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1263, .adv_w = 252, .box_w = 14, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1293, .adv_w = 345, .box_w = 20, .box_h = 23, .ofs_x = 1, .ofs_y = -6},
    {.bitmap_index = 1351, .adv_w = 280, .box_w = 17, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1388, .adv_w = 215, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1412, .adv_w = 276, .box_w = 18, .box_h = 18, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1453, .adv_w = 317, .box_w = 19, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1494, .adv_w = 303, .box_w = 19, .box_h = 18, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1537, .adv_w = 396, .box_w = 25, .box_h = 18, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1594, .adv_w = 313, .box_w = 19, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1635, .adv_w = 294, .box_w = 19, .box_h = 17, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 1676, .adv_w = 294, .box_w = 16, .box_h = 19, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1714, .adv_w = 121, .box_w = 6, .box_h = 23, .ofs_x = 3, .ofs_y = -6},
    {.bitmap_index = 1732, .adv_w = 224, .box_w = 10, .box_h = 23, .ofs_x = 2, .ofs_y = -6},
    {.bitmap_index = 1761, .adv_w = 121, .box_w = 6, .box_h = 23, .ofs_x = -1, .ofs_y = -6},
    {.bitmap_index = 1779, .adv_w = 224, .box_w = 12, .box_h = 8, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 1791, .adv_w = 224, .box_w = 14, .box_h = 1, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1793, .adv_w = 149, .box_w = 5, .box_h = 4, .ofs_x = 3, .ofs_y = 13},
    {.bitmap_index = 1796, .adv_w = 182, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1812, .adv_w = 228, .box_w = 13, .box_h = 18, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1842, .adv_w = 187, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1856, .adv_w = 224, .box_w = 13, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1886, .adv_w = 187, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1900, .adv_w = 145, .box_w = 10, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1923, .adv_w = 201, .box_w = 12, .box_h = 18, .ofs_x = 1, .ofs_y = -7},
    {.bitmap_index = 1950, .adv_w = 228, .box_w = 14, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1982, .adv_w = 103, .box_w = 6, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1995, .adv_w = 103, .box_w = 4, .box_h = 24, .ofs_x = 1, .ofs_y = -7},
    {.bitmap_index = 2007, .adv_w = 210, .box_w = 13, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2037, .adv_w = 103, .box_w = 6, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2051, .adv_w = 345, .box_w = 22, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2084, .adv_w = 228, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2105, .adv_w = 228, .box_w = 12, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2122, .adv_w = 228, .box_w = 13, .box_h = 19, .ofs_x = 0, .ofs_y = -7},
    {.bitmap_index = 2153, .adv_w = 220, .box_w = 13, .box_h = 19, .ofs_x = 1, .ofs_y = -7},
    {.bitmap_index = 2184, .adv_w = 149, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2198, .adv_w = 164, .box_w = 8, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2209, .adv_w = 131, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2223, .adv_w = 220, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2241, .adv_w = 210, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2259, .adv_w = 299, .box_w = 19, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2286, .adv_w = 205, .box_w = 12, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2303, .adv_w = 187, .box_w = 12, .box_h = 18, .ofs_x = 0, .ofs_y = -7},
    {.bitmap_index = 2330, .adv_w = 191, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2345, .adv_w = 215, .box_w = 8, .box_h = 25, .ofs_x = 4, .ofs_y = -7},
    {.bitmap_index = 2370, .adv_w = 224, .box_w = 1, .box_h = 23, .ofs_x = 6, .ofs_y = -6},
    {.bitmap_index = 2373, .adv_w = 215, .box_w = 8, .box_h = 25, .ofs_x = 2, .ofs_y = -7},
    {.bitmap_index = 2398, .adv_w = 299, .box_w = 15, .box_h = 3, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 2404, .adv_w = 224, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 96, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_28 = {
#else
lv_font_t lv_font_28 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 26,          /*The maximum line height required by the font*/
    .base_line = 7,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -4,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_FONT_28*/

