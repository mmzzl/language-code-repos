#include <Arduino.h>
// #include "zh_28.h"
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
#include "caishengye.h"
#define RND_NUM         64
static uint32_t rnd_act;
// #include "lv_demo.h"

LV_IMG_DECLARE(caishengye);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,30*60*1000);

typedef struct _lv_clock
{
    lv_obj_t *time_label; // 时间标签
    lv_obj_t *date_label; // 日期标签
    lv_obj_t *weekday_label; // 星期标签
    lv_obj_t *lunar_label; // 农历标签
    lv_obj_t *lunar_term_label; //节气标签
    lv_obj_t *weather_icon; // 天气图标
    lv_obj_t *weather_label; // 天气标签
}lv_clock_t;

static const uint32_t rnd_map[] = {
  0xbd13204f, 0x67d8167f, 0x20211c99, 0xb0a7cc05,
  0x06d5c703, 0xeafb01a7, 0xd0473b5c, 0xc999aaa2,
  0x86f9d5d9, 0x294bdb29, 0x12a3c207, 0x78914d14,
  0x10a30006, 0x6134c7db, 0x194443af, 0x142d1099,
  0x376292d5, 0x20f433c5, 0x074d2a59, 0x4e74c293,
  0x072a0810, 0xdd0f136d, 0x5cca6dbc, 0x623bfdd8,
  0xb645eb2f, 0xbe50894a, 0xc9b56717, 0xe0f912c8,
  0x4f6b5e24, 0xfe44b128, 0xe12d57a8, 0x9b15c9cc,
  0xab2ae1d3, 0xb4dc5074, 0x67d457c8, 0x8e46b00c,
  0xa29a1871, 0xcee40332, 0x80f93aa1, 0x85286096,
  0x09bd6b49, 0x95072088, 0x2093924b, 0x6a27328f,
  0xa796079b, 0xc3b488bc, 0xe29bcce0, 0x07048a4c,
  0x7d81bd99, 0x27aacb30, 0x44fc7a0e, 0xa2382241,
  0x8357a17d, 0x97e9c9cc, 0xad10ff52, 0x9923fc5c,
  0x8f2c840a, 0x20356ba2, 0x7997a677, 0x9a7f1800,
  0x35c7562b, 0xd901fe51, 0x8f4e053d, 0xa5b94923,
};


static lv_clock_t lv_clock = { 0 };

static weather_t forecasts[3] = { 0 };  // 获取未来3天的天气
struct tm last_date = {0};

void update_time_label(lv_timer_t *timer);
void reconnectWiFi();
void check_date_and_update_weather(struct tm *current_time);
void check_time_and_update_weather(struct tm *current_time);
static int32_t rnd_next(int32_t min, int32_t max);
void processDate(weather_t *forecast);
#define MAX_TOKENS 3
#define DATE_LENGTH 16
#define FULL_BRIGHTNESS 255
#define LOW_BRIGHTNESS  30  // 降低亮度（0~255）

