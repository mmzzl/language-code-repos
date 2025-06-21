#include <Arduino.h>
#include "solar2lunar.h"
#include "zh_32.c"
#include "zh_20.c"
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <time.h>
#include "network.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "my_lv_ports.h"
#include "weather.h"
// 在文件顶部添加图片包含
#include "images/15/baoyu15X15.c"
#include "images/15/duoyun15X15.c"
#include "images/15/leiyu15X15.c"
#include "images/15/xiaoyu15X15.c"
#include "images/15/yin15X15.c"
#include "images/15/yu15X15.c"
#include "images/15/zhongyu15X15.c"
#include "images/15/unknow15X15.c"
#include "images/15/qing15X15.c"
#include "images/15/dayu15X15.c"

#include "images/duoyun.c"
#include "images/leiyu.c"
#include "images/yin.c"
#include "images/yu.c"
#include "images/unknow.c"
#include "images/qing.c"


#define MAX_TOKENS 3
#define DATE_LENGTH 16

// 全局对象实例
NetworkManager networkManager;
LunarCalendar lunarCalendar;
LVGLManager lvglManager;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60*60*8, 24 * 60 * 60 * 1000);

// UI结构体
typedef struct {
    lv_obj_t *time_label;
    lv_obj_t *date_label;
    lv_obj_t *weekday_label;
    lv_obj_t *lunar_label;
    lv_obj_t *lunar_term_label;
    lv_obj_t *weather_icon;
    lv_obj_t *weather_label;
    
    // 新增底部预报区域元素
    lv_obj_t *forecast_day_label[3];   // 三个日期标签
    lv_obj_t *forecast_weather_icon[3]; // 三个天气图标
    lv_obj_t *forecast_weather_label[3]; // 三个天气描述标签
} lv_clock_t;


static lv_clock_t lv_clock = {0};
static weather_t forecasts[3] = {0};  // 获取未来3天的天气
struct tm last_date = {0};


// 函数声明
void update_time_label(lv_timer_t *timer);
void check_date_and_update_weather(struct tm *current_time);
void check_time_and_update_weather(struct tm *current_time);
void processDate(weather_t *forecast);
void split_and_combine(char *date);
void create_ui();
void setup_ui_styles();
void align_ui_elements();
void setup_timer();

// 主程序入口
void setup() {
    Serial.begin(115200);
    pinMode(11, OUTPUT);
    digitalWrite(11, HIGH);
    // 网络初始化
    String topic = WiFi.macAddress().substring(8);
    topic.replace(":", "");
    networkManager.loadConfig();
    networkManager.apConfig(topic);
    networkManager.reconnectWiFi();
    
    // 时间服务初始化
    timeClient.begin();
    
    // LVGL初始化
    lv_init();
    lvglManager.init();
    
    // 创建UI界面
    create_ui();
    setup_ui_styles();
    align_ui_elements();
    
    // 设置定时器
    setup_timer();
    
    Serial.println("Setup done");
}

void loop() {
    lv_timer_handler();
    if (!WiFi.isConnected()) {
        networkManager.reconnectWiFi();
    }
    delay(5);
}


