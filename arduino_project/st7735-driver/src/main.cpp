#include <Arduino.h>
#include "my_lv_ports.h"
// #include "zh_28.h"
#include "solar2lunar.h"
#include "zh_18.h"
#include <TFT_eSPI.h>
#include <lv_demo.h>
#include <lvgl.h>
#include <time.h>

typedef struct _lv_clock
{
    lv_obj_t *time_label; // 时间标签
    lv_obj_t *date_label; // 日期标签
    lv_obj_t *weekday_label; // 星期标签
    lv_obj_t *lunar_label; // 农历标签
    lv_obj_t *lunar_term_label; //节气标签
}lv_clock_t;

static lv_clock_t lv_clock = { 0 };


void update_time_label(lv_timer_t *timer);
void update_weekday_label(lv_timer_t *timer);

// put function declarations here:
// 更新标签文本的函数
// 更新标签文本的函数
void update_time_label(lv_timer_t *timer) {
  const char *weekday_names[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
  time_t now = time(NULL);
  struct tm * local_time = localtime(&now);
  char time_str[20];
  char year_str[5];
  char month_str[3];
  char day_str[3];
  char full_time_str[50];
  const char *weekday_str;
  strftime(time_str, sizeof(time_str), "%H:%M:%S", local_time);
  strftime(year_str, sizeof(year_str), "%Y", local_time);
  strftime(month_str, sizeof(month_str), "%m", local_time);
  strftime(day_str, sizeof(day_str), "%d", local_time);
  snprintf(full_time_str, sizeof(full_time_str), "%s年%s月%s日", year_str, month_str, day_str);
  cnDate solar = Solar2Lunar(atoi(year_str), atoi(month_str), atoi(day_str));
  uint32_t lunar_dd = solar.lunarDay;
  uint32_t lunar_mm = solar.lunarMonth;
  bool is_leap_month = solar.isLeapMonth;
  char *lunar_date = convertChineseString(lunar_mm, lunar_dd, is_leap_month);
  unsigned char  solar_num = Solar2Term(atoi(year_str), atoi(month_str), atoi(day_str));
  char *solar_term = convertSolarString(solar_num);
  weekday_str = weekday_names[local_time->tm_wday];
  if (timer != NULL && timer->user_data != NULL) 
  {
    lv_clock_t * clock = (lv_clock_t *)(timer->user_data);
    if (clock->time_label != NULL)
    {
      lv_label_set_text(clock->time_label, time_str);
      lv_obj_align_to(clock->time_label, lv_obj_get_parent(clock->time_label), LV_ALIGN_CENTER, 0, 0);
    }
    if (clock->weekday_label != NULL)
    {
      lv_label_set_text(clock->weekday_label, weekday_str);
      lv_obj_align_to(clock->weekday_label, lv_obj_get_parent(clock->weekday_label), LV_ALIGN_TOP_LEFT, 0, 0);
    }
    if (clock->date_label != NULL)
    {
      lv_label_set_text(clock->date_label, full_time_str);
      lv_obj_align_to(clock->date_label, lv_obj_get_parent(clock->date_label), LV_ALIGN_TOP_RIGHT, 0, 0);
    }
    if (clock->lunar_label != NULL)
    {
      lv_label_set_text(clock->lunar_label, lunar_date);
      lv_obj_align_to(clock->lunar_label, lv_obj_get_parent(clock->lunar_label), LV_ALIGN_BOTTOM_LEFT, 0, 0);
    }
    if (clock->lunar_term_label != NULL)
    {
      lv_label_set_text(clock->lunar_term_label, solar_term);
      lv_obj_align_to(clock->lunar_term_label, lv_obj_get_parent(clock->lunar_term_label), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    }
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");
  lv_init();
  my_disp_init();

 

  /*Create an object with the new style*/
  
  static lv_style_t date_time_clock_style;
  lv_style_reset(&date_time_clock_style);
  lv_style_init(&date_time_clock_style);
  lv_style_set_radius(&date_time_clock_style, 5); //设置圆角样式
  lv_style_set_bg_opa(&date_time_clock_style,  LV_OPA_20); //设置背景透明度
  lv_style_set_border_width(&date_time_clock_style, 0); //设置边框宽度
  lv_style_set_bg_color(&date_time_clock_style, lv_color_black());
  lv_style_set_pad_left(&date_time_clock_style, 1); // 设置样式左边padding填充宽度
  lv_style_set_pad_right(&date_time_clock_style, 0); //设置样式右边padding填充宽度
  lv_style_set_pad_top(&date_time_clock_style, 0); //设置样式上边padding填充宽度
  lv_style_set_pad_bottom(&date_time_clock_style, 0); //设置样式下边padding填充宽度

  static lv_style_t time_style;
  lv_style_reset(&time_style);
  lv_style_init(&time_style);
  lv_style_set_bg_opa(&time_style, LV_OPA_COVER);
  lv_style_set_border_width(&time_style, 0);
  lv_style_set_radius(&time_style, 5);
  lv_style_set_bg_color(&time_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_pad_left(&time_style, 0);
  lv_style_set_pad_right(&time_style, 0);
  lv_style_set_pad_top(&time_style, 0);
  lv_style_set_pad_bottom(&time_style, 0);

  static lv_style_t date_style;
  lv_style_reset(&date_style);
  lv_style_init(&date_style);
  lv_style_set_bg_opa(&date_style, LV_OPA_COVER);
  lv_style_set_border_width(&date_style, 0);
  lv_style_set_radius(&date_style, 5);
  lv_style_set_bg_color(&date_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_pad_left(&date_style, 0);
  lv_style_set_pad_right(&date_style, 0);

  static lv_style_t time_label_style; //实际标签样式
  lv_style_reset(&time_label_style);
  lv_style_init(&time_label_style);
  lv_style_set_text_color(&time_label_style, lv_color_white());
  lv_style_set_text_font(&time_label_style, &lv_font_montserrat_32);
  // lv_style_set_text_opa(&time_label_style, LV_OPA_0);
  // lv_style_set_bg_opa(&time_label_style, LV_OPA_0);

  // static lv_style_t weekday_label_style; //实际标签样式
  // lv_style_reset(&weekday_label_style);
  // lv_style_init(&weekday_label_style);
  // lv_style_set_text_color(&weekday_label_style, lv_color_white());
  // lv_style_set_text_font(&weekday_label_style, &zh_16);
  // lv_style_set_text_opa(&time_label_style, LV_OPA_0);
  // lv_style_set_bg_opa(&time_label_style, LV_OPA_0);

  static lv_style_t date_label_style;
  lv_style_reset(&date_label_style);
  lv_style_init(&date_label_style);
  lv_style_set_text_opa(&date_label_style, LV_OPA_COVER);
  lv_style_set_bg_opa(&date_label_style, LV_OPA_0);
  lv_style_set_text_color(&date_label_style, lv_color_white());
  lv_style_set_text_font(&date_label_style, &lv_font_montserrat_18);

  static lv_style_t week_label_style;
  lv_style_reset(&week_label_style);
  lv_style_init(&week_label_style);
  // lv_style_set_text_opa(&week_label_style, LV_OPA_COVER);
  lv_style_set_bg_opa(&week_label_style, LV_OPA_0);
  lv_style_set_text_color(&week_label_style, lv_palette_main(LV_PALETTE_YELLOW));
  lv_style_set_text_font(&week_label_style, &zh_18);  
  

  lv_obj_t *time_date_obj = lv_obj_create(lv_scr_act()); // 基于屏幕创建时间日期对象
  if (time_date_obj == NULL)
    {
        printf("[%s:%d] time_date_obj create failed\n", __FUNCTION__, __LINE__);
        return;
    }
 
	lv_obj_set_size(time_date_obj, 320, 100); // 设置对象大小
  lv_obj_center(time_date_obj); // 对象居屏幕中间显示
	lv_obj_add_style(time_date_obj, &date_time_clock_style, LV_STATE_DEFAULT); //给time_date_obj对象添加样式
 
  /*Time display*/
  lv_obj_t *time_obj = lv_obj_create(time_date_obj); // 基于time_date_obj对象创建时间对象
  if (time_obj == NULL)
  {
      printf("[%s:%d] time_obj create failed\n", __FUNCTION__, __LINE__);
      return;
  }
 
  lv_obj_set_size(time_obj, 158, 100); // 设置对象大小
  lv_obj_align_to(time_obj, time_date_obj, LV_ALIGN_LEFT_MID, 0, 0); // 设置time_obj对象基于time_date_obj对象左边中间对齐
  lv_obj_add_style(time_obj, &time_style, LV_STATE_DEFAULT);  // 给time_obj对象添加样式

  
  lv_clock.time_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.time_label, &time_label_style, LV_STATE_DEFAULT);
  // 创建一个定时器，每 1 秒调用一次 update_time_label 函数
  
  
  lv_clock.weekday_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.weekday_label, &week_label_style, LV_STATE_DEFAULT);

  lv_clock.date_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.date_label, &week_label_style, LV_STATE_DEFAULT);

  lv_clock.lunar_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.lunar_label, &week_label_style, LV_STATE_DEFAULT);

  lv_clock.lunar_term_label = lv_label_create(time_obj);
  lv_obj_add_style(lv_clock.lunar_term_label, &week_label_style, LV_STATE_DEFAULT);

   // 设置时间标签lv_clock.time_label对象基于父对象居中对齐
  lv_obj_align_to(lv_clock.time_label, lv_obj_get_parent(lv_clock.time_label), LV_ALIGN_CENTER, 0, 0);
   // 设置时间标签lv_clock.date_label对象基于父对象顶部中间对齐
  lv_obj_align_to(lv_clock.date_label, lv_obj_get_parent(lv_clock.date_label), LV_ALIGN_TOP_RIGHT, 2, 0);
  // 设置时间标签lv_clock.weekday_label对象基于父对象底部中间对齐
  lv_obj_align_to(lv_clock.weekday_label, lv_obj_get_parent(lv_clock.weekday_label),LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_align_to(lv_clock.lunar_label, lv_obj_get_parent(lv_clock.lunar_label),LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_align_to(lv_clock.lunar_term_label, lv_obj_get_parent(lv_clock.lunar_term_label),LV_ALIGN_BOTTOM_RIGHT, 0, 0);

  lv_timer_t *timer = lv_timer_create(update_time_label, 200, (void *)&lv_clock);
  if (timer == NULL) {
    printf("[%s:%d] lv_timer_create failed\n", __FUNCTION__, __LINE__);
  }
    



  

  // lv_obj_t * label = lv_label_create(obj);
  // lv_style_set_text_font(&style, &zh_28);
  // lv_label_set_text(label, "正月初一");
  // // 设置字体为白色
  // static lv_style_t style_label;
  // lv_style_init(&style_label);
  // lv_style_set_text_color(&style_label, lv_color_white());
  // // lv_style_set_text_color(&style_label, lv_color_hex(0xffff00));
  // // 设置字体大小
  // lv_style_set_text_font(&style_label, &zh_28);

  // lv_obj_t *label = lv_label_create(scr);
  // lv_label_set_text(label, "正月初一");
  // //应用样式到标签
  // lv_obj_add_style(label, &style_label, LV_PART_MAIN);

  // // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  // // 设置label标签位置
  // lv_obj_set_pos(label, 0, 0);
  // lv_demo_benchmark(); // OK
  Serial.println("Setup done");
}
void loop() {
  // put your main code here, to run repeatedly:
  lv_timer_handler();
  delay(5);
}

