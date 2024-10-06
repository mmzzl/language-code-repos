# -*- coding: utf-8 -*-

import pandas as pd


test_data = "stock_test_20241003_data.xlsx"
df = pd.read_excel(test_data)
df['日期'] = pd.to_datetime(df['日期'], format="%Y-%m-%d %H:%M:%S")
df.rename(columns={"日期": "datetime", "代码": "code", "名称": "name"}, inplace=True)
df.set_index("datetime", inplace=True)
print(df)
search_code =df[df['code'] == 688615]

resample_df = search_code.resample("15T").last()
print(resample_df)
# print(resample_df)
