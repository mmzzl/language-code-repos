# -*- coding: utf-8 -*-
from __future__ import absolute_import, unicode_literals

import os
import time
import json
from tasks import process_video, upload_main, clean_string, get_series_id
from config import source_dir, target_dir
from celery.result import AsyncResult
from celery_app import app
from upload_media import load_records
from celery import group
from celery import chain
from upload_media import create_series


def on_success(task_id, result):
    print(f"✅ 任务 {task_id} 成功！结果是：{result}")


def on_failure(task_id, error):
    print(f"❌ 任务 {task_id} 失败：{error}")


def main(series_name=None):
    # 获取所有的视频
    tasks = []
    episode_number = 1
    records = load_records()
    if not os.path.exists(source_dir):
        os.makedirs(source_dir)

    if not os.path.exists(target_dir):
        os.makedirs(target_dir)
    if series_name:
        series = os.path.join(source_dir, series_name)
        for file_name in os.listdir(series):
            file_info = file_name.split('.')
            if len(file_info) == 3:
                episode_number, name, ext = file_info
            else:
                name, ext = file_info
            if ext not in ['mp4', 'flv']:
                continue
            if file_name in records['uploaded_videos']:
                continue
            _data = {
                "series_name": series_name,
                "filename": clean_string(file_name),
                "episode_number": int(episode_number),
                "name": clean_string(name)
            }
            video_path = os.path.join(source_dir, series_name, file_name)
            task = process_video.s(video_path, _data)
            tasks.append(task)
        job = group(tasks).apply_async()
        results = job.get()
        with open("task.json", "w", encoding="utf-8") as f:
            json.dump(results, f, ensure_ascii=False)
            print("所有任务已经完成，数据已经保存到task.json")
    else:
        for series_name in os.listdir(source_dir):
            if series_name in records['created_series']:
                continue
            series = os.path.join(source_dir, series_name)
            for file_name in os.listdir(series):
                file_info = file_name.split('.')
                if len(file_info) == 3:
                    episode_number, name, ext = file_info
                else:
                    name, ext = file_info
                if ext not in ['mp4', 'flv']:
                    continue
                if file_name in records['uploaded_videos']:
                    continue
                _data = {
                    "series_name": series_name,
                    "filename": clean_string(file_name),
                    "episode_number": int(episode_number),
                    "name": clean_string(name)
                }
                video_path = os.path.join(source_dir, series_name, file_name)
                task = process_video.s(video_path, _data)
                tasks.append(task)
        job = group(tasks).apply_async()
        results = job.get()
        with open("task.json", "w", encoding="utf-8") as f:
            json.dump(results, f, ensure_ascii=False)
            print("所有任务已经完成，数据已经保存到task.json")

def  create_series_id():
    with open('task.json', 'r', encoding='utf-8') as f:
        tasks = json.loads(f.read())
    for instance in tasks:
        upload_dir = os.path.dirname(instance['video_path'])
        series_title = instance['series_name']
        series_description = "这是一个示例剧集描述"
        cover_image_path = os.path.join(upload_dir, "cover.jpeg")
        series_id = get_series_id(series_title)
        if not series_id:
            series_data = create_series(series_title, series_description,
                                        cover_image_path)
            if not series_data or 'id' not in series_data:
                print("无法创建系列，请检查接口和权限")
                exit(1)
            series_id = series_data['id']
        else:
            print(f"系列 {series_title} 已经存在，ID: {series_id}")

def upload():
    task_chains = []
    with open('task.json', 'r', encoding='utf-8') as f:
        tasks = json.loads(f.read())
    for instance in tasks:
        series_name = instance['series_name']
        task_chain = chain(get_series_id.s(series_name), upload_main.s(
            instance))
        task_chains.append(task_chain)
    result = group(task_chains).apply_async()
    result.get()
    print("所有数据已上传")


if __name__ == '__main__':
    # main()
    create_series_id()
    upload()
