from django.shortcuts import render
from django.http import JsonResponse
# Create your views here.
import feedparser
import requests 
import base64
from bs4 import BeautifulSoup
from django.http import JsonResponse
from urllib.parse import quote, unquote

def fetch_csdn_rss(request):
    rss_url = 'https://rss.csdn.net/qq_63129682/rss/map?spm=1001.2014.3001.5494'
    feed = feedparser.parse(rss_url)
    entries = []
    for entry in feed.entries:
        entries.append({
            'title': entry.title,
            'link': entry.link,
            'published': entry.published if 'published' in entry else None,
            'summary': entry.summary,
        })
    return JsonResponse({'entries': entries})


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