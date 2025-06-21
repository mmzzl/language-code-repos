#include "Solar2Lunar.h"

const unsigned long LunarCalendar::BaseData[199] PROGMEM = {

    0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,
    0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A, //1901年-1910年

    0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,
    0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754, //1911年-1920年

    0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,
    0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E, //1921年-1930年

    0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,
    0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48, //1931年-1940年

    0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,
    0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51, //1941年-1950年

    0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,
    0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x7AD53C, //1951年-1960年

    0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,
    0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46, //1961年-1970年

    0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,
    0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50, //1971年-1980年

    0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,
    0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB, //1981年-1990年

    0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,
    0x05AC53,0x0AB647,0x5936BC,0x092E50,0x0C9645, //1991年-2000年

    0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,
    0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E, //2001年-2010年

    0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,
    0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9, //2011年-2020年

    0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,
    0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43, //2021年-2030年

    0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,
    0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C, //2031年-2040年

    0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,
    0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37, //2041年-2050年

    0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,
    0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42, //2051年-2060年

    0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,
    0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B, //2061年-2070年

    0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,
    0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6, //2071年-2080年

    0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,
    0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E, //2081年-2090年

    0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,
    0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5 //2091年-2099年 

};

const unsigned short LunarCalendar::monthTotal[13] PROGMEM = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };  

const unsigned char LunarCalendar::SolarTerms_List[] PROGMEM = {
    0x05,0x13,0x03,0x12,0x05,0x14,0x04,0x13,0x05,0x14,0x05,0x14,0x06,0x16,0x07,0x16,0x07,0x16,0x07,0x17,0x07,0x15,0x06,0x15,
    0x0F,0x03,0x08,0x0B,0x0E,0x03,0x07,0x0A,0x0E,0x03,0x07,0x0A,0x0E,0x03,0x07,0x09,0x0E,0x03,0x07,0x09,0x0E,0x02,0x06,0x09,
    0x0D,0x01,0x05,0x09,0x0C,0x00,0x04,0x00,0x10,0x26,0x40,0x19,0x26,0x41,0x19,0x26,0x41,0x59,0x66,0x41,0x59,0x67,0x61,0x59,
    0x67,0x61,0x5D,0x67,0x63,0x5D,0x67,0x73,0x7D,0x67,0x7F,0x7D,0xE7,0x7F,0xFF,0xE7,0x7F,0xFF,0xFF,0xF0,0x00,0x00,0xF0,0x10,
    0x04,0xF0,0x10,0x24,0xF0,0x10,0x26,
};

