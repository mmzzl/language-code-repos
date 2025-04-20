#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

struct config_type
{
  char stassid[32];
  char stapsw[16];
  char cuid[40];
  char ctopic[32];
  uint8_t reboot;
  uint8_t magic;
};
extern config_type config;

// 定义 EEPROM 存储位置
#define EEPROM_SSID_START_ADDR 0   // SSID 起始地址
#define EEPROM_PASSWORD_START_ADDR 64 // 密码起始地址
#define EEPROM_LED_START_ADDR 128 // 存储led亮度起始地址
#define MAX_LENGTH 32              // SSID 和密码的最大长度
void loadConfig();
void restoreFactory();
void saveConfig();
// void delayRestart(float t);
void apConfig(String mac);

#endif