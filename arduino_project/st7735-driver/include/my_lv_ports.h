#ifndef _MY_LV_PORTS_H_
#define _MY_LV_PORTS_H_

#include <TFT_eSPI.h>
#include <lvgl.h>

class LVGLManager {
public:
    static const uint16_t screenWidth = 320;
    static const uint16_t screenHeight = 240;
    
    void init();
    static void flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    static void print(const char *buf);
    
    // 新增标签创建函数
    static lv_obj_t* createLabel(lv_obj_t* parent, 
                                const char* error_msg = "Label creation failed");

private:
    static TFT_eSPI tft;
};

#endif

