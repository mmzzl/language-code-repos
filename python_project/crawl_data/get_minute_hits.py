# -*- coding: utf-8 -*-
import copy
import os
import re
import json
import time
import traceback

import pandas as pd
from datetime import datetime, timedelta

import requests
import asyncio
import aiohttp
from collections import deque

from openpyxl.styles.builtins import total

from crawl_data.log import logger


def string_to_json(content):
    match_str = re.search(r".*\((.*)\)", content)
    if not match_str:
        logger.info("no match: %s", content)
        return
    return json.loads(match_str.group(1))


def send_request(page_size, fs):
    url = "https://9.push2.eastmoney.com/api/qt/clist/get?"
    params = {
        "cb": "jQuery11240015358652372567727_1728003713082",
        "pn": 1,
        "pz": page_size,
        "np": 1,
        "ut": "bd1d9ddb04089700cf9c27f6f7426281",
        "fltt": 2,
        "invt": 2,
        "dect": 1,
        "wbp2u": "|0|0|0|web",
        "fid": "f3",
        "fs": fs,
        "fields": "f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f12,f13,f14,f15,f16,f17,f18,f20,f21,f23,f24,f25,f22,f11,f62,f128,f136,f115,f152",
        "_": int(time.time() * 1000)
    }
    headers = {
        "Referer": "https://quote.eastmoney.com/",
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36 Edg/129.0.0.0"
    }
    response = requests.get(url, headers=headers, data=params)
    content = response.text
    match_str = re.search(r".*\((.*)\)", content)
    if not match_str:
        logger.info("no match: %s", content)
        return
    req_json = json.loads(match_str.group(1))
    return req_json


def get_all_stock_code():
    """
    获取所有股票代码
    :return:
    """

    # 提取总数, 然后循环，取所有数据
    try:
        # 获取所有深圳股票代码
        fs = "m:0+t:6,m:0+t:80"
        req_json = send_request(1, fs)
        shenzhen_total = req_json['data']['total']
        # 保存第一页的数据
        stock_code_set = set()
        req_json = send_request(shenzhen_total, fs)
        for item in req_json['data']['diff']:
            stock_code_set.add(item['f12'])
        # 获取上证所有股票代码
        fs = "m:1+t:2,m:1+t:23"
        req_json = send_request(1, fs)
        shangzheng_total = req_json['data']['total']
        req_json = send_request(shangzheng_total, fs)
        for item in req_json['data']['diff']:
            stock_code_set.add(item['f12'])
        # 保存所有的股票代码
        with open("code.txt", "w") as f:
            f.write(json.dumps(list(stock_code_set)))
        assert (shenzhen_total + shangzheng_total) == len(stock_code_set)
    except Exception as err:
        import traceback
        logger.error(traceback.format_exc())
        logger.error("request error: %s", err)

def get_stock_exchange(stock_code):
    if stock_code.startswith('60'):
        return 1
    elif stock_code.startswith('688'):
        return 1
    elif stock_code.startswith('900'):
        return 1
    elif stock_code.startswith('000') or stock_code.startswith('001'):
        return 0
    elif stock_code.startswith('002'):
        return 0
    elif stock_code.startswith('300'):
        return 0


# 因为请求次数很多,采用异步限流限速的方式获取分钟数据
async def fetch_stock_detail(session, stock_code):
    type_param = get_stock_exchange(stock_code)
    url = "https://push2his.eastmoney.com/api/qt/stock/kline/get?"
    params = {
        "fields1": "f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13",
        "fields2": "f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61",
        "beg": 0,
        "end": 20500101,
        "ut": "fa5fd1943c7b386f172d6893dbfba10b",
        "rtntype": 6,
        "secid": "{}.{}".format(type_param, stock_code),
        "klt": 5,
        "fqt": 1,
        "cb": "jsonp{}".format(int(time.time() * 1000))
    }
    headers = {
        "Referer": "https://quote.eastmoney.com/",
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36 Edg/129.0.0.0"
    }
    async with session.get(url, data=params, headers=headers) as response:
        if response.status == 200:
            data = await response.text()
            return data
        else:
            response.raise_for_status()


# 限速装饰器
class RateLimiter:
    def __init__(self, rate_limit, period):
        self.rate_limit = rate_limit  # 每个周期最大请求数
        self.period = period  # 限速周期
        self.tokens = self.rate_limit
        self.last_refill_time = datetime.now()

    async def __aenter__(self):
        while self.tokens <= 0:
            await asyncio.sleep((self.period - (
                    datetime.now() - self.last_refill_time)).total_seconds())
            self.tokens = self.rate_limit
            self.last_refill_time = datetime.now()
        self.tokens -= 1

    async def __aexit__(self, exc_type, exc, tb):
        pass


async def save_stock_data(filename, df: pd):
    """把数据保存到csv文件中用于数据分析
    """
    # 判断文件夹是否存在
    if not os.path.exists("data"):
        os.makedirs("data")
    # 文件命名用日期进行命名
    filepath = os.path.join('data', filename)
    df.to_csv(filepath, index=False, encoding="utf-8", na_rep="N/A")


async def format_data(content):
    """把数据转换为pandas格式的数据
    """
    json_data = string_to_json(content)
    if not json_data.get('data'):
        return False
    columns = ["datetime", "code", "name", "open", "close", "high", "low",
               "turnover", "volume"]
    data = json_data['data']

    rows = []
    for item in data['klines']:
        kline = item.split(",")
        trends = {"datetime": kline[0],
                  "code": data['code'],
                  "name": data['name'],
                  "open": kline[1],
                  "close": kline[2],
                  "high": kline[3],
                  "low": kline[4],
                  "turnover": kline[5],
                  "volume": kline[6]
                  }
        rows.append(copy.deepcopy(trends))
    df = pd.DataFrame(rows, columns=columns)
    return df


async def get_five_minutes_hits():
    if not os.path.exists("code.txt"):
        get_all_stock_code()
    with open("code.txt") as f:
        stock_codes = json.loads(f.read())
    # 设置并发数量
    concurrency_limit = 100
    sem = asyncio.Semaphore(concurrency_limit)
    # 限速设置
    # 每秒最多10个请求
    rate_limiter = RateLimiter(rate_limit=10, period=timedelta(seconds=1))
    # 创建一个任务队列
    task_queue = deque(stock_codes)
    dataframes = []
    async with aiohttp.ClientSession() as session:
        while task_queue:
            # 从队列中取出一个任务
            stock_code = task_queue.popleft()
            # 获取信号量
            async with sem:
                # 限速
                async with rate_limiter:
                    try:
                        result = await fetch_stock_detail(session, stock_code)
                        df = await format_data(result)
                        if isinstance(df, bool) and df is False:
                            continue
                        dataframes.append(df)
                    except Exception as e:
                        logger.error(traceback.format_exc())
                        logger.error("Error fetching data for %s: %s",
                                     stock_code, e)
        # 所有股票请求完成后, 保存到csv文件中
        filename = "{}.csv".format(datetime.strptime(dataframes[0].iloc[0][
                                                         'datetime'],
                                                     "%Y-%m-%d "
                                                     "%H:%M").strftime(
            "%Y%m%d"))
        await save_stock_data(filename, pd.concat(dataframes))


if __name__ == '__main__':
    start_time = time.time()
    asyncio.run(get_five_minutes_hits())
    logger.info("运行完成,用时: %s", (time.time() - start_time) / 60)
