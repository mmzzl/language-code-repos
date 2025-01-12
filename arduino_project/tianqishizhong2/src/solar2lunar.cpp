#include "Solar2Lunar.h"

cnDate Solar2Lunar(unsigned short yyyy, unsigned char mm, unsigned char dd)
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

unsigned char Solar2Term(unsigned short yyyy, unsigned char mm, unsigned char dd)
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

cnLunarDate Lunar2Chinese(cnDate lunar, unsigned char term) {
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
