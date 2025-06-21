#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

struct config_type {
    char stassid[32];
    char stapsw[16];
    char cuid[40];
    char ctopic[32];
    uint8_t reboot;
    uint8_t magic;
};

class NetworkManager {
public:
    void loadConfig();
    void restoreFactory();
    void saveConfig();
    void apConfig(String mac);
    void reconnectWiFi();

    config_type config;

private:
    static const uint16_t EEPROM_SSID_START_ADDR = 0;
    static const uint16_t EEPROM_PASSWORD_START_ADDR = 64;
    static const uint16_t EEPROM_LED_START_ADDR = 128;
    static const uint8_t MAX_LENGTH = 32;
    
    WiFiUDP Udp;
    char config_flag = 0;
    static const uint8_t MAGIC_NUMBER = 0xAA;
};

#endif
