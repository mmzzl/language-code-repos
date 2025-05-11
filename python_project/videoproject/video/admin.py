from django.contrib import admin

# Register your models here.
from .models import Series, Video
admin.site.register(Series)

class VideoAdmin(admin.ModelAdmin):
    list_display = ('title', 'series', 'episode_number', 'play_count')
    list_filter = ('series',)
admin.site.register(Video, VideoAdmin)