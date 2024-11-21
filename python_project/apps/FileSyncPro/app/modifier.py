# -*- coding: utf-8 -*-

import os
import shutil
import configparser


class FileModifier(object):
    """
    文件修改器
    """

    def __init__(self, ctx):
        self.ctx = ctx
        self.temp_path = self.ctx.template_path
        # 目标文件
        self.target_path = self.ctx.template_target_path

    def modify_config_ini(self):
        """
        修改配置ini文件
        :return:
        """
        data = self.ctx.config['ini']
        file_names = data.keys()
        for filename in file_names:
            update_filepath = os.path.join(self.ctx.update_path, filename)
            config = configparser.ConfigParser()
            with open(update_filepath, 'r', encoding='utf-8') as configfile:
                config.read_file(configfile)
            for section_data in data[filename]:
                for section_name, section_values in section_data.items():
                    if not config.has_section(section_name):
                        config.add_section(section_name)
                    for key, value in section_values.items():
                        config.set(section_name, key, value)
            with open(update_filepath, 'w', encoding="utf-8") as configfile:
                config.write(configfile)

    def modify_config_txt(self):
        """
        修改文本格式的文件
        :return:
        """
        data = self.ctx.config['txt']
        for filename, value in data.items():
            data_list = []
            update_filepath = os.path.join(self.ctx.update_path, filename)
            with open(update_filepath, "r", encoding="utf-8") as f:
                lines = f.readlines()
            for line in lines:
                context = line.split("=")
                context = [i.strip() for i in context if i]
                if len(context) == 1:
                    data_list.append(line.strip())
                    continue
                key, value = context
                if not data[filename].get(key):
                    data_list.append(line.strip())
                    continue
                data_list.append("{} = {}".format(key, data[filename][key]))
            with open(update_filepath, "w", encoding="utf-8") as f:
                f.write("\n".join(data_list))

    def modify_sh(self):
        """
        修改appsh文件
        :return:
        """
        data = self.ctx.config['sh']
        for filename, item in data.items():
            update_filepath = os.path.join(self.ctx.update_path, filename)
            with open(update_filepath, "r", encoding="utf-8") as f:
                lines = f.readlines()
            data_list = []
            if item["cmd"]:
                for line in lines:
                    if line.strip() != item['start_write_line']:
                        data_list.append(line.strip())
                        continue
                    data_list.append("")
                    for cmd in item['cmd']:
                        data_list.append(cmd)
                with open(update_filepath, "w", encoding="utf-8") as f:
                    f.write("\n".join(data_list))

    @staticmethod
    def copy_file_to_target(source_dir, destination_dir):
        """
        复制文件到目标目录
        :return:
        """
        if not os.path.exists(os.path.dirname(destination_dir)):
            os.makedirs(os.path.dirname(destination_dir))
        shutil.copytree(source_dir, destination_dir, dirs_exist_ok=True)

    def rename_file(self):
        """
        重新命名文件名
        :return:
        """
        rename_filepath = os.path.join(self.ctx.dst_path,
                                       "重命名为客户中文名称")
        user_filepath = os.path.join(self.ctx.dst_path, self.ctx.user)
        if not os.path.exists(user_filepath):
            if not os.path.exists(rename_filepath):
                with open(os.path.join(self.ctx.dst_path,
                                       self.ctx.user),
                          'w') as f:
                    pass
            else:
                os.rename(rename_filepath, user_filepath)
