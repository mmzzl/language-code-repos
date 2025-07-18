import os
import time
import requests
import pandas as pd


def get_dlt_from_500():
    """
    从体彩彩票网获取大乐透数据
    :return: DataFrame格式的大乐透数据
    """
    all_data = []
    url = ('https://webapi.sporttery.cn/gateway/lottery/getHistoryPageListV1'
           '.qry?gameNo=85&provinceId=0&pageSize={size}&isVerify=0&pageNo={'
           'page}')
    base_url = url
    page = 1
    size = 30
    headers = {
        'referer': 'https://static.sporttery.cn/',
        'user-agent': (
            'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 ('
            'KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36 Edg/138.0.0.0')
    }
    if os.path.exists('dlt.csv'):
        df = pd.read_csv('dlt.csv')
        all_data = df.to_dict(orient='records')
        total = 30
    else:
        res = requests.get(base_url.format(page=page, size=size), verify=False,
                           headers=headers, timeout=10)
        response = res.json()
        total = response.get('value').get('total', 0)
    while (page * size) <= total:
        try:
            res = requests.get(base_url.format(page=page, size=size),
                               headers=headers,
                               verify=False,
                               timeout=10)
            response = res.json()
            for item in response['value']['list']:
                red_1, red_2, red_3, red_4, red_5, blue_1, blue_2 = item[
                    'lotteryDrawResult'].split(' ')
                all_data.append({
                    'lotteryDrawTime': item['lotteryDrawTime'],
                    'lotteryDrawNum': item['lotteryDrawNum'],
                    'lotteryDrawResult': item['lotteryDrawResult'],
                    'red_1': red_1,
                    'red_2': red_2,
                    'red_3': red_3,
                    'red_4': red_4,
                    'red_5': red_5,
                    'blue_1': blue_1,
                    'blue_2': blue_2
                })
            page += 1
            time.sleep(1)  # 礼貌性延迟
        except Exception as e:
            print(e)
            continue
    df = pd.DataFrame(all_data)
    df.to_csv('dlt.csv', index=False, encoding='utf_8_sig')
    return all_data
