from django.shortcuts import render

# Create your views here.
from .models import Video, Series
from django.http import JsonResponse
from rest_framework import viewsets
from .serializers import SeriesModelSerializer, VideoModelSerializer
from .pagination import CustomPagination


def video_played(request, video_id):
    video = Video.objects.get(id=video_id)
    video.play_count += 1
    video.save()
    return JsonResponse({
        'success': True
    })



class SeriesModelViewSet(viewsets.ModelViewSet):
    queryset = Series.objects.all().order_by('-title')
    serializer_class = SeriesModelSerializer
    pagination_class = CustomPagination  # 使用自定义分页类


class VideoModelViewSet(viewsets.ModelViewSet):
    queryset = Video.objects.all().order_by('episode_number')
    serializer_class = VideoModelSerializer
    pagination_class = None  # 禁用分页

    def get_queryset(self):
        queryset = Video.objects.all().order_by('episode_number')

        # 获取 URL 中的查询参数，例如：?series_id=5
        series_id = self.request.query_params.get('series_id', None)

        if series_id is not None:
            queryset = queryset.filter(series_id=series_id)

        return queryset


