import os
import logging
from django.conf import settings
from moviepy.video.io.VideoFileClip import VideoFileClip
from rest_framework.views import exception_handler


def custom_exception_handler(exc, context):
    response = exception_handler(exc, context)

    if response is not None:
        # 统一所有 404 响应格式
        if response.status_code == 404:
            response.data = {
                'status': 'error',
                'code': 'not_found',
                'message': 'The requested resource was not found'
            }

    return response


def process_video(video_path, instance):
    try:
        video = VideoFileClip(video_path)
        # 提取缩略图
        thumbnail_dir = os.path.join(settings.MEDIA_ROOT, 'videos',
                                     'thumbnails')
        if not os.path.exists(thumbnail_dir):
            os.makedirs(thumbnail_dir)
        thumbnail_path = os.path.join(thumbnail_dir, f'thumbnail_'
                                                     f'{instance.id}.jpg')
        video.save_frame(thumbnail_path, t=5)  # 在5秒位置提取缩略图
        # 视频转码
        processed_dir = os.path.join(settings.MEDIA_ROOT, 'videos',
                                     'processed')
        if not os.path.exists(processed_dir):
            os.makedirs(processed_dir)
        processed_video_path = os.path.join(processed_dir,
                                            f'processed_{instance.id}.m3u8')
        # ffmpeg_cmd = f'ffmpeg -i {video_path} -c:v libx264 -c:a aac -strict -2 -f hls -hls_list_size 2 -hls_time 15 {processed_video_path}'
        ffmpeg_cmd = f'ffmpeg -i {video_path} -profile:v baseline -level 3.0 -start_number 0 -hls_time 10 -hls_list_size 0 -f hls {processed_video_path}'
        res = os.popen(ffmpeg_cmd)
        logging.info("ffmpeg执行命令结果: %s", res.read().strip())
        instance.processed_video_file.name = os.path.relpath(
            processed_video_path, settings.MEDIA_ROOT)
        instance.thumbnail.name = os.path.relpath(thumbnail_path,
                                                  settings.MEDIA_ROOT)
        instance.save()
    except Exception as e:
        logging.error("视频处理出错:%s", e)
        return False
