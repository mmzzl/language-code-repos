# /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import logging
import logging.handlers


class Log(object):
    def __init__(self, log_name, log_level=logging.INFO):
        self.format_date = "%Y-%m-%d %H:%M:%S"
        self.format_log = "%(asctime)s %(levelname).1s %(process)d %(pathname)s:%(lineno)d (%(funcName)s)\t| %(message)s"
        self._init_file_log(log_name, log_level)
    
    def _init_file_log(self, log_name, level=logging.INFO):
        if not os.path.exists('./log'):
            os.makedirs('./log')
        log_name = './log/wealth_{}.log'.format(log_name)
        logging.basicConfig(level=level, 
                            format=self.format_log, 
                            datefmt=self.format_date, 
                            filename=log_name,
                            filemode='a')
        logger = logging.getLogger()
        logger.handlers = []
        # 输出到控制台
        console_handler = logging.StreamHandler()
        console_handler.setLevel(level)
        logger.addHandler(console_handler)


        # 输出到文件
        file_handler = logging.handlers.RotatingFileHandler(log_name, 
                                                            maxBytes=1024*1024*10,
                                                            backupCount=5
                                                            )
        file_handler.setLevel(level)
        logger.addHandler(file_handler)

        formatter = logging.Formatter(self.format_log)
        file_handler.setFormatter(formatter)
        console_handler.setFormatter(formatter)
        file_handler.setFormatter(formatter)

