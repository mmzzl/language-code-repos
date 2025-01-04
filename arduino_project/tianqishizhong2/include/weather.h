#ifndef WEATHER_H
#define WEATHER_H
#include "ArduinoJson.h"
#include <WiFiClientSecure.h>
#include <geoip.h>

GeoIP geoip; 
location_t loc;
WiFiClientSecure httpsClient;
struct weather_t {
    String city;
    String date;
    String dayweather;
    String nightweather;
};
void getWeather(); 
#endif