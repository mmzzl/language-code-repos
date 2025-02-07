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
  char week_name[10];
  char lunar_date[50];
  int temperature;
  char solar_term[50];
} data_t;


void get_week_name(uint8_t week, char *week_name);
void get_data(data_t *ptr);
void convertChineseString(uint32_t lunar_mm, uint32_t lunar_dd, bool is_leap_month, char *lunar_date);
int get_temperature();
int temperature_init();
void convertSolarString(unsigned char solar_num, char *solar_term);
void time_init();
int daysToNextSolarTerm(unsigned short year, unsigned char month, unsigned char day, const char** nextTermName);
#endif
