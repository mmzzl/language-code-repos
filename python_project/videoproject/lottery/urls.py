# -*- coding: utf-8 -*-
from django.urls import path, include
from .views import lottery_analysis

urlpatterns = [
    path('analysis/', lottery_analysis, name='analysis'),
]