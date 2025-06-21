#ifndef WEATHER_H
#define WEATHER_H
#include "ArduinoJson.h"
#include <WiFiClientSecure.h>
#include "map"
// #include "baoyu.h"
// #include "dabaoyu.h"
// #include "dayu.h"
// #include "duoyun.h"
// #include "leizhenyu.h"
// #include "qing.h"
// #include "qingjianduoyun.h"
// #include "xiaoyu.h"
// #include "yin.h"
// #include "zhenyu.h"
// #include "zhongyu.h"
// #include "weizhitianqi.h"
#include "lvgl.h"
// #include <geoip.h>

typedef struct {
    char city[32];
    char date[16];
    char dayweather[32];
    char nightweather[32];
} weather_t;
void getWeather(weather_t *forecasts);
#endif