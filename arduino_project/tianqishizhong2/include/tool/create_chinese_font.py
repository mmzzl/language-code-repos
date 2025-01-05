# -*- coding: utf-8 -*-
import json
def create_chinese_font():
    result = []
    data = "多少云四川省广东省广安市深圳市南山区廿一二三四五六七八九十晴阴雨雾大小雪春夏秋冬立初正月星期周末中暴立春水惊蛰分清明谷满芒种至暑处暑白露秋分寒露霜降至寒子鼠丑牛寅虎卯兔辰龙巳蛇午马未羊申猴酉鸡戌狗亥猪甲乙丙丁戊己庚辛壬癸"
    for i in data:
        result.append(hex(ord(i)))
    print(",".join(result))



create_chinese_font()