#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Encoder.h>
TFT_eSPI tft = TFT_eSPI();

// 定义编码器引脚
Encoder myEnc(2, 3);
// 定义按键引脚
const int buttonPin = 4;
// 定义上一次编码器的值
long oldPosition = -999;

//LVGL显示缓存区
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

// 用于显示编码器值的标签对象指针
lv_obj_t * encoderValueLabel;

// 菜单和页面对象
lv_obj_t *mainMenu;
lv_obj_t *settingsMenu;
lv_obj_t *weatherPage;
lv_obj_t *timeCalibrationPage;

// 全局输入组变量
lv_group_t *inputGroup;

// lvgl 显示回调
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

// 编码器输入设备读取回调函数
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    // 根据编码器旋转方向设置输入数据的差值
    data->enc_diff = newPosition - oldPosition;
    oldPosition = newPosition;

    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(buttonPin);
    if (lastButtonState == HIGH && currentButtonState == LOW) {
      data->state = LV_INDEV_STATE_PR;
      lv_obj_t *focused = lv_group_get_focused(inputGroup);
      if(focused) {
        lv_obj_t *label = lv_obj_get_child(focused, 0);
        const char *text = lv_label_get_text(label);
        if(strcmp(text, "查看天气") == 0) {
          lv_obj_add_flag(mainMenu, LV_OBJ_FLAG_HIDDEN);
          lv_obj_clear_flag(weatherPage, LV_OBJ_FLAG_HIDDEN);
        } else if(strcmp(text, "设置") == 0) {
          lv_obj_add_flag(mainMenu, LV_OBJ_FLAG_HIDDEN);
          lv_obj_clear_flag(settingsMenu, LV_OBJ_FLAG_HIDDEN);
        }  else if(strcmp(text, "时间校正") == 0) {
          lv_obj_add_flag(settingsMenu, LV_OBJ_FLAG_HIDDEN);
          lv_obj_clear_flag(timeCalibrationPage, LV_OBJ_FLAG_HIDDEN);
        }
      } else {
        data->state = currentButtonState == LOW ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
      }
      lastButtonState = currentButtonState;
    }
    // 更新显示编码器值的标签文本
    char encoderValueStr[20];
    snprintf(encoderValueStr, sizeof(encoderValueStr), "Encoder Value: %ld", newPosition);
    lv_label_set_text(encoderValueLabel, encoderValueStr);
  }
  // 读取按键状态
  data->state = digitalRead(buttonPin) == LOW ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

// 创建页面函数
void createPages() {
  // 创建主菜单
  mainMenu= lv_menu_create(lv_scr_act());
  lv_obj_set_size(mainMenu, lv_pct(100), lv_pct(100));

  // 添加查看天气菜单项
  lv_obj_t *viewWeatherItem = lv_obj_create(mainMenu);
  lv_obj_set_size(viewWeatherItem, lv_pct(100), 50);
  lv_obj_align(viewWeatherItem, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_t *viewWeatherLabel = lv_label_create(viewWeatherItem);
  lv_label_set_text(viewWeatherLabel, "查看天气");
  lv_obj_center(viewWeatherLabel);

  // 添加进入设置菜单项
  lv_obj_t* settingsItem = lv_obj_create(mainMenu);
  lv_obj_set_size(settingsItem, lv_pct(100), 50);
  lv_obj_align(settingsItem, LV_ALIGN_TOP_MID, 0, 50);
  lv_obj_t * settingsLabel = lv_label_create(settingsItem);
  lv_label_set_text(settingsLabel, "设置");
  lv_obj_center(settingsLabel);

  // 创建设置菜单
  lv_obj_t *settingsMenu = lv_obj_create(lv_scr_act());
  lv_obj_set_size(settingsMenu, lv_pct(100), lv_pct(100));
  lv_obj_add_flag(settingsMenu, LV_OBJ_FLAG_HIDDEN);

  // 添加时间校正菜单项
  lv_obj_t *timeCalibrationItem = lv_obj_create(settingsMenu);
  lv_obj_set_size(timeCalibrationItem, lv_pct(100), 50);
  lv_obj_align(timeCalibrationItem, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_t *timeCalibrationLabel = lv_label_create(timeCalibrationItem);
  lv_label_set_text(timeCalibrationLabel, "时间校正");
  lv_obj_center(timeCalibrationLabel);

// 创建天气页面
  weatherPage = lv_obj_create(lv_scr_act());
  lv_obj_set_size(weatherPage, lv_pct(100), lv_pct(100));
  lv_obj_add_flag(settingsMenu, LV_OBJ_FLAG_HIDDEN);
  lv_obj_t *weatherLabel = lv_label_create(weatherPage);
  lv_label_set_text(weatherLabel, "天气信息");
  lv_obj_center(weatherLabel);
  
  //创建时间矫正页面
  timeCalibrationPage = lv_obj_create(lv_scr_act());
  lv_obj_set_size(timeCalibrationPage, lv_pct(100), lv_pct(100));
  lv_obj_add_flag(timeCalibrationPage, LV_OBJ_FLAG_HIDDEN);
  lv_obj_t *timeCalibrationPageLabel = lv_label_create(timeCalibrationPage);
  lv_label_set_text(timeCalibrationPageLabel, "时间校正");
  lv_obj_center(timeCalibrationLabel);

}
void setup() {
    // 初始化串口通信
    Serial.begin(115200);
    // 初始化 TFT 显示屏
    tft.begin();
    tft.setRotation(1);
    // 初始化 LVGL
    lv_init();
    // 初始化显示缓冲区
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);
    // 初始化显示驱动
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = tft.width();
    disp_drv.ver_res = tft.height();
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

     // 初始化编码器输入设备驱动
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    lv_indev_drv_register(&indev_drv);

    
     // 初始显示编码器值
    char encoderValueStr[20];
    snprintf(encoderValueStr, sizeof(encoderValueStr), "Encoder Value: %ld", oldPosition);
    lv_label_set_text(encoderValueLabel, encoderValueStr);

    createPages();
    inputGroup = lv_group_create();
    lv_group_add_obj(inputGroup, mainMenu);
    lv_group_add_obj(inputGroup, settingsMenu);
    lv_indev_set_group(lv_indev_get_act(), inputGroup);
}

void loop() {
    // 处理 LVGL 任务
    lv_timer_handler();
    delay(5);
}
