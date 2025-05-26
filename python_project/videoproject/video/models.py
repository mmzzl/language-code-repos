import os
from django.db import models
from django.conf import settings
from .tasks import video_processed_task
# from django.contrib.auth.models import AbstractUser


# class CustomUser(AbstractUser):
#     openid = models.CharField(max_length=100, unique=True,
#                               verbose_name='OpenID')
#     nickname = models.CharField(max_length=100, blank=True, null=True,
#                                 verbose_name='昵称')
#     avatar_url = models.URLField(blank=True, null=True, verbose_name='头像')


# Create your models here.
class Series(models.Model):
    title = models.CharField(max_length=100, verbose_name='系列标题')
    description = models.TextField(verbose_name='系列描述')
    cover_image = models.ImageField(upload_to='series_covers/',
                                    verbose_name='系列封面图')

    def __str__(self):
        return f'{self.title}'


class Video(models.Model):
    series = models.ForeignKey(Series, on_delete=models.CASCADE,
                               related_name='videos')
    title = models.CharField(max_length=100, verbose_name='视频标题')
    original_video_file = models.FileField(
        upload_to=os.path.join('videos', 'original'),
        verbose_name='原始视频文件', blank=True,
        null=True)
    processed_video_file = models.FileField(
        upload_to=os.path.join('videos', 'processed'),
        blank=True, null=True,
        verbose_name='处理后的文件'
    )
    thumbnail = models.ImageField(upload_to=os.path.join('videos',
                                                         'thumbnails'),
                                  verbose_name='缩略图', blank=True, null=True,
                                  )
    episode_number = models.IntegerField(verbose_name='集数')
    description = models.TextField(verbose_name='视频描述')
    uploaded_at = models.DateTimeField(auto_now_add=True,
                                       verbose_name='上传时间')
    play_count = models.IntegerField(default=0, verbose_name='播放次数')

    class Meta:
        ordering = ['episode_number']

    def __str__(self):
        return f'{self.series.title} - {self.title}'

    def save(self, *args, **kwargs):
        # 获取旧的实例（如果是更新）
        is_new = self._state.adding
        old_file = None
        if not is_new:
            try:
                old_instance = Video.objects.get(pk=self.pk)
                old_file = old_instance.original_video_file
            except Video.DoesNotExist:
                pass

        super().save(*args, **kwargs)

        # 如果是新增 或者 文件被修改过
        if self.original_video_file and (
                is_new or old_file != self.original_video_file):
            video_processed_task.delay(self.id)


class RetryRecord(models.Model):
    series = models.ForeignKey(Series, on_delete=models.CASCADE,
                               verbose_name="系列", null=True, default=None)
    video = models.ForeignKey(Video, on_delete=models.CASCADE,
                              verbose_name="视频", null=True, default=None)
    created_at = models.DateTimeField(auto_now_add=True,
                                      verbose_name="创建时间")

    class Meta:
        verbose_name = "重试记录"
        verbose_name_plural = "重试记录"

    def __str__(self):
        return f"系列 {self.series.title if self.series else '未知'}, 视频 {self.video.title if self.video else '未知'}"
