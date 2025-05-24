# Create your views here.
import feedparser
import requests 
import base64
from bs4 import BeautifulSoup
from django.http import JsonResponse
from .models import RSSFeed

def fetch_csdn_rss(request):
    rss_feeds = RSSFeed.objects.all()  # 获取所有 RSS Feed
    all_entries = []
    for rss_feed in rss_feeds:
        rss_url = rss_feed.url
        feed = feedparser.parse(rss_url)
        entries = []

        for entry in feed.entries:
            entries.append({
                'title': entry.title,
                'link': entry.link,
                'published': entry.published if 'published' in entry else None,
                'summary': entry.summary,
                'rss_name': rss_feed.name  # 添加 RSS 名称以便区分不同的 RSS 源
            })

        all_entries.extend(entries)

    return JsonResponse({'entries': all_entries})

def proxy_csdn_article(request):
    url = request.GET.get('url')
    if not url:
        return JsonResponse({'error': 'Missing URL parameter'}, status=400)

    try:
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0 Safari/537.36',
            'Referer': 'https://www.google.com/',
        }

        response = requests.get(url, headers=headers, timeout=10, verify=False)

        if 300 <= response.status_code < 400:
            return JsonResponse({
                'error': 'Redirect detected and blocked',
                'status': response.status_code,
                'location': response.headers.get('Location')
            }, status=400)

        if response.status_code != 200:
            return JsonResponse({
                'error': 'Failed to fetch article',
                'status': response.status_code
            }, status=500)

        response.encoding = 'utf-8'
        soup = BeautifulSoup(response.text, 'lxml')

        # ✅ 只提取指定 div 的内容
        content_div = soup.find('div', {'id': 'content_views'})

        if content_div:
            # 处理 img 标签
            for img in content_div.find_all('img'):
                img_url = img.get('src')
                if img_url:
                    # 下载图片
                    img_response = requests.get(img_url, headers=headers, timeout=10, verify=False)
                    if img_response.status_code == 200:
                        # 将图片转换为 Base64 编码
                        img_base64 = base64.b64encode(img_response.content).decode('utf-8')
                        # 替换 img 标签中的 src 属性
                        img['src'] = f'data:image/jpeg;base64,{img_base64}'  # 假设图片格式为 jpeg

            cleaned_html = str(content_div)
        else:
            cleaned_html = "<p>未找到文章正文内容</p>"

        return JsonResponse({
            'title': soup.title.string if soup.title else '无标题',
            'content': cleaned_html,
            'source_url': url
        })

    except Exception as e:
        return JsonResponse({
            'error': 'Server error',
            'details': str(e)
        }, status=500)