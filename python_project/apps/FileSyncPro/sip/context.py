# -*- coding: utf-8 -*-
import os


def parse_param(*args, **kwargs) -> dict:
    param = args[0] if args else kwargs
    if not param:
        raise Exception("配置文件错误")
    return param


class Context(object):
    def __init__(self, *args, **kwargs):
        self.__context = parse_param(*args, **kwargs)

    def __len__(self):
        return len(self.__context)

    def __getitem__(self, item):
        return self.__context[item]

    def __setitem__(self, key, value):
        return self.__context.__setitem__(key, value)

    def __contains__(self, item):
        return item in self.__context

    def items(self):
        return self.__context.items()

    @property
    def config(self):
        return self.__context['config']

    @property
    def path(self):
        return self.__context['path']

    @property
    def branch(self):
        return self.__context['branch']

    @property
    def sh(self):
        return self.__context['sh']

    @property
    def cmd(self):
        return self.__context['cmd']

    @property
    def user(self):
        return self.__context['user']

    @property
    def target_path(self):
        # 拼接文件路径
        customize_path = self.__context['path']['customize_path']
        _target_path = self.__context['path']['target_path']
        # 目标文件
        target_directory = os.path.join(customize_path, _target_path,
                                        "platform", "update")
        return target_directory

    @property
    def temp_path(self):
        customize_path = self.__context['path']['customize_path']
        _temp_path = self.__context['path']['temp_path']
        # 目标文件
        temp_directory = os.path.join(customize_path, _temp_path,
                                      "platform", "update")
        return temp_directory
