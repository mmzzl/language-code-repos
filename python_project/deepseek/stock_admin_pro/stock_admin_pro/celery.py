# -*- coding: utf-8 -*-
import os 
from celery import Celery

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'stock_admin_pro.settings')
app = Celery('stock_admin_pro')
app.config_from_object('django.conf:settings', namespace='CELERY')

app.autodiscover_tasks()