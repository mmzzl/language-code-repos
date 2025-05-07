from django.contrib import admin
from django.urls import path
from .views import play_m3u8

urlpatterns = [
    path('play/<path:file_path>', play_m3u8, name='play_m3u8'),
]
