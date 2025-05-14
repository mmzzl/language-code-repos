from django.shortcuts import render

# Create your views here.
from django.shortcuts import get_object_or_404
from .models import Video, Series
from django.http import JsonResponse
from rest_framework import viewsets, filters
from .serializers import SeriesModelSerializer, VideoModelSerializer
from .pagination import CustomPagination
from rest_framework.response import Response
from celery.result import AsyncResult
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
def video_played(request, video_id):
    if request.method != 'POST':
        return JsonResponse({
            'success': False,
            'error': 'Method not allowed. Use POST.'
        }, status=405)  # 返回 405 Method Not Allowed
    try:
        video = Video.objects.get(id=video_id)
    except Video.DoesNotExist:
        return JsonResponse({
            'success': False,
            'error': f'Video with ID {video_id} not found'
        }, status=404)
    video.play_count += 1
    video.save()
    return JsonResponse({'success': True})


def video_play_count(request, video_id):
    try:
        video = Video.objects.get(id=video_id)
    except Video.DoesNotExist:
        return JsonResponse({
            'success': False,
            'error': f'Video with ID {video_id} not found'
        }, status=404)
    return JsonResponse({'success': True, 'number': video.play_count})

def check_task_status(request, task_id):
    result = AsyncResult(task_id)
    return JsonResponse(
        {
            'status': result.status,
            'result': result.result if result.ready() else None
        }
    )

class SeriesModelViewSet(viewsets.ModelViewSet):
    queryset = Series.objects.all().order_by('title')
    serializer_class = SeriesModelSerializer
    pagination_class = CustomPagination  # 使用自定义分页类

    def get_queryset(self):
        queryset = super().get_queryset()
        title = self.request.query_params.get('title')
        if title:
            queryset = queryset.filter(title__icontains=title)
        return queryset


class VideoModelViewSet(viewsets.ModelViewSet):
    # queryset = Video.objects.all().order_by('episode_number')
    serializer_class = VideoModelSerializer
    pagination_class = None  # 禁用分页

    def get_queryset(self):
        queryset = Video.objects.all().order_by('episode_number')
        return queryset

    def list_by_series(self, request, series_id=None):
        queryset = self.get_queryset().filter(series_id=series_id)
        serializer = self.get_serializer(queryset, many=True)
        return Response(serializer.data)