LunarCalendar::cnDate LunarCalendar::solarToLunar(unsigned short yyyy, unsigned char mm, unsigned char dd)
{
      /*来自网络高手部分代码*/
    unsigned short yearStor = yyyy;
    unsigned char monthStor = mm, dayStor = dd;
    int bySpring, bySolar, daysPerMonth; 
    //bySpring 记录春节离当年元旦的天数。
    //bySolar 记录阳历日离当年元旦的天数。
    //daysPerMonth记录大小月的天数 29 或30
    unsigned char index; //index 记录从哪个月开始来计算。
    bool flag; //flag 是用来对闰月的特殊处理。
    cnDate plunar;
    unsigned long dataForThisYear = pgm_read_dword_near(&BaseData[yearStor - 1901]); //从表中取出这一年的农历数据
    if (yyyy < 1901 || yyyy >2099 ) {
        cnDate defaultDate;
        defaultDate.lunarYear = 2023;
        return defaultDate;
    }

    if (((dataForThisYear & 0x0060) >> 5) == 1)
        bySpring = (dataForThisYear & 0x001F) - 1;
    else
        bySpring = (dataForThisYear & 0x001F) - 1 + 31;
    bySolar = pgm_read_word_near(&monthTotal[monthStor - 1]) + dayStor - 1;
    if ((!(yearStor % 4)) && (monthStor > 2))
        bySolar++;
    //判断阳历日在春节前还是春节后
    if (bySolar >= bySpring) {//阳历日在春节后（含春节那天）
        bySolar -= bySpring;
        monthStor = 1;
        index = 1;
        flag = 0;
        if ((dataForThisYear & (0x80000 >> (index - 1))) == 0)
            daysPerMonth = 29;
        else
            daysPerMonth = 30;
        while (bySolar >= daysPerMonth) {
            bySolar -= daysPerMonth;
            index++;
            if (monthStor == ((dataForThisYear & 0xF00000) >> 20)) {
                flag = !flag;
                if (flag == 0)
                    monthStor++;
            }
            else
                monthStor++;
            if ((dataForThisYear & (0x80000 >> (index - 1))) == 0)
                daysPerMonth = 29;
            else
                daysPerMonth = 30;
        }
        dayStor = bySolar + 1;
    }
    else {//阳历日在春节前
        bySpring -= bySolar;
        yearStor--;
        dataForThisYear = pgm_read_dword_near(&BaseData[yearStor - 1901]); //从表中取出前一年的农历数据
        monthStor = 12;
        if (((dataForThisYear & 0xF00000) >> 20) == 0)
            index = 12;
        else
            index = 13;
        flag = 0;
        if ((dataForThisYear & (0x80000 >> (index - 1))) == 0)
            daysPerMonth = 29;
        else
            daysPerMonth = 30;
        while (bySpring > daysPerMonth) {
            bySpring -= daysPerMonth;
            index--;
            if (flag == 0)
                monthStor--;
            if (monthStor == ((dataForThisYear & 0xF00000) >> 20))
                flag = !flag;
            if ((dataForThisYear & (0x80000 >> (index - 1))) == 0)
                daysPerMonth = 29;
            else
                daysPerMonth = 30;
        }

        dayStor = daysPerMonth - bySpring + 1;
    }
    plunar.lunarDay = dayStor;
    plunar.lunarMonth = monthStor;
    plunar.lunarYear = yearStor;
    plunar.tiangan = (yearStor - 1864) % 10;
    plunar.dizhi = (yearStor - 1864) % 12;
    if (monthStor == ((dataForThisYear & 0xF00000) >> 20))
        plunar.isLeapMonth = true;
    else
        plunar.isLeapMonth = false;

    return plunar;
}

/*************************************************************
  查表法获得2016年到2050年之间的节气编号函数
  作者：Kinneng  麻花痛：65686789
**************************************************************/

unsigned char LunarCalendar::solarToTerm(unsigned short yyyy, unsigned char mm, unsigned char dd)
{
    if(yyyy<2020)return(0);
    else 
    if(yyyy>2050)return(0);
    else 
    {
        unsigned char base,temp,id,offsetStor;
        //计算指向数据表的指针
        id=(mm-1)<<1;
        if(dd>15)id++;
        //从数据表取出数值ֵ
        offsetStor=id;
        base=pgm_read_byte(&SolarTerms_List[offsetStor]);
        //从数据表取出修正值ֵ
        temp=pgm_read_byte(&SolarTerms_List[pgm_read_byte(&SolarTerms_List[yyyy-1996])*3+(mm-1)/4+55]);
        offsetStor=id;
        //计算对应的位
        if(offsetStor>7)offsetStor=offsetStor-8;
        if(offsetStor>7)offsetStor=offsetStor-8;
        temp=(temp<<offsetStor)&0x80;
        //修正日期
        if(temp==128)base=base+1;
        //返回节气编号
        if(base==dd)
        return(id+1);
        else 
        return(0);
    }
}

