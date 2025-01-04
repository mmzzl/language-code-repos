#ifndef NETWORK_H
#define NETWORK_H
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include "config.h"


class Network
{
    private:
    public:
        config_type config;
        WiFiClientSecure httpsClient;
        uint8_t timeVal = 0;
        bool flag = 0;
        void init();
        void saveConfig();
        void loadConfig();
        void connectSta(const char *stdssid, const char *psw);
        void smartConnect();
};


#endif