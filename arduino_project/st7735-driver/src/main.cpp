#include <Arduino.h>
#include "my_lv_ports.h"
#include <TFT_eSPI.h>
#include <lv_demo.h>
#include <lvgl.h>

// put function declarations here:


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
  #if 0
  /* Create simple label */
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, LVGL_Arduino.c_str());
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  #else
    /* Try an example from the lv_examples Arduino library
      make sure to include it as written above.
    lv_example_btn_1();
  */

    // uncomment one of these demos
    // lv_demo_widgets(); // OK
    lv_demo_benchmark(); // OK
    // lv_demo_keypad_encoder();     // works, but I haven't an encoder
    // lv_demo_music();              // NOK
    // lv_demo_printer();
    // lv_demo_stress();             // seems to be OK
  #endif
  Serial.println("Setup done");
}
void loop() {
  // put your main code here, to run repeatedly:
  lv_timer_handler();
  delay(5);
}

