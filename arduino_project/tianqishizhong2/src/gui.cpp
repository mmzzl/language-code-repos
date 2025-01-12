#include "gui.h"

TFT_eSPI tft = TFT_eSPI();

uint8_t omm = 99;
uint8_t oss = 99;
uint8_t xcolon = 0;
String old_date;
String old_lunar_date;
int old_temperature;
String old_week_name;
bool is_change = false;

void tft_init() {
    // TFT屏幕初始化
  tft.init();
  tft.setRotation(1);
  tft.setWindow(0, 0, tft.width() - 1, tft.height() - 1); // 设置显示窗口
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
}

void main_gui() {
    data_t time_t;
    get_data(&time_t);
    int xpos = 56;
    // int xpos = 0;
    int ypos = 96;
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    if (omm != time_t.minute) {
        omm = time_t.minute;
        if (time_t.hour < 10) {
            xpos += tft.drawChar('0', xpos, ypos, 6);
        }
        xpos += tft.drawNumber(time_t.hour, xpos, ypos, 6);
        xcolon = xpos;
        xpos += tft.drawChar(':', xpos, ypos , 6);
        if (time_t.minute < 10) {
            xpos += tft.drawChar('0', xpos, ypos, 6);
        }
        xpos += tft.drawNumber(time_t.minute, xpos, ypos, 6);
    }
    if (oss != time_t.second) {
        oss = time_t.second;
        if (time_t.second % 2) {
            tft.setTextColor(0x39c4, TFT_BLACK);
            tft.drawChar(':', xcolon, ypos, 6);
        } else {
            tft.drawChar(':', xcolon, ypos, 6);
        }
    }
    // 显示日期
    font_t  ptr;
    String month;
    if (time_t.month < 10) {
        month = "0" + String(time_t.month);
    } else {
        month = String(time_t.month);
    }
    String day;
    if (time_t.day < 10) {
        day = "0" + String(time_t.day);
    } else {
        day = String(time_t.day);
    }
    String date_str = month + "月" + day + "日";
    ptr = font_t{0, 20, 28, TFT_WHITE, date_str};
    if (old_date != date_str) {
        old_date = date_str;
        is_change = true;
    }
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示农历日期
    if (old_lunar_date != time_t.lunar_date) {
        old_lunar_date = time_t.lunar_date;
        is_change = true;
    }
    ptr = font_t{112, 20, 28, TFT_YELLOW, time_t.lunar_date};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示星期
    if (old_week_name != time_t.week_name) {
        old_week_name = time_t.week_name;
        is_change = true;
    }
    ptr = font_t{0, 212, 28, TFT_RED, time_t.week_name};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示温度
    int temperature = get_temperature();
    if (old_temperature != temperature) {
        old_temperature = temperature;
        is_change = true;
    }
    ptr = font_t{50, 212, 28, TFT_GREEN, String(temperature) + "℃"};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    Serial.println(date_str);
    Serial.println(time_t.lunar_date);
}

