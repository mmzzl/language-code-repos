#ifndef _MY_LV_PORTS_H_
#define _MY_LV_PORTS_H_
#include <TFT_eSPI.h>
#include <lvgl.h>
const uint16_t screenWidth = 320;
const uint16_t screenHeight = 240;
void my_disp_init(void); // 挂载lvgl接口，设置buffer

#endif