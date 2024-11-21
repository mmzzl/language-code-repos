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
        self.version = self.__context['version']
        if self.version not in ['sip-master-new', 'sip-master']:
            self.source = ""
        else:
            self.source = "platform"

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
    def template_target_path(self):
        """
        模板目标文件路径
        :return:
        """
        # 拼接文件路径
        customize_path = self.__context['path']['sip_customize_path']
        _target_path = self.__context['path']['target_path']
        # 目标文件
        target_directory = os.path.join(customize_path, _target_path,
                                        self.source, "update")
        return target_directory

    @property
    def template_path(self):
        """
        模板文件路径
        :return:
        """
        customize_path = self.__context['path']['sip_customize_path']
        _temp_path = self.__context['path']['temp_path']
        # 目标文件
        temp_directory = os.path.join(customize_path, _temp_path)
        return temp_directory

    @property
    def project_path(self):
        """
        项目文件路径
        :return:
        """
        _target_path = self.__context['path']['target_path']
        # 目标文件
        target_directory = os.path.join(_target_path,
                                        self.__context['filename'],
                                        )
        return target_directory

    @property
    def dst_path(self):
        """
        项目目标路径
        :return:
        """
        if self.version not in ['sip-master-new', 'sip-master']:
            dst_path = os.path.join(
                self.__context['path']['sip_customize_path'],
                self.__context['path']['target_path'],
                self.__context['filename'],
                self.version)
        else:
            dst_path = os.path.join(
                self.__context['path']['sip_customize_path'],
                self.__context['path']['target_path'],
                self.__context['filename'])
        return dst_path

    @property
    def update_path(self):
        """
        update目录路径
        :return:
        """
        dst_path = os.path.join(
            self.__context['path']['sip_customize_path'],
            self.__context['path']['target_path'],
            self.__context['filename'])

        update_filepath = os.path.join(str(dst_path), self.source, 'update')
        return update_filepath

    @property
    def platform_path(self):
        platform_path = os.path.join(str(self.dst_path), self.source)
        return platform_path
