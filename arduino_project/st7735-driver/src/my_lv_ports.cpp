#include "my_lv_ports.h"
#define ESP32_I2C_SDA 33
#define ESP32_I2C_SCL 25

TFT_eSPI tft = TFT_eSPI(screenHeight, screenWidth);

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    
    // 尝试不同的字节交换设置
    tft.setSwapBytes(true); // 或 true
    
    // 正确的指针转换方式
    tft.pushImage(area->x1, area->y1, w, h, (uint16_t *)color_p);
    
    // 通知LVGL刷新完成
    lv_disp_flush_ready(disp);
}

/* Display flushing */
// void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
//     lv_color_t *color_p) {
// uint32_t w = (area->x2 - area->x1 + 1);
// uint32_t h = (area->y2 - area->y1 + 1);

// tft.setSwapBytes(true);
// tft.pushImageDMA(area->x1, area->y1, w, h,(uint16_t *)&color_p->full);

// // tft.startWrite();
// // tft.setAddrWindow( area->x1, area->y1, w, h );
// // tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
// // tft.endWrite();

// lv_disp_flush_ready(disp);
// }

#if LV_USE_LOG != 0 
void my_print(const char *buf){ Serial.printf("% \r\n", buf);}
#endif

void my_disp_init(void) {
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf_2_1[screenWidth * 30];
    static lv_color_t buf_2_2[screenWidth * 30];
    lv_disp_draw_buf_init(&draw_buf, buf_2_1, buf_2_2, screenWidth * 30);
    // TFT驱动初始化
    tft.begin();
    // tft.initDMA();
    tft.setRotation(3);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    #if LV_USE_LOG != 0
        lv_log_register_print_cb(my_print);
    #endif
} 