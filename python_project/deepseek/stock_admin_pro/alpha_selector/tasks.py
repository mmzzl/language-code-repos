# -*- coding: utf-8 -*-
from celery import shared_task
import requests 

@shared_task
def get_stock_daily_data():
    """
    获取股票日线数据
    """
    pass
