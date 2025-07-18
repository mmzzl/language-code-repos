from django.shortcuts import render
from .dlt import get_dlt_from_500
import pandas as pd
from collections import defaultdict
from django.http import JsonResponse

# Create your views here.

# 定义红球和蓝球的范围
RED_BALL_RANGE = (1, 35)
BLUE_BALL_RANGE = (1, 12)


def lottery_analysis(request):
    all_data = get_dlt_from_500()
    df = pd.DataFrame(all_data)
    # 初始化统计字典
    column_stats = defaultdict(lambda: defaultdict(int))

    # 统计每列每个数字出现的次数
    columns_to_analyze = [f'red_{i}' for i in range(1, 6)] + [f'blue_{i}' for i
                                                              in
                                                              range(1, 3)]
    for column in columns_to_analyze:
        for num in df[column]:
            column_stats[column][num] += 1
    # 计算每列的总抽奖次数（等于数据行数）
    total_draws = len(df)
    # 准备结果DataFrame
    results = []

    for column in columns_to_analyze:
        # 确定数字范围
        num_range = RED_BALL_RANGE if 'red' in column else BLUE_BALL_RANGE

        for num in range(num_range[0], num_range[1] + 1):
            count = column_stats[column].get(num, 0)
            probability = count / total_draws * 100  # 转换为百分比

            results.append({
                '列名': column,
                '数字': num,
                '出现次数': count,
                '出现概率(%)': f"{probability:.2f}%"
            })

    # 创建结果DataFrame
    result_df = pd.DataFrame(results)

    # 按列名和出现次数排序
    result_df = result_df.sort_values(['列名', '出现次数'],
                                      ascending=[True, False])
    data = result_df.to_dict(orient='records')
    return JsonResponse(data, safe=False)
