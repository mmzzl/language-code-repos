# -*- coding: utf-8 -*-
import os
import logging
from celery import shared_task
from moviepy.video.io.VideoFileClip import VideoFileClip
from config import source_dir, target_dir
from celery_app import app

@app.task
def process_video(video_path, instance):
    try:
        video = VideoFileClip(video_path)
        # 提取缩略图
        series_path = os.path.join(target_dir, instance['series_name'])
        thumbnail_dir = os.path.join(target_dir, 'thumbnails')
        if not os.path.exists(thumbnail_dir):
            os.makedirs(thumbnail_dir)
        thumbnail_path = os.path.join(thumbnail_dir, f'thumbnail'
                                          f'_{instance['episode_number']}.jpg')
        video.save_frame(thumbnail_path, t=5)  # 在5秒位置提取缩略图
        # 视频转码
        processed_dir = series_path
        if not os.path.exists(processed_dir):
            os.makedirs(processed_dir)
        process_video_name = f'processed_{instance['episode_number']}.m3u8'
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
    except Exception as e:
        logging.error("视频处理出错:%s", e)
        return False