# -*- coding: utf-8 -*-

import os
import time
from tasks import process_video
from config import source_dir, target_dir
from celery.result import AsyncResult
from celery_app import app
from upload_media import upload_main, load_records


def on_success(task_id, result):
    print(f"✅ 任务 {task_id} 成功！结果是：{result}")

def on_failure(task_id, error):
    print(f"❌ 任务 {task_id} 失败：{error}")

def monitor_all_tasks(task_ids, poll_interval=1, max_wait_time=60 * 60 * 2):
    results = {task_id: None for task_id  in task_ids}
    finished = set()
    start_time = time.time()

    while len(finished) < len(task_ids):
        try:
            now = time.time()
            if now - start_time > max_wait_time:
                print("超时，部分任务仍在运行中。")
                break
            for task_id in task_ids:
                if task_id in finished:
                    continue
                result = AsyncResult(task_id, app=app)
                if  result.state == 'SUCCESS':
                    on_success(task_id, result.result)
                    results[task_id] = result.result
                    finished.add(task_id)
                    # 文件上传
                    if result.result:
                        upload_main(result.result)
                elif result.state in ['FAILURE', 'REVOKED']:
                    error_info = {
                        'state': result.state,
                        'error': str(result.result),
                        'traceback': result.traceback
                    }
                    on_failure(task_id, error_info)
                    results[task_id] = error_info
                    finished.add(task_id)
            time.sleep(poll_interval)
        except Exception as e:
            print("error: ", e)
            continue

    for task_id in task_ids:
        if task_id not in finished:
            print(f"任务{task_id}仍在运行中。。。")
    return results



def main():
    # 获取所有的视频

    task_ids = []
    records = load_records()
    if not os.path.exists(source_dir):
        os.makedirs(source_dir)
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)
    for series_name in os.listdir(source_dir):
        series = os.path.join(source_dir, series_name)
        for file_name in os.listdir(series):
            file_info = file_name.split('.')
            if len(file_info) == 3:
                episode_number, name, ext = file_info
            else:
                name , ext = file_info
            if ext not in ['mp4', 'flv']:
                continue
            if file_name in records['uploaded_videos']:
                continue
            _data = {
                "series_name": series_name,
                "filename": file_name,
                "episode_number": episode_number,
                "name": name
            }
            video_path = os.path.join(source_dir, series_name, file_name)
            result = process_video.delay(video_path, _data)
            task_ids.append(result.id)
    monitor_all_tasks(task_ids)



if __name__ == '__main__':
    main()
