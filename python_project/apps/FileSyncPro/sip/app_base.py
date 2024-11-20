# -*- coding: utf-8 -*-
import os
import shutil
from abc import ABCMeta, abstractmethod
from FileSyncPro.sip.context import Context
from git import Git, Repo, GitCommandError
from FileSyncPro.sip.logger import MyLog

from FileSyncPro.sip.file_modifier import FileModifier


class AppBase(metaclass=ABCMeta):
    app_config: dict = None
    delete_file_list = []
    modified_file_list = []

    def __init__(self, *args, **kwargs):
        self.ctx = Context(*args, **kwargs)
        self.origin_repo = Repo(self.ctx.path['sip_master_path'])
        self.origin_git = Git(self.ctx.path['sip_master_path'])
        self.file_modifier = FileModifier(self.ctx)
        self.customize_repo = Repo(self.ctx.path['sip_customize_path'])
        self.customize_git = Git(self.ctx.path['sip_customize_path'])
        self.log = MyLog(
            '{}-{}.log'.format(self.ctx.config['txt']['CUSTOM_NUMBER'],
                               self.ctx.config['txt']['CUSTOMER_NAME'])).log()

    def initialization(self):
        """
        初始化,模板文件代码提交
        :return:
        """
        self.log.info("检测当前分支是否干净.....")
        status = self.customize_git.status()
        if "nothing to commit" not in status:
            self.log.info("当前分支不干净，请先提交代码，再执行该脚本")
            raise AssertionError("当前分支不干净，未新增、删除、修改文件")
        # 不是第一次提交，不继续后续的操作
        if not self.ctx["first"]:
            self.customize_repo.git.checkout(self.ctx.branch)
            self.log.info("当前分支为: %s", self.customize_repo.active_branch)
            return
        self.log.info("切换分支到master.....")
        current_branch = self.customize_repo.active_branch
        if current_branch != 'master':
            self.customize_repo.git.checkout('master')
        self.log.info("当前分支为: %s", self.customize_repo.active_branch)
        try:
            if self.ctx["first"]:
                # 创建分支
                os.chdir(self.ctx.path['sip_customize_path'])
                git_cmd = "git push --delete origin {}".format(self.ctx.branch)
                os.system(git_cmd)
                self.customize_repo.git.branch('-D', self.ctx.branch)
                if os.path.exists(self.ctx.project_path):
                    shutil.rmtree(self.ctx.project_path)
        except GitCommandError as err:
            self.log.error("error: %s", err)
        # 拉取最新代码
        self.log.info("拉取最新代码.....")
        self.customize_repo.git.pull()
        # 创建分支
        try:
            self.customize_repo.git.branch(self.ctx.branch)
        except GitCommandError as err:
            self.log.error("error: %s", err)
            res = input("检查错误是否影响，是否继续？(y/n)")
            if res.lower() == 'n':
                raise AssertionError("分支创建失败")
        # 切换分支
        self.customize_repo.git.checkout(self.ctx.branch)
        self.log.info("当前分支为: %s", self.customize_repo.active_branch)
        # 复制模板文件
        self.file_modifier.copy_file_to_target(self.ctx.temp_path,
                                               self.ctx.target_path)
        # 修改模板文件
        self.file_modifier.modify_config_ini()
        self.file_modifier.modify_config_txt()
        self.file_modifier.modify_sh()
        # 重命名为客户中文名称
        self.file_modifier.rename_user()
        self.commit_message(self.ctx['template_commit_message'])
        # 查找改动的文件
        self.find_change_file()

    def commit_message(self, message):
        """
        提交代码
        :return:
        """
        # 提交基线记录
        os.chdir(self.ctx.target_path)
        git_status = self.customize_git.status()
        if "nothing to commit" in git_status:
            return
        os.system('git add .')
        os.system('git commit -m "{}"'.format(message))
        self.log.info("commit message: %s", message)

    def find_change_file(self):
        """
        查找改动的文件
        :return:
        """
        os.chdir(self.ctx.path['sip_master_path'])
        git_status = self.origin_git.status()
        if "nothing to commit" in git_status():
            return
        os.system('git add .')
        git_status = self.origin_git.status()
        status_list = git_status.split('\n')
        status_list = [i.replace('\t', '') for i in status_list]
        for item in status_list:
            tmp = item.split(":")
            if len(tmp) < 2:
                continue
            if 'appserver/static' in tmp[1] or 'www/ui' in tmp[1]:
                # 前端代码不做迁移
                continue
            if tmp[0] == 'deleted':
                self.delete_file_list.append(tmp[1].strip())
            elif tmp[0] == 'new file' or tmp[0] == 'modified':
                self.modified_file_list.append(tmp[1].strip())

    def baseline_commit(self):
        """
        基线代码提交
        :return:
        """
        os.chdir(self.ctx.path['sip_master_path'])
        os.system('git stash save "{}"'.format(""))
        for src_path in self.modified_file_list:
            self.file_modifier.copy_file_to_target(src_path, "")
        self.commit_message(self.ctx['base_commit_message'])

    def source_code_commit(self):
        """
        源代码提交
        :return:
        """
        os.chdir(self.ctx.path['sip_master_path'])
        os.system('git stash pop')
        for src_path in self.modified_file_list:
            self.file_modifier.copy_file_to_target(src_path, "")
        for delete_path in self.delete_file_list:
            os.remove(delete_path)
        # 执行命令
        if self.ctx.cmd:
            os.chdir(self.ctx.target_path)
            os.system('git add .')
            for cmd in self.ctx.cmd:
                os.system(cmd)
        self.commit_message(self.ctx['commit_message'])
        os.system('git push origin {}'.format(self.ctx.branch))
        self.log.info("代码提交成功: %s", self.ctx.branch)

    def run_action(self):
        self.initialization()
        self.baseline_commit()
        self.source_code_commit()
