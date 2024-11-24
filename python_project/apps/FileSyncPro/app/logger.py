# -*- coding: utf-8 -*-
import os
import logging


class MyLog:
    def __init__(self, file_name):
        if not os.path.exists('log'):
            os.mkdir('log')
        self.file_name = os.path.join('log', file_name)

    def log(self):
        logger = logging.getLogger(__name__)
        logger.setLevel(level=logging.INFO)
        handler = logging.FileHandler(self.file_name, encoding='utf-8')
        handler.setLevel(logging.INFO)
        formatter = logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        handler.setFormatter(formatter)
        logger.addHandler(handler)
        console = logging.StreamHandler()
        console.setLevel(logging.INFO)
        formatter = logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        console.setFormatter(formatter)
        logger.addHandler(console)
        return logger
