#ifndef WEATHER_H
#define WEATHER_H
#include "ArduinoJson.h"
#include "Arduino.h"
#include <WiFiClientSecure.h>
#include <geoip.h>

typedef struct {
    char city[50];
    char date[50];
    char dayweather[50];
    char nightweather[50];
    char realtimeWeather[50];
} weather_t;
void getWeather(weather_t *forecasts);
void getRequestHttp(String url, String *body);
void getGeo();
void realtimeWeather(weather_t *forecasts);
#endif