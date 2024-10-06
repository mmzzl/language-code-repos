# -*- coding: utf-8 -*-
import logging

# 创建 logger
logger = logging.getLogger('logger')
logger.setLevel(logging.DEBUG)  # 设置日志级别

# 创建 handler 输出到控制台
console_handler = logging.StreamHandler()
console_handler.setLevel(logging.DEBUG)  # 控制台输出的日志级别

# 创建 handler 输出到文件
file_handler = logging.FileHandler('app.log', encoding="utf-8")
file_handler.setLevel(logging.DEBUG)  # 文件输出的日志级别

# 创建 formatter 并设置格式
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - [%(filename)s:%(lineno)d] - [%(module)s.%(funcName)s] - %(message)s')

# 为 handler 添加 formatter
console_handler.setFormatter(formatter)
file_handler.setFormatter(formatter)

# 为 logger 添加 handler
logger.addHandler(console_handler)
logger.addHandler(file_handler)
