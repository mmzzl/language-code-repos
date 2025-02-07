#include "clock.h"

OneWire oneWire;
DallasTemperature sensors;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,30*60*1000);
// 假设 SolarTerms_List 是已定义的数据表
extern const unsigned char SolarTerms_List[];

// 节气名称数组
const char* solarTermNames[] = {
    "小寒", "大寒", "立春", "雨水", "惊蛰", "春分",
    "清明", "谷雨", "立夏", "小满", "芒种", "夏至",
    "小暑", "大暑", "立秋", "处暑", "白露", "秋分",
    "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"
};


// 修改 get_week_name 函数
void get_week_name(uint8_t week, char *week_name) {
    switch (week) {
        case 1:
            strcpy(week_name, "周一");
            break;
        case 2:
            strcpy(week_name, "周二");
            break;
        case 3:
            strcpy(week_name, "周三");
            break;
        case 4:
            strcpy(week_name, "周四");
            break;
        case 5:
            strcpy(week_name, "周五");
            break;
        case 6:
            strcpy(week_name, "周六");
            break;
        case 0:
            strcpy(week_name, "周日");
            break;
        default:
            strcpy(week_name, "error");
            break;
    }
}

void time_init() {
    timeClient.begin();
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
    char week_name[10];
    get_week_name(week, week_name);
    strcpy(ptr->week_name, week_name);
    cnDate solar = Solar2Lunar(ptr->year, ptr->month, ptr->day);
    uint32_t lunar_dd = solar.lunarDay;
    uint32_t lunar_mm = solar.lunarMonth;
    bool is_leap_month = solar.isLeapMonth;
    // String lunar_date;
    convertChineseString(lunar_mm, lunar_dd, is_leap_month, ptr->lunar_date);
    unsigned char  solar_num = Solar2Term(ptr->year, ptr->month, ptr->day);
    convertSolarString(solar_num, ptr->solar_term);
}

int temperature_init() {
    oneWire.begin(ONE_WIRE_BUS);
    sensors.setOneWire(&oneWire);
    uint8_t deviceAddress[8];
    sensors.getAddress(deviceAddress, 0);
    bool connected = sensors.isConnected(deviceAddress);
    if (!connected) {
        Serial.println("Temperature sensor connection failed!");
        return -1;
    }
    Serial.println("Temperature sensor connected successfully.");
    return 0;
}

int get_temperature() {
    sensors.requestTemperatures();
    int temperature = sensors.getTempCByIndex(0);
    return temperature;
}

void convertSolarString(unsigned char solar_num, char *solar_term) {
    // 节气
    const char* solar_terms[] = {"无节气", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑","立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};
    // 检查 solar_num 是否越界
    if (solar_num < sizeof(solar_terms) / sizeof(solar_terms[0])) {
        strcpy(solar_term, solar_terms[solar_num]);
    } else {
        // 处理越界情况，可根据实际需求修改
        strcpy(solar_term, "无节气");
    }
}

void convertLunarMonth(uint32_t lunar_mm, bool is_leap_month, char *month_str) {
    const char* chinese_number[] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
    if (is_leap_month) {
        strcpy(month_str, "闰");
    } else {
        month_str[0] = '\0';
    }
    if (lunar_mm == 1) {
        strcat(month_str, "正月");
    } else if (lunar_mm < 10) {
        strcat(month_str, chinese_number[lunar_mm]);
        strcat(month_str, "月");
    } else if (lunar_mm == 10) {
        strcat(month_str, "十月");
    } else if (lunar_mm == 11) {
        strcat(month_str, "冬月");
    } else if (lunar_mm == 12) {
        strcat(month_str, "腊月");
    }
}

void convertLunarDay(uint32_t lunar_dd, char *day_str) {
    const char* chinese_number[] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
    if (lunar_dd < 10) {
        strcpy(day_str, "初");
        strcat(day_str, chinese_number[lunar_dd]);
    } else if (lunar_dd == 10) {
        strcpy(day_str, "初十");
    } else if (lunar_dd < 20) {
        strcpy(day_str, "十");
        strcat(day_str, chinese_number[lunar_dd % 10]);
    } else if (lunar_dd == 20) {
        strcpy(day_str, "廿");
    } else if (lunar_dd < 30) {
        strcpy(day_str, "廿");
        strcat(day_str, chinese_number[lunar_dd % 10]);
    } else {
        strcpy(day_str, "三十");
    }
}

void convertChineseString(uint32_t lunar_mm, uint32_t lunar_dd, bool is_leap_month, char *lunar_date) {
    char month_str[20];
    char day_str[20];
    convertLunarMonth(lunar_mm, is_leap_month, month_str);
    convertLunarDay(lunar_dd, day_str);
    strcpy(lunar_date, month_str);
    strcat(lunar_date, day_str);
}


// 函数用于计算距离下个节气的天数并获取节气名称
int daysToNextSolarTerm(unsigned short year, unsigned char month, unsigned char day, const char** nextTermName) {
    int days = 0;
    unsigned short currentYear = year;
    unsigned char currentMonth = month;
    unsigned char currentDay = day;

    // 每个月的天数（不考虑闰年）
    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (true) {
        // 检查当前日期是否为节气
        unsigned char termId = Solar2Term(currentYear, currentMonth, currentDay);
        if (termId != 0) {
            // 获取节气名称
            *nextTermName = solarTermNames[termId - 1];
            break;
        }

        // 增加一天
        days++;
        currentDay++;

        // 处理月份和年份的进位
        if (currentDay > daysInMonth[currentMonth - 1]) {
            currentDay = 1;
            currentMonth++;
            if (currentMonth > 12) {
                currentMonth = 1;
                currentYear++;
            }
        }
    }

    return days;
}