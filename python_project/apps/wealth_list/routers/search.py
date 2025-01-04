# -*- coding: utf-8 -*-
import logging
from fastapi import APIRouter, Depends
from dependencies import get_current_active_user
from database import get_db
from models.wealth import Wealth
import baostock as bs
import pandas as pd
from pydantic import BaseModel
from enum import Enum
from datetime import datetime
from models.user import User
from sqlalchemy.orm import Session
lg = bs.login()

logging.info("login response error_code: %s, error_msg: %s", lg.error_code, lg.error_msg)


router = APIRouter()

class Period(Enum):
    week  = "week"
    month = "month"

class ProfitParam(BaseModel):
    name: str
    start_date: str
    end_date: str
    peroid: Period


async def query_history_k_data_plus(
    stock_code, 
    start_date,
    end_date,
    field = None
):
    if not field:
        field = "date,code,open,high,low,close,preclose,volume,amount,adjustflag,turn,tradestatus,pctChg,isST" 
    rs = bs.query_history_k_data_plus(stotck_code, field, start_date, end_date, frequency="d", adjustflag="3")
    logging.info("response error_code: %s, error_msg: %s", rs.error_code, rs.error_msg)
    data_list = []
    while(rs.error_code == '0') & rs.next():
        data_list.append(rs.get_row_data())
    result = pd.DataFrame(data_list, columns=rs.fields)
    return result

async def calculate_profit(df, price, amount):
    echarts_data = {"xAxis":[], "yAxis": []}
    df['profit'] = (df['colse'] * amount)  - (price * amount)
    return echarts_data


@router.post("/api/v1/profit")
async def profit(param: ProfitParam, current_user: User = Depends(get_current_active_user), db: Session = Depends(get_db)):
    """
    计算收益
    """
    # 只查询周和月
    # 计算收益的方法，当前收盘价 * 买入的数量 - (买入价格 * 买入的数量) 
    record = db.query(Wealth).filter(user=current_user.user_id, name=param.name).first()
    if record is None:
        return {"message": "Record not found", "success": False}
    # 获取购买日期
    purchase_at = record.purchase_at.strftime("%Y-%m-%d")
    df = await query_history_k_data_plus(name, param.start_date, param.end_date)
    if param.peroid == 'week':
        resample_df = df.resample("W")
    else:
        resample_df = df.resample('M')
    echarts_data = await calculate_profit(resample_df, record.price, record.amount)
    return {"success": True, "data": echarts_data}