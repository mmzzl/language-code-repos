# -*- coding: utf-8 -*-
import time
import os
import re
import pyautogui
import pyperclip
from selenium.common import TimeoutException
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.action_chains import ActionChains

# 新建文件目录
counter = 1
base_path = r"C:\Users\life8\Downloads"
dir_name = "初中重要几何模型"
url = 'https://www.bilibili.com/video/BV1NN1xYvENb?spm_id_from=333.788.videopod.episodes&vd_source=a7ae190e67243ecda7824979cda776d3'
download_path = os.path.join(base_path, dir_name)
if not os.path.exists(download_path):
    os.makedirs(download_path)

downloaded_file_path = os.path.join(download_path, "downloaded.txt")


def read_downloaded_files():
    if os.path.exists(downloaded_file_path):
        with open(downloaded_file_path, 'r', encoding='utf-8') as f:
            return set(line.strip() for line in f.readlines())
    return set()


def write_downloaded_file(filename):
    with open(downloaded_file_path, 'a', encoding='utf-8') as f:
        f.write(filename + '\n')


def selenium_init():
    os.environ['WDM_SSL_VERIFY'] = '0'
    os.environ['WDM_LOG_LEVEL'] = '0'
    os.environ[
        'CHROMEDRIVER_DOWNLOAD_URL'] = 'https://npmmirror.com/mirrors/chromedriver'
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
    driver.maximize_window()
    return driver


def open_web_page(driver):
    driver.get("https://www.bilibili.com")
    # 获取视频详情url
    print("正在加载页面...")
    driver.get(url)
    time.sleep(5)


def do_same_operation(driver, filename):
    try:
        # 等待元素出现的时候，在获取源码
        button = WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.ID, 'bilibiliHelper2HandleButton'))
        )
        print("按钮已出现，准备点击")
        button.click()
        print("按钮已点击")
        # 等待 .button-grid 出现
        button_grid = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located(
                (By.CSS_SELECTOR, "div.button-grid"))
        )
        # 在 button-grid 中查找 span 文本为“高清 720P”的按钮并点击
        hd_720p_button = WebDriverWait(button_grid, 10).until(
            EC.element_to_be_clickable(
                (
                    By.XPATH,
                    ".//button[.//span[normalize-space()='高清 720P']]"))
        )
        hd_720p_button.click()
        print("✅ 已点击【高清 720P】")
        print("目标元素已出现，可以进行下一步操作")
        # 等待弹出出现
        timeout = 10
        start_time = time.time()
        while time.time() - start_time < timeout:
            try:
                location = pyautogui.locateOnScreen('location.png',
                                                    confidence=0.8)
                if location:
                    print("🔍 弹窗已出现")
                    break
            except:
                pass
            time.sleep(1)
        # 模拟按下 Ctrl + L 聚焦地址栏（部分系统有效）
        pyautogui.hotkey('ctrl', 'l')
        time.sleep(0.5)
        # 输入目录路径
        pyautogui.hotkey('ctrl', 'a')
        pyautogui.press('delete')
        # 目录操作
        pyperclip.copy(download_path)
        pyautogui.hotkey('ctrl', 'v')
        time.sleep(0.5)
        pyautogui.press('enter')
        time.sleep(0.5)
        print("输入文件名...")
        time.sleep(1)
        file_name_input_area = pyautogui.locateOnScreen('filename_input.png',
                                                        confidence=0.8)
        if file_name_input_area:
            center_x, center_y = pyautogui.center(file_name_input_area)
            offset_x = 60
            new_x = center_x + offset_x
            new_y = center_y
            pyautogui.click(new_x, new_y)
            pyperclip.copy(filename)
            time.sleep(0.5)
            pyautogui.hotkey('ctrl', 'v')
        else:
            print("未找到输入框")

        save_button_area = pyautogui.locateOnScreen('save.png',
                                                    confidence=0.8)
        if save_button_area:
            save_button_center = pyautogui.center(save_button_area)
            pyautogui.click(save_button_center)
            time.sleep(0.5)
            try:
                confirm_location = pyautogui.locateOnScreen('confirm.png',
                                                            confidence=0.8)
                if confirm_location:
                    confirm_location_center = pyautogui.center(
                        confirm_location)
                    pyautogui.click(confirm_location_center)
            except:
                print("未找到确认保存")
        else:
            print("未找到保存按钮")
        print(f"🎯 已设置保存路径: {download_path}，文件名: {filename}")
        # 等待文件下载完成
        timeout = 600
        start_time = time.time()
        while time.time() - start_time < timeout:
            try:
                location = pyautogui.locateOnScreen('download_completed.png',
                                                    confidence=0.8)
                if location:
                    print("🔍 文件已下载完成")
                    write_downloaded_file(filename)
                    break
            except:
                pass
            time.sleep(1)
        # 再点击插件
        button.click()
    except TimeoutException:
        print("超时: 未找到指定ID元素")


