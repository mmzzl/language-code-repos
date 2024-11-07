# -*- coding: utf-8 -*-
from abc import ABCMeta, abstractmethod

from FileSyncPro.sip.context import Context


class AppBase(metaclass=ABCMeta):
    app_config: dict = None

    def __init__(self, *args, **kwargs):
        self.ctx = Context(*args, **kwargs)

    def run_action(self):
        pass

    def modifier_file(self):
        """
        对文件进行改动
        :return:
        """
        config = self.ctx.config
