#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// 定义 EEPROM 存储位置
#define EEPROM_SSID_START_ADDR 0   // SSID 起始地址
#define EEPROM_PASSWORD_START_ADDR 64 // 密码起始地址
#define MAX_LENGTH 32              // SSID 和密码的最大长度
void saveDataToEEPROM(const char* ssid, const char* password);
void connectToWiFi(const char* ssid, const char* password);
void saveDataToEEPROM(const char *ssid, const char *password);
void disconnectSoftAP();
bool readDataFromEEPROM(String &ssid, String &password);
void handleRoot();
void handlerConnect();
ESP8266WebServer server(80);
#endif