def format_filename(filename, counter):

    # 匹配类似 "手拉手模型" 的纯中文文件名（不带序号）
    name = filename.split('.')
    if len(name) > 1:
        name = name[1]
    else:
        name = filename

    print(f"filename:{filename}, name: {name}")
    # 无法匹配的保留原名
    return f"{counter}.{name}.mp4"


def get_max_index(downloaded_files):
    max_idx = 0
    for name in downloaded_files:
        match = re.match(r'^(\d+)\.', name)
        if match:
            idx = int(match.group(1))
            if idx > max_idx:
                max_idx = idx
    return max_idx

def is_element_visible(driver, element):
    """
    判断元素是否完全可见（没有被遮挡）
    """
    script = """
    function isElementVisible(element) {
        const rect = element.getBoundingClientRect();
        const viewportWidth = window.innerWidth;
        const viewportHeight = window.innerHeight;

        return (
            rect.top >= 0 &&
            rect.left >= 0 &&
            rect.bottom <= viewportHeight &&
            rect.right <= viewportWidth
        );
    }
    return isElementVisible(arguments[0]);
    """
    return driver.execute_script(script, element)

def scroll_if_obscured(driver, element):
    """
    如果元素被遮挡，则进行滚动
    """
    if not is_element_visible(driver, element):
        # 滚动到元素可见位置
        # current_scroll_position = driver.execute_script(
        #     "return window.pageYOffset;", element)
        # driver.execute_script(
        #     f"window.scrollTo(0, {current_scroll_position + 50});", element)
        driver.execute_script("arguments[0].scrollIntoView({block: 'center', inline: 'nearest'});", element)
        # 等待一段时间，确保滚动完成
        time.sleep(0.5)

def run():
    driver = selenium_init()
    # try:
    downloaded_files = read_downloaded_files()
    open_web_page(driver)
    # 定位视频列表容器
    video_list_container = driver.find_element(By.CSS_SELECTOR,
                                               ".video-pod__list")
    # 获取所有视频项
    video_items = video_list_container.find_elements(By.CSS_SELECTOR,
                                                     ".video-pod__item")
    for index in range(len(video_items)):
        counter = index + 1
        video_items = video_list_container.find_elements(By.CSS_SELECTOR,
                                                         ".video-pod__item")
        video_item = video_items[index]

        # 判断元素是否被遮挡，如果被遮挡则滚动
        scroll_if_obscured(driver, video_item)
        # 获取当前滚动位置
        before_click_scroll = driver.execute_script(
            "return window.pageYOffset;")
        # 显式等待可点击
        WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, ".video-pod__item"))
        )
        # 使用 JS 点击确保成功
        video_item.click()
        title_element = video_item.find_element(By.CSS_SELECTOR, '.title-txt')
        filename = format_filename(title_element.text, counter)
        # 判断是否已下载
        if filename in downloaded_files:
            print(f"跳过已下载文件：{filename}")
            continue
        # 等待一段时间，以便观察点击效果或加载新内容
        time.sleep(2)
        print(f"开始下载：{filename}")
        do_same_operation(driver, filename)
        # 点击之后再次获取滚动位置，判断是否需要回滚
        after_click_scroll = driver.execute_script(
            "return window.pageYOffset;")

        # 如果滚动过多（例如超过 500px），则回滚 300px
        if after_click_scroll - before_click_scroll > 500:
            print("🔄 页面滚动过多，准备回滚...")
            driver.execute_script("window.scrollBy(0, -300);")
            time.sleep(0.5)
        # except Exception as e:
        #     print('error: ', e)
        # finally:
    driver.quit()


if __name__ == '__main__':
    run()
