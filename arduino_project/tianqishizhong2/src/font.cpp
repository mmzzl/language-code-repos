#include "font.h"

bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

void drawString(TFT_eSPI *tft, font_t *font) {
    switch (font->size) {
        case 16:
            tft->loadFont(msyh16);
            break;
        case 20:
            tft->loadFont(msyh20);
            break;
        case 28:
            tft->loadFont(msyh28);
            break;
        case 40:
            tft->loadFont(msyh40);
            break;
        default:
            tft->loadFont(msyh16);
            break;
    }
    tft->setTextColor(font->color);
    tft->drawString(font->text, font->x, font->y, font->size);
    tft->unloadFont();
}