LunarCalendar::cnLunarDate LunarCalendar::lunarToChinese(cnDate lunar, unsigned char term) {
    String tiangan[] = {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
    String dizhi[] = {"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥"};
    String chinese_numerals[] = {"零", "一", "二", "三","四","五","六","七", "八", "九", "十"};
    String solar_term[] = {"无节气", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑","立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};
    cnLunarDate cn_lunar;
    String cn_date = "";
    String cn_month = "";
    String cn_tidi = "";
    
    cn_tidi = tiangan[lunar.tiangan]  + dizhi[lunar.dizhi]; 
    if (lunar.lunarMonth < 10) {
        cn_month = chinese_numerals[lunar.lunarMonth] + "月";
    } else if (lunar.lunarMonth == 10) {
        cn_month = "十月";
    } else {
        cn_month = "十" + chinese_numerals[lunar.lunarMonth % 10] + "月";
    }
    if (lunar.isLeapMonth) {
        cn_month = "闰" + cn_month;
    } 
    if (lunar.lunarDay < 10) {
        cn_date = "初" + chinese_numerals[lunar.lunarDay];
    } else if (lunar.lunarDay == 10) {
        cn_date = "初十";
    } else if (lunar.lunarDay < 20) {
        cn_date  = "十" +  chinese_numerals[lunar.lunarDay % 10];
    } else if (lunar.lunarDay == 20) {
        cn_date = "廿十";
    } else if (lunar.lunarDay < 30) {
        cn_date = "廿" + chinese_numerals[lunar.lunarDay % 10];
    } else if (lunar.lunarDay == 30) {
        cn_date = "三十";
    }
    cn_lunar.cn_date = cn_date;
    cn_lunar.cn_month = cn_month;
    cn_lunar.cn_tidi = cn_tidi;
    cn_lunar.cn_term = solar_term[term];
    return cn_lunar;
}

char* LunarCalendar::convertSolarString(unsigned char solar_num) {
    // 节气
    char *solar_terms[] = {"无节气", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑","立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};
    char *solar_term = solar_terms[solar_num];
    return solar_term;
}

char* LunarCalendar::convertChineseString(uint32_t lunar_mm, uint32_t lunar_dd, bool is_leap_month) {
    static char lunar_date[50];
    const char *chinese_number[] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
    lunar_date[0] = '\0';  // 初始化字符串为空

    if (is_leap_month) {
        strncat(lunar_date, "闰", sizeof(lunar_date) - strlen(lunar_date) - 1);
    }

    // 月份转换
    switch (lunar_mm) {
        case 1:
            strncat(lunar_date, "正月", sizeof(lunar_date) - strlen(lunar_date) - 1);
            break;
        case 10:
            strncat(lunar_date, "十月", sizeof(lunar_date) - strlen(lunar_date) - 1);
            break;
        case 11:
            strncat(lunar_date, "冬月", sizeof(lunar_date) - strlen(lunar_date) - 1);
            break;
        case 12:
            strncat(lunar_date, "腊月", sizeof(lunar_date) - strlen(lunar_date) - 1);
            break;
        default:
            if (lunar_mm < 10) {
                char month_str[10];
                snprintf(month_str, sizeof(month_str), "%s月", chinese_number[lunar_mm]);
                strncat(lunar_date, month_str, sizeof(lunar_date) - strlen(lunar_date) - 1);
            }
            break;
    }

    // 日期转换
    if (lunar_dd < 10) {
        char day_str[10];
        snprintf(day_str, sizeof(day_str), "初%s", chinese_number[lunar_dd]);
        strncat(lunar_date, day_str, sizeof(lunar_date) - strlen(lunar_date) - 1);
    } else if (lunar_dd == 10) {
        strncat(lunar_date, "初十", sizeof(lunar_date) - strlen(lunar_date) - 1);
    } else if (lunar_dd < 20) {
        char day_str[10];
        snprintf(day_str, sizeof(day_str), "十%s", chinese_number[lunar_dd % 10]);
        strncat(lunar_date, day_str, sizeof(lunar_date) - strlen(lunar_date) - 1);
    } else if (lunar_dd == 20) {
        strncat(lunar_date, "廿十", sizeof(lunar_date) - strlen(lunar_date) - 1);
    } else if (lunar_dd < 30) {
        char day_str[10];
        snprintf(day_str, sizeof(day_str), "廿%s", chinese_number[lunar_dd % 10]);
        strncat(lunar_date, day_str, sizeof(lunar_date) - strlen(lunar_date) - 1);
    } else {
        strncat(lunar_date, "三十", sizeof(lunar_date) - strlen(lunar_date) - 1);
    }

    return lunar_date;
}