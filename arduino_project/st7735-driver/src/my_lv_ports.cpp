#include "my_lv_ports.h"

TFT_eSPI LVGLManager::tft = TFT_eSPI(LVGLManager::screenHeight, LVGLManager::screenWidth);

void LVGLManager::flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    
    tft.setSwapBytes(true);
    tft.pushImage(area->x1, area->y1, w, h, (uint16_t *)color_p);
    lv_disp_flush_ready(disp);
}

void LVGLManager::print(const char *buf) {
    Serial.printf("%s\r\n", buf);
}


// 新增标签创建函数实现
lv_obj_t* LVGLManager::createLabel(lv_obj_t* parent, const char* error_msg) {
    lv_obj_t* label = lv_label_create(parent);
    if (!label && error_msg) {
        Serial.println(error_msg);
    }
    return label;
}

void LVGLManager::init() {
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf_2_1[screenWidth * 30];
    static lv_color_t buf_2_2[screenWidth * 30];
    
    lv_disp_draw_buf_init(&draw_buf, buf_2_1, buf_2_2, screenWidth * 30);
    
    tft.begin();
    tft.setRotation(3);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
#if LV_USE_LOG != 0
    lv_log_register_print_cb(print);
#endif
}
