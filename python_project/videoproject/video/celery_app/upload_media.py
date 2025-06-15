# -*- coding: utf-8 -*-
import os
import re
import json
import requests
from PIL.ImageOps import cover




# 替换为你的服务器地址
BASE_URL = "https://www.life233.top/api/videos/"

# 已上传记录文件
RECORD_FILE = "uploaded_records.json"

HEADERS = {
    # 如果有认证机制，请填写 Token 或 Cookie
    # "Authorization": "Token your_token_here"
}
chunk_size = 1024 * 1024  # 1MB


def clean_string(input_string):
    # 使用正则表达式去除括号、空格和《》
    cleaned_string = re.sub(r'[()《》\s]', '', input_string)
    return cleaned_string


def load_records():
    """加载已上传和已创建的记录"""
    if os.path.exists(RECORD_FILE):
        with open(RECORD_FILE, 'r', encoding='utf-8') as f:
            return json.load(f)
    return {"uploaded_videos": [], "created_series": {}}


def save_record(series_title=None, series_id=None, uploaded_video=None):
    """保存新的上传记录或创建的系列记录"""
    records = load_records()

    if series_id:
        records["created_series"][series_title] = series_id

    if uploaded_video:
        records["uploaded_videos"].append(uploaded_video)

    with open(RECORD_FILE, 'w', encoding='utf-8') as f:
        json.dump(records, f, ensure_ascii=False, indent=2)


def create_series(title, description, cover_image_path):
    url = f"{BASE_URL}series/"
    data = {
        "title": title,
        "description": description,
    }
    with open(cover_image_path, 'rb') as cover:
        files = {
            "cover_image": cover
        }
        response = requests.post(url, data=data, files=files, headers=HEADERS)
    try:
        result = response.json()
        if 'id' in result:
            print("创建 Series 成功:", result)
            save_record(series_title=title, series_id=result['id'])
        else:
            print("创建 Series 失败:", response.text)
        return result
    except Exception as e:
        print("响应解析失败:", str(e))
        print(response.text)
        return None





def chunk_upload_video(series_id, title, episode_number, description,
                       processed_video_file, video_name, series_title):
    file_size = os.path.getsize(processed_video_file)
    total_chunks = (file_size // chunk_size) + (
        1 if file_size % chunk_size != 0 else 0)
    file_name = os.path.basename(processed_video_file)
    url = f'{BASE_URL}upload/'

    with open(processed_video_file, 'rb') as f:
        for chunk_index in range(total_chunks):
            start = chunk_index * chunk_size
            end = min(start + chunk_size, file_size)
            f.seek(start)
            chunk = f.read(end - start)

            files = {'file': (file_name, chunk)}
            data = {
                'chunk_index': chunk_index,
                'total_chunks': total_chunks,
                'file_name': clean_string(file_name),
                'title': title,
                'episode_number': episode_number,
                'description': description,
                'series_id': series_id,
                'series_title': series_title
            }

            response = requests.post(url, files=files, data=data)
            print(response.text)

            if response.status_code != 200 or response.json().get(
                    'status') != 'chunk_uploaded':
                break

    if response.json().get('status') == 'completed':
        print(f"✅ {video_name}上传成功: {file_name}")
        if file_name.endswith("m3u8"):
            save_record(uploaded_video=video_name)
            save_record(uploaded_video=file_name)
        else:
            save_record(uploaded_video=file_name)
    else:
        print(f"❌ 上传失败: {file_name}")


def upload_video(series_id, title, episode_number, description,
                 original_video_path):
    url = f"{BASE_URL}episodes/{series_id}"
    data = {
        "series": series_id,
        "title": title,
        "episode_number": episode_number,
        "description": description,
    }

    try:
        with open(original_video_path, 'rb') as video_file:
            files = {
                "original_video_file": video_file
            }
            response = requests.post(url, data=data, files=files,
                                     headers=HEADERS, verify=False,
                                     timeout=600)
            try:
                result = response.json()
                if 'id' in result:
                    print("上传 Video 成功:", result)
                    save_record(
                        uploaded_video=os.path.basename(original_video_path))
                else:
                    print("上传 Video 失败:", response.text)
                return result
            except Exception as e:
                print(e)
                print("响应解析失败:", response.text)
                return None
    except Exception as e:
        print("打开文件失败:", str(e))
        return None



