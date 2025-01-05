#include "NTPClient.h"
#include "WiFiUdp.h"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,30*60*1000);

void get_week_name(uint8_t week, char *week_name);
void get_time();

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t week;
  uint8_t day;
  uint8_t month;
  uint8_t year;
  char* week_name;
} n_time_t;