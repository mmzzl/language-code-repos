#include "font.h"


// 定义全局变量来缓存已加载的字体
int currentFontSize = 0;

void drawChineseString(TFT_eSPI *tft, font_t *font, bool is_change) {
    // 仅在必要时填充矩形
    if (is_change) {
        tft->fillRect(font->x - 28, font->y - 10, font->text.length() * font->size + (28 * 3), font->size + 10, TFT_BLACK);
    }

    // 检查是否需要加载新字体
    if (currentFontSize != font->size) {
        switch (font->size) {
            case 20:
                tft->loadFont(msyh20);
                break;
            case 28:
                tft->loadFont(STSONG28);
                break;
            case 75:
                tft->loadFont(STSONG75);
                break;
            default:
                tft->loadFont(msyh20);
                break;
        }
        currentFontSize = font->size;
    }

    // 设置文本颜色
    tft->setTextColor(font->color, TFT_BLACK);

    // 绘制字符串
    tft->drawString(font->text, font->x, font->y, font->size);

    // 不再每次都卸载字体，避免频繁操作
    // tft->unloadFont();
}