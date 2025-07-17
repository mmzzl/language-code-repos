# -*- coding: utf-8 -*-
from django.urls import path
from .views import fetch_csdn_rss, proxy_csdn_article


urlpatterns = [
    path('csdn/rss/', fetch_csdn_rss, name='csdn_rss'),
    path('api/csdn-article/', proxy_csdn_article, name='proxy_csdn_article')
]