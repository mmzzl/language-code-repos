#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

typedef struct Data
{
    struct Node* head;
    struct tm_time* time;
    int temperature;
} Data;


struct tm_time
{
   uint8_t tm_day;
   uint8_t tm_hour;
   uint8_t tm_minute;
   uint8_t tm_second;
   uint8_t tm_month;
   uint16_t tm_year;
   const char* tm_week;
};

struct config_type
{
    char stdssid[32];
    char stapsw[64];
};

typedef struct Weather
{
    const char* city;
    const char* date;
    const char* dayweather;
    const char* nightweather;
    const char* daytemp;
    const char* nighttemp;
} Weather;

typedef struct Node
{
    struct Weather* weather;
    struct Node* next;
} Node;


#endif 