# -*- coding: utf-8 -*-
from __future__ import absolute_import, unicode_literals

from celery import Celery

app = Celery(
    'tasks',
    broker='redis://127.0.0.1:6379/0',
    backend='redis://127.0.0.1:6379/0',
    include=['tasks']
)

# app.conf.update(
#     result_expire=86400 * 7
# )