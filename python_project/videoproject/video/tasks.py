# -*- coding: utf-8 -*-

from celery import shared_task
import time
from .utils import process_video

@shared_task
def video_processed_task(video_id):
    # 根据ID获取视频对象
    from .models import Video
    video = Video.objects.get(id=video_id)
    # 处理视频文件
    process_video(video.original_video_file.path, video)