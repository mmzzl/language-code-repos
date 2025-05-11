import os
from django.db import models
from django.conf import settings
from .utils import process_video

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
        upload_to=os.path.join('videos','original'),
                                       verbose_name='原始视频文件', blank=True,
                                           null=True)
    processed_video_file = models.FileField(
        upload_to=os.path.join('videos','processed'),
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
        is_new = self._state.adding  # 判断是否是新记录
        super().save(*args, **kwargs)
        if is_new and self.original_video_file:
            video_path = os.path.join(settings.MEDIA_ROOT,
                                      self.original_video_file.name)
            # 调用自定义函数处理视频文件
            process_video(video_path, self)

