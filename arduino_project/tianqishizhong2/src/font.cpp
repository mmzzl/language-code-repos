#include "font.h"


void drawChineseString(TFT_eSPI *tft, font_t *font, bool is_change) {
    if (is_change) {
       tft->fillRect(font->x - 28, font->y - 10, font->text.length() * font->size + (28 * 3), font->size + 10 , TFT_BLACK);
    }
    switch (font->size) {
        // case 16:
        //     tft->loadFont(msyh16);
        //     break;
        case 20:
            tft->loadFont(msyh20);
            break;
        case 28:
            tft->loadFont(msyh28);
            break;
        // case 40:
        //     tft->loadFont(msyh40);
        //     break;
        default:
            tft->loadFont(msyh20);
            break;
    }
    tft->setTextColor(font->color, TFT_BLACK);
    tft->drawString(font->text, font->x, font->y, font->size);
    tft->unloadFont();
}