// put function declarations here:
// 更新标签文本的函数
// 更新标签文本的函数
void update_time_label(lv_timer_t *timer) {
  const char *weekday_names[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
  timeClient.update();
  // 获取当前时间戳
  time_t now = timeClient.getEpochTime();
  struct tm * local_time = localtime(&now);
  char time_str[20];
  char year_str[5];
  char month_str[3];
  char day_str[3];
  char full_time_str[50];
  char weather_str[50];
  const char *weekday_str;
  const char *city_name;
  strftime(time_str, sizeof(time_str), "%H:%M:%S", local_time);
  strftime(year_str, sizeof(year_str), "%Y", local_time);
  strftime(month_str, sizeof(month_str), "%m", local_time);
  strftime(day_str, sizeof(day_str), "%d", local_time);
  snprintf(full_time_str, sizeof(full_time_str), "%s/%s", month_str, day_str);
  cnDate solar = Solar2Lunar(atoi(year_str), atoi(month_str), atoi(day_str));
  uint32_t lunar_dd = solar.lunarDay;
  uint32_t lunar_mm = solar.lunarMonth;
  bool is_leap_month = solar.isLeapMonth;
  char *lunar_date = convertChineseString(lunar_mm, lunar_dd, is_leap_month);
  unsigned char  solar_num = Solar2Term(atoi(year_str), atoi(month_str), atoi(day_str));
  char *solar_term = convertSolarString(solar_num);
  weekday_str = weekday_names[local_time->tm_wday];
  // check_date_and_update_weather(local_time);
  check_time_and_update_weather(local_time);
  if (forecasts[0].city[0] == '\0')
  {
    city_name = "深圳市";
  } else {
    city_name = forecasts[0].city;
  }

  if (timer != NULL && timer->user_data != NULL) 
  {
    lv_clock_t * clock = (lv_clock_t *)(timer->user_data);
    if (clock->time_label != NULL)
    {
      lv_label_set_text(clock->time_label, time_str);
      lv_obj_align_to(clock->time_label, lv_obj_get_parent(clock->time_label), LV_ALIGN_BOTTOM_LEFT, 0, 5);
    }
    if (clock->weekday_label != NULL)
    {
      lv_label_set_text(clock->weekday_label, weekday_str);
      lv_obj_align_to(clock->weekday_label, lv_obj_get_parent(clock->weekday_label), LV_ALIGN_TOP_RIGHT, 0, 0);
    }
    if (clock->date_label != NULL)
    {
      lv_label_set_text(clock->date_label, full_time_str);
      lv_obj_align_to(clock->date_label, lv_obj_get_parent(clock->date_label), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    }
    if (clock->lunar_label != NULL)
    {
      lv_label_set_text(clock->lunar_label, lunar_date);
      lv_obj_align_to(clock->lunar_label, lv_obj_get_parent(clock->lunar_label), LV_ALIGN_TOP_LEFT, 0, 0);
    }
    if (clock->lunar_term_label != NULL)
    {
      lv_label_set_text(clock->lunar_term_label, solar_term);
      lv_obj_align_to(clock->lunar_term_label, lv_obj_get_parent(clock->lunar_term_label), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    }
    if (clock->weather_label != NULL) 
    {
      char weather_str[256] = {0};  // 用于存储拼接后的字符串
      // 先显示城市名
      snprintf(weather_str, sizeof(weather_str), "%s\n", city_name);
  
      // 拼接三天的天气信息
      for (int i = 0; i < 3; i++) {
          char temp_str[128];
          // processDate(&forecasts[i]);
          snprintf(temp_str, sizeof(temp_str), "%s %s\n", forecasts[i].date, forecasts[i].dayweather);
          strncat(weather_str, temp_str, sizeof(weather_str) - strlen(weather_str) - 1);
      }
      // Serial.printf(weather_str);
      // 设置标签文本
      lv_label_set_text(clock->weather_label, weather_str);
      // 对齐标签
      lv_obj_align_to(clock->weather_label, lv_obj_get_parent(clock->weather_label), LV_ALIGN_TOP_LEFT, 0, 0);
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

static int32_t rnd_next(int32_t min, int32_t max)
{
    if(min == max)
        return min;

    if(min > max) {
        int32_t t = min;
        min = max;
        max = t;
    }

    int32_t d = max - min;
    int32_t r = (rnd_map[rnd_act] % d) + min;

    rnd_act++;
    if(rnd_act >= RND_NUM) rnd_act = 0;

    return r;

}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, FULL_BRIGHTNESS);
  // 初始化看门狗定时器
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();
  String topic = WiFi.macAddress().substring(8); // 取mac地址做主题用
  topic.replace(":", ""); // 去掉:号
  loadConfig();//加载存储的数据
  apConfig(topic);//加载ap
  reconnectWiFi(); // 初始连接WiFi
  timeClient.begin();
  lv_init();
  my_disp_init();
  // lv_demo_widgets(); 
  // lv_demo_music(); 
  // lv_demo_benchmark();
  // lv_png_init();
 
  lv_obj_t *screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, 320, 240);
  // lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);
  // lv_obj_set_style_bg_opa(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  
  static lv_style_t bg_style;
  lv_style_reset(&bg_style);
  lv_style_init(&bg_style);
  lv_style_set_radius(&bg_style, 5);
  lv_style_set_bg_opa(&bg_style, LV_OPA_20);
  lv_style_set_border_width(&bg_style, 0);
  lv_style_set_bg_color(&bg_style, lv_palette_darken(LV_PALETTE_BLUE, 4));
  

  lv_obj_t *bg_obj = lv_obj_create(screen);
  if (bg_obj == NULL)
  {
      printf("[%s:%d] bg_obj create failed\n", __FUNCTION__, __LINE__);
      return;
  }
  lv_obj_set_size(bg_obj, 320, 240);
  lv_obj_center(bg_obj);
  lv_obj_add_style(bg_obj, &bg_style, LV_STATE_DEFAULT); //给time_date_obj对象添加样式

  lv_obj_t *time_obj = lv_obj_create(bg_obj);
  lv_obj_set_scrollbar_mode(time_obj, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_size(time_obj, 320, 110); // 设置对象大小
  lv_obj_align_to(time_obj, bg_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
  // lv_coord_t current_y = lv_obj_get_y(time_obj);
  // // 将对象向下偏移 5 个像素
  // lv_obj_set_y(time_obj, current_y + 20);

  // lv_obj_align_to(bg_obj, time_obj, LV_ALIGN_CENTER, 0, 10); // 对象居屏幕中间显示
  if (time_obj == NULL)
  {
      printf("[%s:%d] time_obj create failed\n", __FUNCTION__, __LINE__);
      return;
  }
  static lv_style_t time_obj_style;
  lv_style_reset(&time_obj_style);
  lv_style_init(&time_obj_style);
  lv_style_set_bg_opa(&time_obj_style, LV_OPA_20);
  lv_style_set_bg_color(&time_obj_style, lv_palette_darken(LV_PALETTE_PURPLE, 4));
  lv_obj_add_style(time_obj, &time_obj_style,  LV_STATE_DEFAULT);


  static lv_style_t time_style;
  lv_style_reset(&time_style);
  lv_style_init(&time_style);
  lv_style_set_bg_opa(&time_style, LV_OPA_TRANSP);
  lv_style_set_text_font(&time_style, &lv_font_40);
  lv_style_set_text_color(&time_style, lv_color_white());

  // 添加label
  lv_clock.time_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.time_label, &time_style, LV_STATE_DEFAULT);

  // lunal style
  static lv_style_t lunar_style;
  lv_style_reset(&lunar_style);
  lv_style_init(&lunar_style);
  lv_style_set_bg_opa(&lunar_style, LV_OPA_TRANSP);
  lv_style_set_text_color(&lunar_style, lv_color_white());
  lv_style_set_text_font(&lunar_style, &zh_32);

  lv_clock.lunar_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.lunar_label, &lunar_style, LV_STATE_DEFAULT);


  lv_clock.weekday_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.weekday_label, &lunar_style, LV_STATE_DEFAULT);

  lv_obj_t *weather_t = lv_obj_create(bg_obj);
  lv_obj_set_size(weather_t, 320, 110);
  lv_obj_set_scrollbar_mode(weather_t, LV_SCROLLBAR_MODE_OFF);
  lv_obj_align_to(weather_t, bg_obj, LV_ALIGN_TOP_MID, 0, -5);

  static lv_style_t weather_style;
  lv_style_reset(&weather_style);
  lv_style_init(&weather_style);
  lv_style_set_bg_opa(&weather_style, LV_OPA_COVER);
  lv_style_set_bg_color(&weather_style, lv_color_black());
  lv_style_set_text_font(&weather_style, &zh_20);
  lv_style_set_text_color(&weather_style, lv_color_white());
  lv_obj_add_style(weather_t, &weather_style,  LV_STATE_DEFAULT);

  // /*Create an object with the new style*/
  static lv_obj_t * img_bg = NULL;
  img_bg = lv_img_create(weather_t);
  lv_img_set_src(img_bg, &caishengye);
  lv_obj_set_style_img_recolor(img_bg, lv_color_hex(rnd_next(0, 0xFFFFF0)), 0);
  lv_img_set_pivot(img_bg, 50,50);
  lv_img_set_angle(img_bg, 0);
  lv_obj_set_pos(img_bg, 4, 5);
  lv_obj_set_size(img_bg, 75, 133);
  lv_obj_align_to(img_bg, weather_t, LV_ALIGN_TOP_RIGHT, 0, 0);



  static lv_style_t data_style;
  lv_style_reset(&data_style);
  lv_style_init(&data_style);
  lv_style_set_bg_opa(&data_style, LV_OPA_TRANSP);
  lv_style_set_text_font(&data_style, &lv_font_28);
  lv_style_set_text_color(&data_style, lv_color_white());


  lv_clock.date_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.date_label, &data_style, LV_STATE_DEFAULT);


  lv_clock.weather_label = lv_label_create(weather_t);
  lv_obj_add_style(lv_clock.weather_label, &weather_style, LV_STATE_DEFAULT);
  
  lv_obj_align_to(lv_clock.time_label, lv_obj_get_parent(lv_clock.time_label), LV_ALIGN_BOTTOM_LEFT, 0, 5);
   // 设置时间标签lv_clock.date_label对象基于父对象顶部中间对齐
  lv_obj_align_to(lv_clock.lunar_label, lv_obj_get_parent(lv_clock.lunar_label), LV_ALIGN_TOP_LEFT, 0, 0);
  // 设置时间标签lv_clock.weekday_label对象基于父对象底部中间对齐
  lv_obj_align_to(lv_clock.weekday_label, lv_obj_get_parent(lv_clock.weekday_label),LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_align_to(lv_clock.weather_label, lv_obj_get_parent(lv_clock.weather_label),LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_align_to(lv_clock.date_label, lv_obj_get_parent(lv_clock.date_label), LV_ALIGN_BOTTOM_RIGHT, 0, 0);


 
  // 添加定时任务，获取当前设备时间
  lv_timer_t *timer = lv_timer_create(update_time_label, 200, (void *)&lv_clock);
  if (timer == NULL) {
    printf("[%s:%d] lv_timer_create failed\n", __FUNCTION__, __LINE__);
  }
  Serial.println("Setup done");
  // 关闭NTP客户端
  timeClient.end();
}
void loop() {
  // put your main code here, to run repeatedly:
  lv_timer_handler();
  delay(50);
  ledcWrite(0, LOW_BRIGHTNESS); // 降低亮度
  esp_sleep_enable_timer_wakeup(5 * 1000000);
  esp_light_sleep_start();
  if (!WiFi.isConnected()) {
    reconnectWiFi();
  } 
}


void reconnectWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(config.stassid, config.stapsw);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("WiFi connected");
}

