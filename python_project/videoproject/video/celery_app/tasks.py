# -*- coding: utf-8 -*-
import os
import re
import json
import logging
import requests
from moviepy.video.io.VideoFileClip import VideoFileClip
from config import target_dir
from celery_app import app
from config import BASE_URL
from upload_media import (load_records, create_series,
                          chunk_upload_video)


def clean_string(input_string):
    # 使用正则表达式去除括号、空格和《》
    cleaned_string = re.sub(r'[()《》\s]', '', input_string)
    return cleaned_string

@app.task
def save_task(data):
    with open("task.json", 'a+', encoding='utf-8') as f:
        f.write(json.dumps(data, ensure_ascii=False))
    print("实列已经保存到task.json中")


@app.task
def get_series_id(series_title):
    """获取已存在的系列ID"""
    records = load_records()
    if not records["created_series"].get(series_title):
        url = f'{BASE_URL}series/?title={series_title}'
        result = requests.get(url, verify=False)
        response = result.json()
        if not response.get('results'):
            return False
        else:
            series_id = response['results'][0]['id']
            return series_id
    else:
        return records["created_series"][series_title]

@app.task(bind=True, max_retries=3)
def upload_main(self, series_id, instance):
    try:
        series_title = instance['series_name']
        records = load_records()
        uploaded_files = set(records['uploaded_videos'])
        processed_video_path = os.path.dirname(
            instance['processed_video_path'])
        for item in os.listdir(processed_video_path):
            if item in uploaded_files:
                print(f"【跳过】{item} 已上传过")
                continue
            full_path = os.path.join(processed_video_path, item)
            episode_number = instance['episode_number']
            video_description = instance['name']
            print("开始请求，上传文件")
            video_response = chunk_upload_video(
                series_id=series_id,
                title=clean_string(instance['name']),
                episode_number=int(episode_number),
                description=video_description,
                processed_video_file=full_path,
                video_name=instance['filename'],
                series_title=series_title
            )
    except Exception as exc:
        print("请求错误,原因是: %s" % exc)
        self.retry(exc=exc)


@app.task(bind=True, max_retries=3)
def process_video(self, video_path, instance):
    try:
        video = VideoFileClip(video_path)
        # 提取缩略图
        series_path = os.path.join(target_dir, instance['series_name'],
                                   instance['name'])
        if not os.path.exists(series_path):
            os.makedirs(series_path)
        thumbnail_dir = os.path.join(target_dir, instance[
            'series_name'], instance['name'])
        if not os.path.exists(thumbnail_dir):
            os.makedirs(thumbnail_dir)
        thumbnail_path = os.path.join(thumbnail_dir, f'{instance['name']}.jpg')
        video.save_frame(thumbnail_path, t=5)  # 在5秒位置提取缩略图
        # 视频转码
        processed_dir = series_path
        if not os.path.exists(processed_dir):
            os.makedirs(processed_dir)
        process_video_name = f'processed_{instance['name']}.m3u8'
        processed_video_path = os.path.join(processed_dir, process_video_name)
        # ffmpeg_cmd = f'ffmpeg -i {video_path} -c:v libx264 -c:a aac -strict -2 -f hls -hls_list_size 2 -hls_time 15 {processed_video_path}'
        ffmpeg_cmd = f'ffmpeg -i {video_path} -profile:v baseline -level 3.0 -start_number 0 -hls_time 10 -hls_list_size 0 -f hls {processed_video_path}'
        res = os.popen(ffmpeg_cmd)
        logging.info("ffmpeg执行命令结果: %s", res.read().strip())
        instance['processed_video_path'] = processed_video_path
        instance['thumbnail_path'] = thumbnail_path
        instance['process_video_name'] = process_video_name
        instance['video_path'] = video_path
        return instance
    except Exception as exc:
        logging.error("视频处理出错:%s", exc)
        self.retry(exc=exc)

