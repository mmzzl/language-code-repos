from django.contrib import admin
from django.urls import path, include
from rest_framework.routers import DefaultRouter
from .views import video_played, SeriesModelViewSet, VideoModelViewSet

router = DefaultRouter()
router.register('series', SeriesModelViewSet, basename='series')
# router.register('episodes', VideoModelViewSet, basename='episodes')
urlpatterns = [
    path('video-played/<int:video_id>/', video_played, name='video_played'),
    path('videos/episodes/<int:series_id>', VideoModelViewSet.as_view(
        {'get':'list_by_series'})),
    path('videos/', include(router.urls))

]
