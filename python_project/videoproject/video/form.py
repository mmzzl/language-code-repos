# -*- coding: utf-8 -*-

from django import forms

class RetryForm(forms.Form):
    series_id = forms.CharField(label='系列ID', required=True)
    video_id = forms.CharField(label='视频ID', required=True)
