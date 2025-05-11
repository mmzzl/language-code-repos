from django.contrib import admin
from django.urls import path, include
from rest_framework.routers import DefaultRouter
from .views import video_played, SeriesModelViewSet, VideoModelViewSet
router = DefaultRouter()
router.register('series', SeriesModelViewSet)
router.register('episodes', VideoModelViewSet)
urlpatterns = [
    path('video-played/<int:video_id>/', video_played, name='video_played'),
    path('videos/', include(router.urls))
]
