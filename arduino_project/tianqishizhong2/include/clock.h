#ifndef CLOCK_H
#define CLOCK_H
#include "NTPClient.h"
#include "WiFiUdp.h"
#include "solar2lunar.h"
#define ONE_WIRE_BUS 4
#include <OneWire.h>
#include <DallasTemperature.h>


typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t week;
  uint8_t day;
  uint8_t month;
  uint16_t year;
  String week_name;
  String lunar_date;
  int temperature;
  String solar_term;
} data_t;


void get_week_name(uint8_t week, String *week_name);
void get_data(data_t *ptr);
void convertChineseString(uint32_t lunar_mm, uint32_t lunar_dd, bool is_leap_month,  String *lunar_date);
int get_temperature();
int temperature_init();
void convertSolarString(unsigned char solar_num, String *solar_term);
#endif
