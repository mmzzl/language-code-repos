#include "gui.h"
#include "weather.h"
#include <TJpg_Decoder.h>




// 使用数组替代 std::map
struct WeatherIcon {
    const char* condition;
    const uint16_t* icon;
};

const WeatherIcon weatherIcons[] = {
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
static weather_t forecasts[4];
uint8_t omm = 99;
uint8_t oss = 99;
uint8_t xcolon = 0;
String old_date;
char old_lunar_date[50] = "";  // 使用数组替代 char*
int old_temperature;
char old_week_name[20] = "";  // 使用数组替代 char*
static unsigned long start_time = 0;
bool is_change = false;
char old_solar_term[50] = "";  // 使用数组替代 char*
char old_city[50] = "";  // 使用数组替代 char*
char old_today_weather[50] = "";  // 使用数组替代 char*
char old_tomorrow_weather[50] = "";  // 使用数组替代 char*
uint8_t Animate_key = -1; // 初始化图标显示帧数
int Amimate_reflash_Time = 0;  // 动画更新时间计数
// 用于标记是否已经断开网络
bool isNetworkDisconnected = false;
char hour_str[3];
char minute_str[3];

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
    if (Animate_key >= (sizeof(astronaut)/sizeof(astronaut[0]) - 1))  //gif动图帧数
        Animate_key = -1;
}

void showTaikongRen() {
    // 显示太空人
    const uint8_t * Animate_value;
    uint32_t Animate_size; // 指向关键帧大小的指针
    if(millis() - Amimate_reflash_Time > 2) {
        Amimate_reflash_Time = millis();
        imgAnim(&Animate_value, &Animate_size);
        TJpgDec.drawJpg(192, 186, Animate_value, Animate_size);
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
    for (size_t i = 0; i < sizeof(weatherIcons) / sizeof(weatherIcons[0]); i++) {
        if (strcmp(weatherCondition, weatherIcons[i].condition) == 0) {
            tft.pushImage(176, 86, 48, 48, weatherIcons[i].icon, 16);
            return;
        }
    }
    // 未找到对应的天气图标，使用默认图标
    tft.pushImage(176, 86, 48, 48, tianqi, 16);
}

void main_gui() {
    font_t  ptr;
    data_t time_t;
    get_data(&time_t);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    if (omm != time_t.minute) {
        omm = time_t.minute;
        if (time_t.hour < 10) {
            snprintf(hour_str, sizeof(hour_str), "0%d", time_t.hour);
        } else {
            snprintf(hour_str, sizeof(hour_str), "%d", time_t.hour);
        }
        ptr = font_t{0, 86, 75, TFT_YELLOW, hour_str};
        drawChineseString(&tft, &ptr, true);
        if (time_t.minute < 10) {
            snprintf(minute_str, sizeof(minute_str), "0%d", time_t.minute);
        } else {
            snprintf(minute_str, sizeof(minute_str), "%d", time_t.minute);
        }
        ptr = font_t{100, 86, 75, TFT_YELLOW, minute_str};
        drawChineseString(&tft, &ptr, true);
    }

    if (oss != time_t.second) {
        oss = time_t.second;
        if (time_t.second % 2) {
            ptr = font_t{75, 76, 75, 0x39c4, ":"};
        } else {
            ptr = font_t{75, 76, 75, TFT_YELLOW, ":"};
        }
        drawChineseString(&tft, &ptr, is_change);
    }
    // 显示日期
    char date_str[20];
    snprintf(date_str, sizeof(date_str), "%02d月%02d日", time_t.month, time_t.day);
    ptr = font_t{0, 153, 28, TFT_WHITE, date_str};
    if (old_date != date_str) {
        old_date = date_str;
        is_change = true;
    }
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示农历日期
    if (strcmp(old_lunar_date, time_t.lunar_date) != 0) {
        strcpy(old_lunar_date, time_t.lunar_date);
        is_change = true;
    }
    ptr = font_t{0, 20, 28, TFT_YELLOW, time_t.lunar_date};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示星期
    if (strcmp(old_week_name, time_t.week_name) != 0) {
        strcpy(old_week_name, time_t.week_name);
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
    char temp_str[20]; // 增大缓冲区大小
    if (temperature > 0) {
        snprintf(temp_str, sizeof(temp_str), "%d℃", temperature);
        ptr = font_t{184, 58, 28, TFT_GREEN, temp_str};
    } else {
        snprintf(temp_str, sizeof(temp_str), "%d℃", temperature);
        ptr = font_t{156, 58, 28, TFT_RED, temp_str};
    }

    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示节气
    const char* nextTermName;
    int days = daysToNextSolarTerm(time_t.year, time_t.month, time_t.day, &nextTermName);
    char combinedStr[50];  // 足够存储拼接后的字符串
    if (days == 0) {
        strcpy(combinedStr, time_t.solar_term);
    } else {
        snprintf(combinedStr, sizeof(combinedStr), "即将%s", nextTermName);
    }
    if (strcmp(old_solar_term, combinedStr) != 0) {
        strcpy(old_solar_term, combinedStr);
        is_change = true;
    }
    ptr = font_t{128, 20, 28, TFT_WHITE, combinedStr};
    drawChineseString(&tft, &ptr, is_change);
    // Serial.println("距离下个节气还有" + String(days) + "天");
    // Serial.println(nextTermName);
    is_change = false;
    // 显示天气
    if (millis() - start_time >= 3 * 60 * 60 * 1000) {
        //3 小时更新一次天气
        Serial.println("开始更新天气");
        start_time = millis();
        getWeather(forecasts);
        realtimeWeather(forecasts);
    };
    if (strlen(forecasts[0].dayweather) == 0) {
        getWeather(forecasts);
        realtimeWeather(forecasts);
        start_time = millis();
    }
    // 显示天气
    if (strcmp(old_today_weather, forecasts[0].realtimeWeather) != 0) {
        strcpy(old_today_weather, forecasts[0].realtimeWeather);
        is_change = true;  
    }
    ptr = font_t{90, 58, 28, TFT_RED, forecasts[0].realtimeWeather};
    drawChineseString(&tft, &ptr, true);
    // 显示天气图标
    showWeatherIcon(forecasts[0].realtimeWeather);
    is_change = false;
    // 显示地区
    if (strcmp(old_city, forecasts[0].city) != 0) {
        strcpy(old_city, forecasts[0].city);
        is_change = true;
    }
    ptr = font_t{0, 58, 28, TFT_GREEN, forecasts[0].city};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 明日天气
    if (strcmp(old_tomorrow_weather, forecasts[1].dayweather) != 0) {
        strcpy(old_tomorrow_weather, forecasts[1].dayweather);
        is_change = true;
    }
    char tomato_str[50];
    snprintf(tomato_str, sizeof(tomato_str), "明日 %s", forecasts[1].dayweather);
    ptr = font_t{0, 202, 28, TFT_YELLOW, tomato_str};
    drawChineseString(&tft, &ptr, is_change);
    is_change = false;
    // 显示天空人
    showTaikongRen();
}

