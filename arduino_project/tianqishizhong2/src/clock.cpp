#include "clock.h"

OneWire oneWire;
DallasTemperature sensors;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,30*60*1000);

void get_week_name(uint8_t week, String *week_name){
    switch (week)
    {
    case 1:
        *week_name = "周一";
        break;
    case 2:
        *week_name =  "周二";
        break;
    case 3:
        *week_name = "周三";
        break;
    case 4:
        *week_name = "周四";
        break;
    case 5:
        *week_name = "周五"; 
        break;
    case 6:
        *week_name = "周六";
        break;
    case 0:
        *week_name = "周日";
        break;
    default:
        *week_name = "error";
        break;
    }
}

void get_data(data_t *ptr) {
    if (ptr == NULL) {
        Serial.println("get_data error");
        return;
    }
    timeClient.update();
    ptr->hour = timeClient.getHours();
    ptr->minute = timeClient.getMinutes();
    ptr->second = timeClient.getSeconds();
    time_t now = timeClient.getEpochTime();
    struct tm *ptm = localtime(&now);
    ptr->day = ptm->tm_mday;
    ptr->month = ptm->tm_mon + 1;
    ptr->year = ptm->tm_year + 1900;
    uint8_t week = ptm->tm_wday;
    String week_name;
    get_week_name(week, &week_name);
    ptr->week_name = week_name;
    cnDate solar = Solar2Lunar(ptr->year, ptr->month, ptr->day);
    uint32_t lunar_dd = solar.lunarDay;
    uint32_t lunar_mm = solar.lunarMonth;
    bool is_leap_month = solar.isLeapMonth;
    // String lunar_date;
    convertChineseString(lunar_mm, lunar_dd, is_leap_month, &ptr->lunar_date);
    unsigned char  solar_num = Solar2Term(ptr->year, ptr->month, ptr->day);
    convertSolarString(solar_num, &ptr->solar_term);
}

int temperature_init() {
    // 初始化 oneWire 对象
    oneWire.begin(ONE_WIRE_BUS);
    // 初始化 sensors 对象
    sensors.setOneWire(&oneWire);
    uint8_t deviceAddress[8];
    sensors.getAddress(deviceAddress, 0);
    bool connected = sensors.isConnected(deviceAddress);
    if (!connected) {
        return -1;
    }
    return 0;
}

int get_temperature() {
    sensors.requestTemperatures();
    int temperature = sensors.getTempCByIndex(0);
    return temperature;
}

void convertSolarString(unsigned char solar_num, String *solar_term) {
    // 节气
    String solar_terms[] = {"无节气", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑","立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};
    *solar_term = solar_terms[solar_num];
}

void convertChineseString(uint32_t lunar_mm, uint32_t lunar_dd, bool is_leap_month,  String *lunar_date) {
    String chinese_number[] = {"零", "一", "二", "三","四","五","六","七", "八", "九", "十"};
    if (is_leap_month) {
        *lunar_date = "闰";
    }
    // 月份转换
    if (lunar_mm == 1) {
        *lunar_date = "正月";
    } else if (lunar_mm < 10) {
        *lunar_date = chinese_number[lunar_mm] + "月";
    } else if (lunar_mm == 10) {
        *lunar_date = "十月";
    } else if (lunar_mm == 11) {
        *lunar_date = "冬月";
    } else if (lunar_mm == 12) {
        *lunar_date = "腊月";
    }
    // 日期转换 十二月初一
    if (lunar_dd < 10) {
        *lunar_date += "初" + chinese_number[lunar_dd];
    } else if (lunar_dd == 10) {
        *lunar_date += "初十";
    } else if (lunar_dd < 20) {
        *lunar_date += "十" + chinese_number[lunar_dd % 10];
    } else if (lunar_dd == 20) {
        *lunar_date += "廿";
    } else if (lunar_dd < 30) {
        *lunar_date += "廿" + chinese_number[lunar_dd % 10];
    } else {
        *lunar_date += "三十";
    }
}