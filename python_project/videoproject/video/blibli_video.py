# -*- coding: utf-8 -*-
from selenium import webdriver
from selenium.webdriver.firefox.service import Service
from selenium.webdriver.firefox.options import Options
driver_path = 'D:\\geckodriver-v0.36.0-win32\\geckodriver.exe'
firefox_binary_path = r'C:\\Program Files\\Mozilla Firefox\\firefox.exe'
service = Service(driver_path)
options = Options()
options.binary = firefox_binary_path
driver = webdriver.Firefox(service=service, options=options)
# 获取视频详情url
url = 'https://www.bilibili.com/video/BV1eUBPYgE4P?spm_id_from=333.788.videopod.episodes&vd_source=a7ae190e67243ecda7824979cda776d3'

driver.get(url)
input("Press Enter to close the browser...")
driver.quit()
if __name__ == '__main__':
    pass