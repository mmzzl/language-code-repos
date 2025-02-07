#include <Arduino.h>
#include "network.h"
#include <TFT_eSPI.h>
#include "gui.h"
#include "clock.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(512);
  // 网络初始化
  network_init();
  //温度初始化
  int rest = temperature_init();
  if (rest == -1) {
    Serial.println("Failed to initialize temperature sensor");
  }
  tft_init();
  time_init();
  // 启用看门狗
  ESP.wdtEnable(5000);
}

void loop() {
  ESP.wdtFeed();
  main_gui();
  start_server();
}