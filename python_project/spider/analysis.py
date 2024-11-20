# -*- coding: utf-8 -*-
import logging
import json
import asyncio
from datetime import datetime, timedelta
import traceback
import aiohttp
import config
from collections import deque
import abc


class RateLimiter:
    """
    限速器
    """

    def __init__(self, rate_limit, period):
        self.rate_limit = rate_limit
        self.tokens = self.rate_limit
        self.period = period
        self.last_refill_time = datetime.now()  # 上次填充令牌的时间

    async def refill_tokens(self):
        """
        根据时间差补充令牌
        """
        now = datetime.now()
        elapsed_time = (now - self.last_refill_time).total_seconds()
        tokens_to_add = elapsed_time * (self.rate_limit / self.period)
        if tokens_to_add > 0:
            self.tokens = min(self.rate_limit, self.tokens + tokens_to_add)
            self.last_refill_time = now

    async def __aenter__(self):
        await self.refill_tokens()
        while self.tokens <= 0:  # 如果没有令牌，则等待
            await asyncio.sleep(0.1)  # 1秒10次并发
            await self.refill_tokens()
        self.tokens -= 1

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        pass


class BaseSpider(metaclass=abc.ABCMeta):
    def __init__(self):
        self.ctx = ""
        self.code = None

    @abc.abstractmethod
    async def send_request(self):
        return

    @abc.abstractmethod
    async def parse_response(self, response):
        return

    @abc.abstractmethod
    async def save_result(self, result):
        pass

    @abc.abstractmethod
    async def get_request_list(self):
        pass

    async def run_action(self):
        """
        执行蜘蛛程序
        """
        with open("code.txt") as f:
            # 股票代码
            codes = json.loads(f.read())
        sem = asyncio.Semaphore(config.CONCURRENCY_LIMIT)
        # 加入队列
        task_queue = deque(codes)
        rate_limiter = RateLimiter(rate_limit=10, period=timedelta(seconds=1))
        async with aiohttp.ClientSession() as session:
            while task_queue:
                # 从队列中取出一个任务
                code = task_queue.popleft()
                self.code = code
                # 获取信号量
                async with sem:
                    # 限速
                    async with rate_limiter:
                        try:
                            response = await self.send_request()
                            result = await self.parse_response(response)
                            await self.save_result(result)
                        except Exception as error:
                            logging.error(traceback.format_exc())
                            logging.error("error %s", error)
