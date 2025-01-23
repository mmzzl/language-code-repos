#include "gui.h"
#include "weather.h"
#include <TJpg_Decoder.h>




const std::map<std::string, const uint16_t*> weatherIcons = {
        {"晴", qing},
        {"雷阵雨", leizhenyu},
        {"阴", yin},
        {"阵雨", zhenyu},
        {"晴间多云", qingjianduoyun},
        {"大暴雨", dabaoyu},
        {"暴雨", baoyu},
        {"大雨", dayu},
        {"中雨", zhongyu},
        {"小雨", xiaoyu},
        {"多云", duoyun}
};

TFT_eSPI tft = TFT_eSPI();
weather_t forecasts[3];
uint8_t omm = 99;
uint8_t oss = 99;
uint8_t xcolon = 0;
String old_date;
String old_lunar_date;
int old_temperature;
String old_week_name;
bool is_change = false;
unsigned long start_time = 0;
String old_solar_term;
String old_city;
String old_today_weather;
String old_tomorrow_weather;
uint8_t Animate_key = -1; // 初始化图标显示帧数
int Amimate_reflash_Time = 0;  // 动画更新时间计数

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   if (y >= tft.height()) return 0;
    tft.pushImage(x, y, w, h, bitmap);
   return 1;
}

//从动图数组里面获取一帧图像数据
void imgAnim(const uint8_t **Animate_value, uint32_t *Animate_size){
     Animate_key++;
    *Animate_value = astronaut[Animate_key];
    *Animate_size = astronaut_size[Animate_key];
    if (Animate_key >= (sizeof(astronaut)/sizeof(astronaut[0])-1))  //gif动图帧数
            Animate_key = -1;
}

void showTaikongRen() {
     // 显示太空人
  const uint8_t * Animate_value;
  uint32_t Animate_size; // 指向关键帧大小的指针
  if(millis() - Amimate_reflash_Time > 2) {
      Amimate_reflash_Time = millis();
      imgAnim(&Animate_value, &Animate_size);
      TJpgDec.drawJpg(180, 176, Animate_value, Animate_size);
  }
}


void tft_init() {
    // TFT屏幕初始化
  tft.init();
  tft.setRotation(1);
  tft.invertDisplay(1);
  tft.setWindow(0, 0, tft.width() - 1, tft.height() - 1); // 设置显示窗口
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
  tft.fillScreen(TFT_BLACK);
}

void showWeatherIcon(const char* weatherCondition)
{
    std::string str(weatherCondition);
    auto it = weatherIcons.find(str.c_str());
        if (it != weatherIcons.end()) {
            tft.pushImage(176, 86, 48, 48, it->second, 16);
        } else {
            // 未找到对应的天气图标，使用默认图标
            Serial.println(weatherCondition);
            tft.pushImage(176, 86, 48, 48, tianqi, 16);
        }
}


void main_gui() {
    data_t time_t;
    get_data(&time_t);
    int xpos = 0;
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
    ptr = font_t{0, 153, 28, TFT_WHITE, date_str};
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
    ptr = font_t{0, 20, 28, TFT_YELLOW, time_t.lunar_date};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示星期
    if (old_week_name != time_t.week_name) {
        old_week_name = time_t.week_name;
        is_change = true;
    }
    ptr = font_t{184, 153, 28, TFT_RED, time_t.week_name};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示温度
    int temperature = get_temperature();
    if (old_temperature != temperature) {
        old_temperature = temperature;
        is_change = true;
    }
    if (temperature > 0) {
        ptr = font_t{184, 58, 28, TFT_GREEN, String(temperature) + "℃"};
    } else {
        ptr = font_t{156, 58, 28, TFT_RED, String(temperature) + "℃"};
    }

    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示节气
    if (old_solar_term != time_t.solar_term) {
        old_solar_term = time_t.solar_term;
        is_change = true;
    }
    if (time_t.solar_term.length() < 3) {
        ptr = font_t{184, 20, 28, TFT_GREEN, time_t.solar_term};
    } else {
        ptr = font_t{156, 20, 28, TFT_GREEN, time_t.solar_term};
    }
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示天气
    if ((start_time>0) && ((millis() - start_time) >= (3 * 60 * 60 * 1000))) {
        //3 小时更新一次天气
        start_time = millis();
        network_init();
        getWeather(forecasts);
        // 更新完后断开网络
        disconnectWiFi();
    };
    if (forecasts[0].dayweather == NULL) {
        getWeather(forecasts);
        start_time = millis();
    }
    
    // 显示天气
    if (old_today_weather != forecasts[0].dayweather) {
        old_today_weather = forecasts[0].dayweather;
        is_change = true;
    }
    ptr = font_t{90, 58, 28, TFT_GREEN, forecasts[0].dayweather};
    drawChineseString(&tft, &ptr, is_change);
    // 显示天气图标
    showWeatherIcon(forecasts[0].dayweather.c_str());
    is_change = false;
    // 显示地区
    if (old_city != forecasts[0].city) {
        old_city = forecasts[0].city;
        is_change = true;
    }
    ptr = font_t{0, 58, 28, TFT_GREEN, forecasts[0].city};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 明日天气
    if (old_tomorrow_weather != forecasts[1].dayweather) {
        old_tomorrow_weather = forecasts[1].dayweather;
        is_change = true;
    }
    ptr = font_t{0, 202, 28, TFT_GREEN, "明日 " + forecasts[1].dayweather};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    if (millis() -  start_time == 60) {
        disconnectWiFi();
    }
    // 显示天空人
    showTaikongRen();
}

