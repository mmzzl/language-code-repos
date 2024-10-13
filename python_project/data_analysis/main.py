# -*- coding: utf-8 -*-

import pandas as pd
import backtrader as bt
import numpy as np
from python_project.crawl_data import logger


def simple_strategy():
    """
    简单的量化的策略
    上涨5% 买入， 下降3%卖出
    """
    df = pd.read_csv("../crawl_data/data/20240815.csv")
    # 计算上涨幅度
    df.sort_values(by='datetime')
    df['pct_change'] = df['close'].pct_change() * 100

    # 筛选上涨5% 和下跌超过3%的股票
    up_5_percent = df[df['pct_change'] > 5]
    down_3_percent = df[df['pct_change'] < -3]
    print(up_5_percent, 'this is up_5_percent')
    print(down_3_percent)


class CustomPandasData(bt.feeds.PandasData):
    lines = ('turnover',)
    params = (
        # 默认字段
        ('datetime', 'datetime'),  # 假设日期时间列名为 "Date"
        ('open', 'open'),
        ('high', 'high'),
        ('low', 'low'),
        ('close', 'close'),
        ('volume', 'volume'),
        # 自定义字段
        ('turnover', 'turnover'),  # 假设有一个自定义列名为 "CustomColumn"
    )

    datafields = [
        'datetime', 'open', 'close', 'high', 'low', 'turnover', 'volume'
    ]


class EMV(bt.Indicator):
    lines = ('emv',)
    params = (('period', 14),)

    def __init__(self):
        pass

    def next(self):
        # 计算中间价
        print(self.params.period)
        high = self.data.high.get(size=self.params.period)
        low = self.data.low.get(size=self.params.period)
        volume = self.data.turnover.get(size=self.params.period)

        if len(high) < self.params.period or len(
                low) < self.params.period or len(volume) < self.params.period:
            return

        high = np.array(high)
        low = np.array(low)
        volume = np.array(volume)

        midpoint = (high + low) / 2
        # 将成交量为0的值替换为1，避免除以0
        volume[volume == 0] = 1
        # 计算中间价的变化
        midpoint_change = midpoint - np.roll(midpoint, 1)
        # 计算箱体比值
        box_ratio = midpoint_change / (volume / (high - low))
        # 初始化EMV
        self.lines.emv[0] = np.mean(box_ratio[-self.params.period:])


class TestStrategy(bt.Strategy):
    params = (
        ('period', 14),
    )

    def log(self, txt, dt=None):
        dt = dt or self.datas[0].datetime.date(0)
        logger.info("%s, %s", dt.isoformat(), txt)

    def __init__(self):
        self.dataclose = self.datas[0].close
        # 计算中间价

        self.order = None
        self.buyprice = None
        self.buycomm = None
        self.emv = EMV(period=self.params.period)

    def notify_order(self, order):
        if order.status in [order.Submitted, order.Accepted]:
            return
        if order.status in [order.Completed]:
            if order.isbuy():
                self.log("BUY EXECUTED,Price: %.2f, Cost: %.2f, Comm %.2f" %
                         (order.executed.price,
                          order.executed.value,
                          order.executed.comm)
                         )
                self.buyprice = order.executed.price
                self.buycomm = order.executed.comm
            elif order.issell():
                self.log("BUY EXECUTED, %.2f, Cost: %.2f, Comm %.2f" %
                         (order.executed.price,
                          order.executed.value,
                          order.executed.comm
                          )
                         )
            self.bar_executed = len(self)
        elif order.status in [order.Canceled, order.Margin, order.Rejected]:
            self.log("Order Canceled/Margin/Rejected")
        self.order = None

    def notify_trade(self, trade):
        if not trade.isclosed:
            return
        self.log("OPERATION PROFIT, GROSS %.2f, NET %.2f" % (trade.pnl,
                                                             trade.pnlcomm))

    def next(self):
        self.log("Close, %.2f" % self.dataclose[0])
        if self.order:
            return
        if not self.position:
            if self.emv[0] < 0:
                self.log("BUY CREATE, %.2f" % self.dataclose[0])
                self.order = self.buy()
        elif self.emv[0] > 0:
            self.sell()


if __name__ == '__main__':
    # simple_strategy()
    cerebro = bt.Cerebro()
    cerebro.addstrategy(TestStrategy)
    data = pd.read_csv("../crawl_data/data/20240815.csv")
    data['datetime'] = pd.to_datetime(data['datetime'])
    data['Date'] = pd.to_datetime(data['datetime'])
    data.set_index('Date', inplace=True)
    data = data[data['code'] == 601882]
    # 将DataFrame 转换为backtrader的DataFeed
    data_feed = CustomPandasData(dataname=data)
    cerebro.adddata(data_feed)

    cerebro.broker.setcash(100000.0)
    cerebro.broker.setcommission(commission=0.001)
    print("Starting Portfolio Value: %.2f" % cerebro.broker.getvalue())
    cerebro.run()
    print("Final Portfolio Value: %.2f" % cerebro.broker.getvalue())
    cerebro.plot()
