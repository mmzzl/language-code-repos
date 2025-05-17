from django.contrib import admin
from django.urls import path, include
from rest_framework.routers import DefaultRouter
from .views import (video_played, SeriesModelViewSet, VideoModelViewSet,
                    video_play_count)

router = DefaultRouter()
router.register('series', SeriesModelViewSet, basename='series')
# router.register('episodes', VideoModelViewSet, basename='episodes')
urlpatterns = [
    path('videos/playcount/<int:video_id>', video_play_count,
         name='play_count'),
    path('videos/played/<int:video_id>', video_played, name='video_played'),
    path('videos/episodes/<int:series_id>', VideoModelViewSet.as_view({
        'get': 'list_by_series',
        'post': 'create'
    }
    )),
    path('videos/', include(router.urls))
]
