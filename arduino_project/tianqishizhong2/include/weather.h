#ifndef WEATHER_H
#define WEATHER_H
#include "ArduinoJson.h"
#include <WiFiClientSecure.h>
#include <geoip.h>

typedef struct {
    String city;
    String date;
    String dayweather;
    String nightweather;
} weather_t;
void getWeather(weather_t *forecasts);
void getRequestHttp(String url, String *body);
void getGeo();
#endif