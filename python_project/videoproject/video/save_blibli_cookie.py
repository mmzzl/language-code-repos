import os
import json
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options

os.environ['WDM_SSL_VERIFY'] = '0'
os.environ['WDM_LOG_LEVEL'] = '0'
os.environ['CHROMEDRIVER_DOWNLOAD_URL'] = 'https://npmmirror.com/mirrors/chromedriver'
os.environ["WDM_LOCAL"] = "True"
# 指定 chromedriver 路径
service = Service(ChromeDriverManager().install())
profile_dir = 'Default'

options = Options()

custom_profile_dir = r"D:\chrome_profile\bilibili_downloader"
options.add_argument(f"--user-data-dir={custom_profile_dir}")
options.add_argument(f"--profile-directory={profile_dir}")

# 创建浏览器对象
driver = webdriver.Chrome(service=service, options=options)
driver.get("https://www.bilibili.com")

input("请手动登录 B站账号后，按回车继续...")  # 手动登录
# 获取当前页面的 Cookie 并保存
cookies = driver.get_cookies()

with open("bilibili_cookies.json", "w", encoding="utf-8") as f:
    json.dump(cookies, f)

print("✅ Cookie 已保存到 bilibili_cookies.json")
driver.quit()

if __name__ == '__main__':
    pass