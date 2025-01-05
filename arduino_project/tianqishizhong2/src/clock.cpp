#include "clock.h"

void get_week_name(uint8_t week, char *week_name){
    switch (week)
    {
    case 1:
        week_name = "周一";
        break;
    case 2:
        week_name = "周二";
        break;
    case 3:
        week_name = "周三";
        break;
    case 4:
        week_name = "周四";
        break;
    case 5:
        week_name = "周五"; 
        break;
    case 6:
        week_name = "周六";
        break;
    case 0:
        week_name = "周日";
        break;
    default:
        week_name = "error";
        break;
    }
}

void get_time(n_time_t  *time){
    timeClient.update();
    time->hour = timeClient.getHours();
    time->minute = timeClient.getMinutes();
    time->second = timeClient.getSeconds();
    time_t now = timeClient.getEpochTime();
    struct tm *ptm = localtime(&now);
    time->day = ptm->tm_mday;
    time->month = ptm->tm_mon + 1;
    time->year = ptm->tm_year + 1900;
    uint8_t week = ptm->tm_wday;
    char *week_name;
    get_week_name(week, week_name);
    time->week_name = week_name;
}