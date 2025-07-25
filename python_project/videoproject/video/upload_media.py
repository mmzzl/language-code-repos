# -*- coding: utf-8 -*-
import os
import json
import requests

# 替换为你的服务器地址
BASE_URL = "https://www.life233.top/api/videos/"

# 已上传记录文件
RECORD_FILE = "uploaded_records.json"

HEADERS = {
    # 如果有认证机制，请填写 Token 或 Cookie
    # "Authorization": "Token your_token_here"
}
chunk_size = 1024 * 1024  # 1MB

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


def get_series_id(series_title):
    """获取已存在的系列ID"""
    records = load_records()
    return records["created_series"].get(series_title)


def chunk_upload_video(series_id, title, episode_number, description,
                       original_video_path):
    file_size = os.path.getsize(original_video_path)
    total_chunks = (file_size // chunk_size) + (
        1 if file_size % chunk_size != 0 else 0)
    file_name = os.path.basename(original_video_path)
    url = f'{BASE_URL}upload/'

    with open(original_video_path, 'rb') as f:
        for chunk_index in range(total_chunks):
            start = chunk_index * chunk_size
            end = min(start + chunk_size, file_size)
            f.seek(start)
            chunk = f.read(end - start)

            files = {'file': (file_name, chunk)}
            data = {
                'chunk_index': chunk_index,
                'total_chunks': total_chunks,
                'file_name': file_name,
                'title': title,
                'episode_number': episode_number,
                'description': description,
                'series_id': series_id
            }

            response = requests.post(url, files=files, data=data)
            print(response.text)

            if response.status_code != 200 or response.json().get(
                    'status') != 'chunk_uploaded':
                break

    if response.json().get('status') == 'completed':
        print(f"✅ 上传成功: {file_name}")
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


if __name__ == "__main__":
    # Step 1: 系列信息
    series_title = "初中数学"
    base_dir = os.path.join("C:", 'Users', 'life8', 'Downloads')
    # base_dir = os.path.join("C:", 'Users', 'User', 'Downloads')
    upload_dir = os.path.join(base_dir, series_title)

    series_description = "这是一个示例剧集描述"
    cover_image_path = os.path.join(upload_dir, "cover.jpeg")  # 替换为你本地封面图路径

    # 加载上传记录和已创建的系列
    records = load_records()
    uploaded_files = set(records["uploaded_videos"])
    series_id = get_series_id(series_title)

    # 检查系列是否已经创建，如果没有则创建新系列
    if not series_id:
        series_data = create_series(series_title, series_description,
                                    cover_image_path)
        if not series_data or 'id' not in series_data:
            print("无法创建系列，请检查接口和权限")
            exit(1)
        series_id = series_data['id']
    else:
        print(f"系列 {series_title} 已经存在，ID: {series_id}")

    # 遍历目录下的视频文件进行上传
    for item in os.listdir(upload_dir):
        full_path = os.path.join(upload_dir, item)
        if not os.path.isfile(full_path) or not item.lower().endswith(
                ('.mp4', '.avi', '.mkv')):
            continue  # 跳过非视频文件

        if item in uploaded_files:
            print(f"【跳过】{item} 已上传过")
            continue

        try:
            parts = item.split('.')
            if len(parts) < 2:
                print(f"【跳过】文件名格式错误: {item}")
                continue

            episode_number, video_title = parts[0], '.'.join(parts[:-1])
            video_description = video_title

            print(f"正在上传: {item}")
            video_response = chunk_upload_video(
                series_id=series_id,
                title=video_title,
                episode_number=int(episode_number),
                description=video_description,
                original_video_path=full_path,
            )

        except Exception as e:
            print(f"处理文件出错: {item}", str(e))