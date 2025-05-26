import os
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
from django.views import View
from django.conf import settings
from django.core.files.storage import default_storage
from django.utils import timezone
from django.utils.decorators import method_decorator
from rest_framework_simplejwt.views import TokenObtainPairView
from .serializers import MyTokenObtainPairSerializer


class MyTokenObtainPairView(TokenObtainPairView):
    serializer_class = MyTokenObtainPairSerializer


def get_tabbar(request):
    # 获取当前请求的路径
    current_path = request.path

    # 获取当前请求的完整域名
    host = request.get_host()

    # Tab 配置
    tabs = [
        {
            'pagePath': "pages/index/index",
            'text': "视频",
            'iconPath': "tabs/video.png",
            'selectedIconPath': 'tabs/video.png'
        },
        {
            'pagePath': 'pages/blog/blog',
            'text': '文章',
            'iconPath': 'tabs/article.png',
            'selectedIconPath': 'tabs/article.png'
        },
        {
            'pagePath': 'pages/wechat/wechat',
            'text': '聊天',
            'iconPath': 'tabs/聊天.png',
            'selectedIconPath': 'tabs/聊天.png'
        }
    ]

    # 动态设置 active 状态
    for tab in tabs:
        tab['active'] = (tab['pagePath'] == current_path)

    # 拼接 iconPath 和 selectedIconPath 为 http 开头的链接
    static_url = settings.STATIC_URL  # 获取静态文件的 URL 前缀（通常是 /static/）
    protocol = 'http://'  # 使用 http 协议

    for tab in tabs:
        tab['iconPath'] = f"{protocol}{host}{static_url}{tab['iconPath']}"
        tab[
            'selectedIconPath'] = f"{protocol}{host}{static_url}{tab['selectedIconPath']}"

    return JsonResponse({'tabs': tabs})


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


@method_decorator(csrf_exempt, name='dispatch')
class ChunkedUploadView(View):
    def post(self, request):
        try:
            errors = []
            file = request.FILES.get('file')
            chunk_index = int(request.POST.get('chunk_index', -1))
            total_chunks = int(request.POST.get('total_chunks', -1))
            file_name = request.POST.get('file_name')
            series_id = request.POST.get('series_id')
            title = request.POST.get('title')
            episode_number = request.POST.get('episode_number')
            description = request.POST.get('description')

            # 验证每个必需字段
            if not file:
                errors.append("Missing 'file'")
            if not isinstance(chunk_index, int) or int(chunk_index) < 0:
                try:
                    if int(chunk_index) < 0:
                        errors.append(
                            "'chunk_index' must be a non-negative integer")
                except ValueError:
                    errors.append("'chunk_index' must be an integer")
            if not isinstance(total_chunks, int) or int(total_chunks) < 0:
                try:
                    if int(total_chunks) < 0:
                        errors.append(
                            "'total_chunks' must be a non-negative integer")
                except ValueError:
                    errors.append("'total_chunks' must be an integer")
            if not file_name:
                errors.append("Missing 'file_name'")
            if not series_id:
                errors.append("Missing 'series_id'")
            if not title:
                errors.append("Missing 'title'")

            # 如果有错误，返回它们
            if errors:
                return JsonResponse({"errors": errors}, status=400)

            # 构建文件路径
            upload_dir = os.path.join(settings.MEDIA_ROOT, 'videos',
                                      'original')
            os.makedirs(upload_dir, exist_ok=True)
            file_path = os.path.join(upload_dir, file_name)

            # 写入 chunk
            with default_storage.open(file_path, 'ab+') as destination:
                for chunk in file.chunks():
                    destination.write(chunk)

            # 如果是最后一个 chunk，创建 Video 对象
            if chunk_index == total_chunks - 1:
                video_instance = Video(
                    series_id=series_id,
                    title=title,
                    original_video_file=os.path.join('videos/original',
                                                     file_name),
                    episode_number=episode_number,
                    description=description,
                    uploaded_at=timezone.now()
                )
                video_instance.save()
                return JsonResponse(
                    {'status': 'completed', 'file_name': file_name})

            return JsonResponse(
                {"status": "chunk_uploaded", "chunk_index": chunk_index})

        except Exception as e:
            return JsonResponse({"error": str(e)}, status=500)


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
