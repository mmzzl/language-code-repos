# -*- coding: utf-8 -*-

import os
import configparser


class FileModifier(object):
    def __init__(self, ctx):
        self.ctx = ctx
        self.temp_path = self.ctx.temp_path
        # 目标文件
        self.target_path = self.ctx.target_path

    def modify_config_ini(self):
        """
        修改配置ini文件
        :return:
        """
        data = self.ctx.config['ini']

        file_names = data.keys()
        for filename in file_names:
            temp_join_path = os.path.join(self.temp_path, filename)
            config = configparser.ConfigParser()
            config.read(temp_join_path)
            for section_data in data[filename]:
                for section_name, section_values in section_data.items():
                    if not config.has_section(section_name):
                        config.add_section(section_name)
                    for key, value in section_values.items():
                        config.set(section_name, key, value)
            os.makedirs(self.target_path, exist_ok=True)
            target_join_path = os.path.join(self.target_path, filename)
            with open(target_join_path, 'w') as configfile:
                config.write(configfile)

    def modify_config_txt(self):
        """
        修改文本格式的文件
        :return:
        """
        data = self.ctx.config['txt']
        for filename, value in data.items():
            temp_join_path = os.path.join(self.temp_path,filename)
            target_join_path = os.path.join(self.target_path, filename)
            with open(temp_join_path, "w") as t:
                with open(temp_join_path, 'r') as f:
                    pass
