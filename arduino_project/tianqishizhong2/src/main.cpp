#include <Arduino.h>
#include "network.h"


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(512);

  // 尝试读取 EEPROM 中保存的 SSID 和密码
  String ssid = "";
  String password = "";

  if (readDataFromEEPROM(ssid, password)) {
    Serial.println("Using saved SSID and password from EEPROM.");
    connectToWiFi(ssid.c_str(), password.c_str());
  } else {
    WiFi.softAP("ESP8266-Config");
    server.on("/", handleRoot);
    server.on("/connect", HTTP_POST, handlerConnect);
    server.begin();
  }
}

void loop() {
  server.handleClient();
}