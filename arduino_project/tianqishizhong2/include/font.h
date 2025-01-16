#ifndef FONT_H
#define FONT_H
#include <Arduino.h>
#include <TFT_eSPI.h>

// #include "fonts/msyh16.h"
#include "fonts/msyh20.h"
#include "fonts/msyh28.h"
// #include "fonts/msyh40.h"
typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t size;
    uint32_t color;
    String  text;

} font_t;

void drawChineseString(TFT_eSPI *tft, font_t *font, bool is_change);

#endif
