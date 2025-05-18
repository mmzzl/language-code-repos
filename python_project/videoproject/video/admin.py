import logging
from django.contrib import admin

from .form import RetryForm
# Register your models here.
from .models import Series, Video, RetryRecord
from django.shortcuts import render
from .tasks import video_processed_task

admin.site.site_header = '新手学堂管理后台'
admin.site.site_title = '新手学堂管理后台'
admin.site.index_title = '新手学堂管理后台'
logger = logging.getLogger(__name__)

class SeriesModelAdmin(admin.ModelAdmin):
    list_display = ('id', 'title')
    search_fields = ('title',)

class VideoAdmin(admin.ModelAdmin):
    list_display = ('id', 'title', 'series', 'episode_number', 'play_count')
    list_filter = ('series',)
    search_fields =  ('id','title')

admin.site.register(Video, VideoAdmin)
admin.site.register(Series, SeriesModelAdmin)


@admin.register(RetryRecord)
class RetryModelAdmin(admin.ModelAdmin):
    actions = ['retry_action']

    def retry_action(self, request, queryset):
        logger.info("【重试动作】开始处理选中项")

        success_count = 0
        for obj in queryset:
            # 获取 video 对象
            video = obj.video
            if video:
                try:
                    video_processed_task.delay(video.id)
                    logger.info(f"已下发任务，视频ID: {video.id}")
                    success_count += 1
                except Exception as e:
                    logger.error(f"任务下发失败: {e}")
            else:
                logger.warning(f"对象 {obj} 缺少 video 关联，无法下发任务")

        self.message_user(request, f'已为 {success_count} 个视频下发重试任务')

    retry_action.short_description = "重试选中项"