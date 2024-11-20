# -*- coding: utf-8 -*-

import os
import shutil
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
            with open(target_join_path, 'w', encoding="utf-8") as configfile:
                config.write(configfile)

    def modify_config_txt(self):
        """
        修改文本格式的文件
        :return:
        """
        data = self.ctx.config['txt']
        for filename, value in data.items():
            data_list = []
            target_join_path = os.path.join(self.target_path, filename)
            with open(target_join_path, "r", encoding="utf-8") as f:
                lines = f.readlines()
            for line in lines:
                context = line.split("=")
                if len(context) == 1:
                    data_list.append(line.strip())
                    continue
                key, value = context
                if not data[filename].get(key):
                    data_list.append(line.strip())
                    continue
                data_list.append("{} = {}".format(key, data[filename][key]))
            with open(target_join_path, "w", encoding="utf-8") as f:
                f.write("\n".join(data_list))

    def modify_sh(self):
        """
        修改appsh文件
        :return:
        """
        data = self.ctx.config['sh']
        for filename, item in data.items():
            target_join_path = os.path.join(self.target_path, filename)
            with open(target_join_path, "r", encoding="utf-8") as f:
                lines = f.readlines()
            data_list = []
            for line in lines:
                if line.strip() != item['start_write_line']:
                    data_list.append(line.strip())
                    continue
                data_list.append("\n")
                data_list.append(item['cmd'])
            with open(target_join_path, "w", encoding="utf-8"):
                f.write("\n".join(data_list))

    @staticmethod
    def copy_file_to_target(src, dst):
        """
        复制文件到目标目录
        :return:
        """
        shutil.copy(src, dst)

    def rename_user(self):
        user_filepath = os.path.join(self.ctx.target_path, self.ctx.user)
        if not os.path.exists(user_filepath):
            return os.rename(user_filepath, self.ctx.user)
