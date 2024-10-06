import os
import subprocess
from log import logger
import configparser
from apscheduler.schedulers.blocking import BlockingScheduler


def read_conf_ini(ini_file_name):
    config = configparser.ConfigParser()
    config.read(ini_file_name)
    sections = {}
    for section in config.sections():
        if not sections.get(section):
            sections[section] = {}
        for key, value in config.items(section):
            sections[section].update({key: value})
    return sections


def execute_cmd(cmd):
    try:
        result = subprocess.run(cmd, shell=True,
                                capture_output=True,
                                text=True)
        stdout = result.stdout.strip()
        stderr = result.stderr.strip()
        return_code = result.returncode
        logger.info("stdout -> %s, stderr -> %s, return_code -> %s",
                    stdout,
                    stderr,
                    return_code)
    except Exception as e:
        logger.error("execute error: %s", e)


def add_task():
    scheduler = BlockingScheduler()
    config = read_conf_ini("inputs.ini")
    for section, item in config.items():
        if item['enable'] is False:
            continue
        scheduler.add_job(execute_cmd,
                          "interval",
                          minutes=1,
                          args=[section])


if __name__ == '__main__':
    read_conf_ini("inputs.ini")
    execute_cmd("ipconfig")
