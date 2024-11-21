# -*- coding: utf-8 -*-
import os
import shutil

from .context import Context
from git import Git, Repo, GitCommandError
from .logger import MyLog

from .modifier import FileModifier


class AppBase(object):
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
            '{}-{}.log'.format(
                self.ctx.config['txt']['pkgrc']['CUSTOM_NUMBER'],
                self.ctx.config['txt']['pkgrc']['CUSTOMER_NAME'])).log()

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
        except Exception as err:
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

    def commit_message(self, message):
        """
        提交代码
        :return:
        """
        # 提交基线记录
        os.chdir(self.ctx.path['sip_customize_path'])
        git_status = self.customize_git.status()
        if "nothing to commit" in git_status:
            self.log.info("当前分支干净，无需提交")
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
        if "nothing to commit" in git_status:
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
        self.log.info("modified file list: %s", self.modified_file_list)
        self.log.info("delete file list: %s", self.delete_file_list)

    def custom_template_commit(self):
        """
        定制模板文件提交
        :return:
        """
        # 复制模板文件
        self.log.info("复制模板文件%s 到目标文件夹%s", self.ctx.template_path,
                      self.ctx.dst_path)
        if self.ctx.version not in ['sip-master', 'sip-master-new']:
            self.file_modifier.copy_file_to_target(self.ctx.template_path,
                                                   os.path.dirname(
                                                       self.ctx.dst_path))
        else:
            self.file_modifier.copy_file_to_target(self.ctx.template_path,
                                                   self.ctx.dst_path)
        # 修改模板文件
        self.file_modifier.modify_config_ini()
        self.file_modifier.modify_config_txt()
        self.file_modifier.modify_sh()
        # 重命名为客户中文名称
        self.file_modifier.rename_file()
        self.commit_message(self.ctx['template_commit_message'])

    def baseline_commit(self):
        """
        基线代码提交
        :return:
        """
        self.log.info("基线代码开始提交.....")
        os.chdir(self.ctx.path['sip_master_path'])
        stash_msg = "{}{}".format(self.ctx.user,
                                  '临时保存的定制信息')
        self.log.info("stash  message: %s", stash_msg)
        os.system('git stash save "{}"'.format(stash_msg))
        for path in self.modified_file_list:
            self.log.info("modified file: %s", path)
            path = os.path.normpath(path)
            src_path = os.path.join(self.ctx.path['sip_master_path'], path)
            if "srcnew" in path:
                path = path.replace("srcnew\\", "")
            elif 'newbuild\\runtime\\root\\' in path:
                path = path.replace('newbuild\\runtime\\root\\',
                                    'SIP-INSTALLROOT\\')
            elif 'srcnew\\dmoc\\etc\\' in path:
                path = path.replace('srcnew\\dmoc\\etc\\', 'SIP-INSTALLROOT\\')
            platform_path = os.path.join(self.ctx.platform_path, path)
            if not os.path.exists(os.path.dirname(platform_path)):
                os.makedirs(os.path.dirname(platform_path))
            if os.path.exists(src_path):
                shutil.copyfile(src_path, platform_path)
        self.commit_message(self.ctx['base_commit_message'])

    def source_code_commit(self):
        """
        源代码提交
        :return:
        """
        os.chdir(self.ctx.path['sip_master_path'])
        os.system('git stash pop')
        for path in self.modified_file_list:
            path = os.path.normpath(path)
            src_path = os.path.join(self.ctx.path['sip_master_path'], path)
            if "srcnew" in path:
                path = path.replace("srcnew\\", "")
            elif 'newbuild\\runtime\\root\\' in path:
                path = path.replace('newbuild\\runtime\\root\\',
                                    'SIP-INSTALLROOT\\')
            elif 'srcnew\\dmoc\\etc\\' in path:
                path = path.replace('srcnew\\dmoc\\etc\\', 'SIP-INSTALLROOT\\')
            platform_path = os.path.join(self.ctx.platform_path, path)
            if not os.path.exists(os.path.dirname(platform_path)):
                os.makedirs(os.path.dirname(platform_path))
            if os.path.exists(src_path):
                shutil.copyfile(src_path, platform_path)

        for delete_path in self.delete_file_list:
            delete_path = os.path.normpath(delete_path)
            if os.path.exists(
                    os.path.join(self.ctx.dst_path, delete_path)):
                os.remove(os.path.join(self.ctx.dst_path, delete_path))
        # 执行命令
        if self.ctx.cmd:
            os.chdir(self.ctx.template_target_path)
            os.system('git add .')
            for cmd in self.ctx.cmd:
                os.system(cmd)
        self.commit_message(self.ctx['commit_message'])
        os.system('git push origin {}'.format(self.ctx.branch))
        self.log.info("代码提交成功: %s", self.ctx.branch)

    def action(self):
        self.initialization()
        self.custom_template_commit()
        self.find_change_file()
        # 如果第一次提交，则需要提交基线代码
        if self.ctx["first"] is True:
            self.baseline_commit()
        self.source_code_commit()

    def run_action(self):
        try:
            self.action()
        except Exception as err:
            print(err, '==========================')
            import traceback
            self.log.warning('=' * 50 + '报错信息如下：')
            self.log.warning(traceback.format_exc())
            self.log.warning('=' * 50 + '请检查错误并修改后重新执行！')
            # 异常的时候回滚，然后检查错误重新执行脚本
            os.chdir(self.ctx.path['sip_master_path'])
            stash_list = os.popen('git stash list')
            stash_msg = "{}{}".format(self.ctx.user,
                                      '临时保存的定制信息')
            stash_str_list = stash_list.buffer.read().decode('utf-8').split(
                "\n")
            for item in stash_str_list:
                if stash_msg in item:
                    stash = item.split(":")[0]
                    os.system('git stash pop {}'.format(stash))
                    break
