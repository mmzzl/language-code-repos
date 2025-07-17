# -*- coding: utf-8 -*-
from rest_framework.pagination import PageNumberPagination


class CustomPagination(PageNumberPagination):
    page_size = 10  # 默认每页数量
    page_size_query_param = 'pagesize' # 前端传入的每页数量参数名
    max_page_size = 100  # 最大每页数量限制
    page_query_param = 'page'  # 前端传入的页码参数名