// UI创建函数
void create_ui() {
    // 1. 创建全屏背景
    // static lv_obj_t *img_bg = lv_img_create(lv_scr_act());
    // lv_img_set_src(img_bg, &caishengye);
    // lv_obj_set_size(img_bg, 240, 320);
    // lv_obj_align(img_bg, LV_ALIGN_TOP_LEFT, 0, 0);
    // lv_obj_move_background(img_bg);

    // 2. 顶部区域：当前天气
    lv_obj_t *weather_now = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(weather_now); // 透明背景
    lv_obj_set_size(weather_now, 320, 50);  // 高度减小
    lv_obj_align(weather_now, LV_ALIGN_TOP_MID, 0, 5);  // 上边距减小

    // 天气图标 - 设置固定大小
    lv_clock.weather_icon = lv_img_create(weather_now);
    lv_img_set_src(lv_clock.weather_icon, &qing);
    lv_obj_set_size(lv_clock.weather_icon, 40, 40);  // 固定图标大小
    lv_obj_align(lv_clock.weather_icon, LV_ALIGN_LEFT_MID, 10, 0);

    // 天气描述
    lv_clock.weather_label = LVGLManager::createLabel(weather_now, "晴");
    lv_obj_align_to(lv_clock.weather_label, lv_clock.weather_icon, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_text_font(lv_clock.weather_label, &zh_20, 0);

    // 3. 中部区域：日期时间
    lv_obj_t *datetime_area = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(datetime_area);
    lv_obj_set_size(datetime_area, 320, 100);  // 高度减小
    lv_obj_align(datetime_area, LV_ALIGN_CENTER, 0, -10);  // 上移10像素

    // 日期 (格式: 05/20)
    lv_clock.date_label = LVGLManager::createLabel(datetime_area, "05/20");
    lv_obj_align(lv_clock.date_label, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_text_font(lv_clock.date_label, &lv_font_28, 0);

    // 星期
    lv_clock.weekday_label = LVGLManager::createLabel(datetime_area, "周一");
    lv_obj_align_to(lv_clock.weekday_label, lv_clock.date_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);  // 间距减小
    lv_obj_set_style_text_font(lv_clock.weekday_label, &zh_20, 0);

    // 农历
    lv_clock.lunar_label = LVGLManager::createLabel(datetime_area, "闰四月廿四");
    lv_obj_align_to(lv_clock.lunar_label, lv_clock.date_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);  // 间距减小
    lv_obj_set_style_text_font(lv_clock.lunar_label, &zh_20, 0);

    // 时间 (格式: 14:30:45)
    lv_clock.time_label = LVGLManager::createLabel(datetime_area, "14:30:45");
    lv_obj_align_to(lv_clock.time_label, lv_clock.weekday_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);  // 间距减小
    lv_obj_set_style_text_font(lv_clock.time_label, &lv_font_montserrat_48, 0);


    // 4. 底部区域：未来三天预报
    lv_obj_t *forecast_area = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(forecast_area);
    lv_obj_set_size(forecast_area, 320, 60);
    lv_obj_align(forecast_area, LV_ALIGN_BOTTOM_MID, 0, -5);

    // 创建三个预报区块
    for (int i = 0; i < 3; i++) {
        lv_obj_t *day_box = lv_obj_create(forecast_area);
        lv_obj_remove_style_all(day_box);
        lv_obj_set_size(day_box, 100, 55);
        lv_obj_align(day_box, LV_ALIGN_LEFT_MID, 10 + i*105, 0);
        
        // 日期标签 - 存储到结构体中
        lv_clock.forecast_day_label[i] = lv_label_create(day_box);
        lv_label_set_text(lv_clock.forecast_day_label[i], "05/21");
        lv_obj_align(lv_clock.forecast_day_label[i], LV_ALIGN_TOP_MID, 0, 2);
        lv_obj_set_style_text_font(lv_clock.forecast_day_label[i], &zh_20, 0);
        
        // 天气图标 - 存储到结构体中
        lv_clock.forecast_weather_icon[i] = lv_img_create(day_box);
        lv_img_set_src(lv_clock.forecast_weather_icon[i], &qing);
        lv_obj_set_size(lv_clock.forecast_weather_icon[i], 15, 15);
        lv_obj_align(lv_clock.forecast_weather_icon[i], LV_ALIGN_CENTER, 0, 0);
        
        // 天气描述 - 存储到结构体中
        lv_clock.forecast_weather_label[i] = lv_label_create(day_box);
        lv_label_set_text(lv_clock.forecast_weather_label[i], "晴");
        lv_obj_align(lv_clock.forecast_weather_label[i], LV_ALIGN_BOTTOM_MID, 0, 2);
        lv_obj_set_style_text_font(lv_clock.forecast_weather_label[i], &zh_20, 0);
    }

}



// 设置UI样式
void setup_ui_styles() {
    // 全局背景样式
    static lv_style_t bg_style;
    lv_style_init(&bg_style);
    lv_style_set_bg_opa(&bg_style, LV_OPA_COVER);
    lv_style_set_bg_color(&bg_style, lv_color_black());
    lv_obj_add_style(lv_scr_act(), &bg_style, LV_STATE_DEFAULT);
    
    // 时间标签样式
    static lv_style_t time_style;
    lv_style_init(&time_style);
    lv_style_set_text_color(&time_style, lv_color_white());
    lv_style_set_text_font(&time_style, &lv_font_40);
    lv_obj_add_style(lv_clock.time_label, &time_style, LV_STATE_DEFAULT);
    
    // 日期标签样式
    static lv_style_t date_style;
    lv_style_init(&date_style);
    lv_style_set_text_color(&date_style, lv_color_white());
    lv_style_set_text_font(&date_style, &lv_font_28);
    lv_obj_add_style(lv_clock.date_label, &date_style, LV_STATE_DEFAULT);
    
    // 星期标签样式
    static lv_style_t weekday_style;
    lv_style_init(&weekday_style);
    lv_style_set_text_color(&weekday_style, lv_color_white());
    lv_style_set_text_font(&weekday_style, &zh_20);
    lv_obj_add_style(lv_clock.weekday_label, &weekday_style, LV_STATE_DEFAULT);

    // 农历标签样式
    static lv_style_t lunar_style;
    lv_style_init(&lunar_style);
    lv_style_set_text_color(&lunar_style, lv_color_white());
    lv_obj_add_style(lv_clock.lunar_label, &lunar_style, LV_STATE_DEFAULT);
    
    // 天气标签样式
    static lv_style_t weather_style;
    lv_style_init(&weather_style);
    lv_style_set_text_color(&weather_style, lv_color_white());
    lv_style_set_text_font(&weather_style, &zh_20);
    lv_obj_add_style(lv_clock.weather_label, &weather_style, LV_STATE_DEFAULT);
    
    // 预报区块样式
    static lv_style_t forecast_style;
    lv_style_init(&forecast_style);
    lv_style_set_text_color(&forecast_style, lv_color_white());
    lv_style_set_bg_opa(&forecast_style, LV_OPA_TRANSP);
    lv_style_set_border_width(&forecast_style, 0);
    
    // 应用预报区块样式
    lv_obj_t* forecast_area = lv_obj_get_child(lv_scr_act(), -1);
    if(forecast_area) {
        lv_obj_add_style(forecast_area, &forecast_style, LV_STATE_DEFAULT);
    }
}


// 对齐UI元素
void align_ui_elements() {
    // 顶部天气区域
    lv_obj_align(lv_clock.weather_icon, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_align_to(lv_clock.weather_label, lv_clock.weather_icon, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    
    // 中部时间区域
    lv_obj_align(lv_clock.date_label, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_align_to(lv_clock.weekday_label, lv_clock.date_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_align_to(lv_clock.time_label, lv_clock.weekday_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    
    // 底部预报区域
    lv_obj_t* forecast_area = lv_obj_get_child(lv_scr_act(), -1);
    if(forecast_area) {
        lv_obj_align(forecast_area, LV_ALIGN_BOTTOM_MID, 0, -5);
        
        // 对齐每个预报区块
        for(int i = 0; i < 3; i++) {
            lv_obj_t* day_box = lv_obj_get_child(forecast_area, i);
            if(day_box) {
                lv_obj_align(day_box, LV_ALIGN_LEFT_MID, 10 + i*105, 0);
                
                // 对齐区块内元素
                lv_obj_t* day_label = lv_obj_get_child(day_box, 0);
                lv_obj_t* weather_icon = lv_obj_get_child(day_box, 1);
                lv_obj_t* weather_label = lv_obj_get_child(day_box, 2);
                
                if(day_label) lv_obj_align(day_label, LV_ALIGN_TOP_MID, 0, 0);
                if(weather_icon) lv_obj_align(weather_icon, LV_ALIGN_CENTER, 0, 0);
                if(weather_label) lv_obj_align(weather_label, LV_ALIGN_BOTTOM_MID, 0, 2);
            }
        }
    }
}


// 设置定时器
void setup_timer() {
    // 创建定时器，每200ms更新一次时间
    lv_timer_t *timer = lv_timer_create(update_time_label, 200, &lv_clock);
    
    // 检查定时器是否创建成功
    if (!timer) {
        Serial.println("Error: Failed to create timer");
    }
}

void update_time_label(lv_timer_t *timer) {
    const char *weekday_names[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
    timeClient.update();
    
    // 获取当前时间
    time_t now = timeClient.getEpochTime();
    struct tm *local_time = localtime(&now);
    char time_str[20], year_str[5], month_str[3], day_str[3], full_time_str[50];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", local_time);
    strftime(year_str, sizeof(year_str), "%Y", local_time);
    strftime(month_str, sizeof(month_str), "%m", local_time);
    strftime(day_str, sizeof(day_str), "%d", local_time);
    snprintf(full_time_str, sizeof(full_time_str), "%s/%s", month_str, day_str);
    
    // 获取农历信息
    LunarCalendar::cnDate solar = lunarCalendar.solarToLunar(atoi(year_str), atoi(month_str), atoi(day_str));
    unsigned char solar_num = lunarCalendar.solarToTerm(atoi(year_str), atoi(month_str), atoi(day_str));
    LunarCalendar::cnLunarDate cnLunar = lunarCalendar.lunarToChinese(solar, solar_num);
    
    // 获取星期信息
    const char *weekday_str = weekday_names[local_time->tm_wday];
    
    // 更新天气信息
    check_time_and_update_weather(local_time);
    if (forecasts == nullptr) {
      Serial.println("Error: Failed to get weather");
      return;
    }
    const char *city_name = (forecasts[0].city[0] == '\0') ? "深圳市" : forecasts[0].city;

    // 更新UI元素
    if (timer != NULL && timer->user_data != NULL) {
        lv_clock_t *clock = (lv_clock_t *)(timer->user_data);
        
        if (clock->time_label) {
            lv_label_set_text(clock->time_label, time_str);
            lv_obj_align_to(clock->time_label, lv_obj_get_parent(clock->time_label), LV_ALIGN_BOTTOM_LEFT, 0, 5);
        }
        
        if (clock->weekday_label) {
            lv_label_set_text(clock->weekday_label, weekday_str);
            lv_obj_align_to(clock->weekday_label, lv_obj_get_parent(clock->weekday_label), LV_ALIGN_TOP_RIGHT, 0, 0);
        }
        
        if (clock->date_label) {
            lv_label_set_text(clock->date_label, full_time_str);
            lv_obj_align_to(clock->date_label, lv_obj_get_parent(clock->date_label), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        }
        
        if (clock->lunar_label) {
            // 在 update_time_label 函数中修改
            String lunarText = cnLunar.cn_month + cnLunar.cn_date;
            lv_label_set_text(clock->lunar_label, lunarText.c_str());
            lv_obj_align_to(clock->lunar_label, lv_obj_get_parent(clock->lunar_label), LV_ALIGN_TOP_LEFT, 0, 0);
        }
        
        if (clock->lunar_term_label) {
            lv_label_set_text(clock->lunar_term_label, cnLunar.cn_term.c_str());
            lv_obj_align_to(clock->lunar_term_label, lv_obj_get_parent(clock->lunar_term_label), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        }

        // 在 update_time_label 函数中修改顶部天气图标部分
        if (clock->weather_icon) {
            // 直接使用顶部天气图标对象
            const char *weather = forecasts[0].dayweather;
            if (strstr(weather, "晴")) {
                lv_img_set_src(clock->weather_icon, &qing);
            } else if (strstr(weather, "多云")) {
                lv_img_set_src(clock->weather_icon, &duoyun);
            } else if (strstr(weather, "阴")) {
                lv_img_set_src(clock->weather_icon, &yin);
            } else if (strstr(weather, "雨")) {
                lv_img_set_src(clock->weather_icon, &yu);
            } else if (strstr(weather, "雷")) {
                lv_img_set_src(clock->weather_icon, &leiyu);
            } else {
                lv_img_set_src(clock->weather_icon, &unknow);
            }
        }

        
        if (clock->weather_label) {
            char weather_str[256];
            snprintf(weather_str, sizeof(weather_str), "%s\n%s %s\n",
                     city_name,
                     forecasts[0].date, forecasts[0].dayweather);
             // 天气图标 - 存储到结构体中
              
            lv_label_set_text(clock->weather_label, weather_str);
            lv_obj_align_to(clock->weather_label, lv_obj_get_parent(clock->weather_label), LV_ALIGN_TOP_LEFT, 60, 5);
            // 更新底部预报区域
            for (int i = 0; i < 3; i++) {
                // 确保forecasts数组有数据
                if (lv_clock.forecast_day_label[i]) {
                  // 创建日期副本进行处理（避免修改原始数据）
                      char date_copy[16];
                      strncpy(date_copy, forecasts[i].date, sizeof(date_copy));
                      date_copy[sizeof(date_copy)-1] = '\0'; // 确保终止符
                      // 定义 date_str 变量
                      char date_str[16] = {0}; // ✅ 添加这行
                      // 使用临时指针处理副本
                      char *token = strtok(date_copy, "-");
                      if (token) { // 跳过年份
                          token = strtok(NULL, "-"); // 月份
                          char *month = token;
                          token = strtok(NULL, "-"); // 日
                          if (token) {
                              char day[3] = {0};
                              strncpy(day, token, 2); // 取日前两位
                              snprintf(date_str, sizeof(date_str), "%s/%s", month, day);
                              lv_label_set_text(lv_clock.forecast_day_label[i], date_str);
                          }
                      }
                    }
                    
                    // 更新天气图标
                    if (lv_clock.forecast_weather_icon[i]) {
                        // 根据天气描述选择图标
                        const char *weather = forecasts[i].dayweather;
                        if (strstr(weather, "晴")) {
                            lv_img_set_src(lv_clock.forecast_weather_icon[i], &qing15X15);
                        } else if (strstr(weather, "多云")) {
                            lv_img_set_src(lv_clock.forecast_weather_icon[i], &duoyun15X15);
                        } else if (strstr(weather, "阴")) {
                            lv_img_set_src(lv_clock.forecast_weather_icon[i], &yin15X15);
                        } else if (strstr(weather, "雨")) {
                            if (strstr(weather, "小雨")) {
                                lv_img_set_src(lv_clock.forecast_weather_icon[i], &xiaoyu15X15);
                            } else if (strstr(weather, "中雨")) {
                                lv_img_set_src(lv_clock.forecast_weather_icon[i], &zhongyu15X15);
                            } else if (strstr(weather, "大雨")) {
                                lv_img_set_src(lv_clock.forecast_weather_icon[i], &dayu15X15);
                            } else if (strstr(weather, "暴雨")) {
                                lv_img_set_src(lv_clock.forecast_weather_icon[i], &baoyu15X15);
                            } else {
                                lv_img_set_src(lv_clock.forecast_weather_icon[i], &yu15X15);
                            }
                        } else if (strstr(weather, "雷")) {
                            lv_img_set_src(lv_clock.forecast_weather_icon[i], &leiyu15X15);
                        } else {
                            lv_img_set_src(lv_clock.forecast_weather_icon[i], &unknow15X15);
                        }
                    }
                    
                    // 更新天气描述
                    if (lv_clock.forecast_weather_label[i]) {
                        lv_label_set_text(lv_clock.forecast_weather_label[i], forecasts[i].dayweather);
                    }
                }
          }

        }
    
}

// 检测日期变化并更新天气
void check_date_and_update_weather(struct tm *current_time) {
  // 比较当前日期和上一次的日期
  if (current_time->tm_year != last_date.tm_year ||
      current_time->tm_mon != last_date.tm_mon ||
      current_time->tm_mday != last_date.tm_mday) {
      getWeather(forecasts);
      last_date = *current_time;  // 更新上一次的日期
  }
}

// 检测是否达到两小时并更新天气
void check_time_and_update_weather(struct tm *current_time) {
  static time_t last_update_time = 0;
  time_t current_timestamp = mktime(current_time);
  double time_diff = difftime(current_timestamp, last_update_time);

  if (last_update_time == 0 || time_diff >= 7200) {
      getWeather(forecasts);
      last_update_time = current_timestamp;
  }
}

void split_and_combine(char *date) {
  char *tokens[MAX_TOKENS];
  int token_count = 0;
  char *token = strtok(date, "-");

  // 分割字符串
  while (token != NULL && token_count < MAX_TOKENS) {
      tokens[token_count++] = token;
      token = strtok(NULL, "-");
  }

  // 重新组合字符串，跳过第一个部分
  if (token_count > 1) {
      char new_date[DATE_LENGTH] = "";
      for (int i = 1; i < token_count; i++) {
          if (i > 1) {
              // 检查空间是否足够，这里简单重新初始化new_date，更严谨做法可动态分配内存
              if (strlen(new_date)+strlen(tokens[i]) + 2 > DATE_LENGTH) {
                  break;
              }
              strncat(new_date, "-", sizeof(new_date) - strlen(new_date) - 1);
          }
          strncat(new_date, tokens[i], sizeof(new_date) - strlen(new_date) - 1);
      }
      strcpy(date, new_date);
  }
}

void processDate(weather_t *forecast) {
  split_and_combine(forecast->date);
}


