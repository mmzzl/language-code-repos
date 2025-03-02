# -*- coding: utf-8 -*-
from django.contrib import admin
from django.http import HttpRequest
from django.template.response import TemplateResponse
from .models import HoldingModel, TransactionRecord
from django.urls import path
from django.utils.html import format_html
from .views import trend_view, start_beat, stop_beat
from django.urls import reverse
from django_celery_beat.admin import PeriodicTaskAdmin
from django_celery_beat.models import PeriodicTask, IntervalSchedule
# Register your models here.

@admin.register(HoldingModel)
class HoldingAdmin(admin.ModelAdmin):
    list_display = ('code', 'name', 'holding_quantity', 'cost_price', 'user', 'status', 'custom_button')
    list_filter = ('name',)
    search_fields = ('name', 'code')
    ordering = ('-code',)
    list_per_page = 50
    def has_delete_permission(self, request, obj=None):
        return request.user.is_superuser
    
    def custom_button(self, obj):
        url = reverse('trend_view', args=[obj.code])
        return format_html('<a class="button" href={}>收益</a>', url)
    
    custom_button.short_description = '操作'


@admin.register(TransactionRecord)
class TransactionAdmin(admin.ModelAdmin):
    list_display = ('stock_code', 'stock_name', 'transaction_id', 'amount', 'transaction_type', 'transaction_price', 'transaction_quantity',  'user', 'timestamp')
    list_filter = ('user',)
    search_fields = ('user',)

# 添加自定义视图到管理后台
class CustomAdminSite(admin.AdminSite):
    site_header = "我的地盘"
    site_title = '股票管理系统后台'
    index_title = '欢迎使用股票管理系统后台'

    def get_urls(self):
        urls = super().get_urls()
        custom_urls = [
            path('start-beat/', self.admin_view(start_beat), name='start_beat'),
            path('stop-beat/', self.admin_view(stop_beat), name='stop_beat'),
        ]
        return custom_urls + urls
    
    def index(self, request, extra_context=None):
        extra_context = extra_context or {}
        extra_context['start_beat_url'] = reverse('start_beat')
        extra_context['stop_beat_url'] = reverse('stop_beat')
        return super().index(request, extra_context=extra_context)


admin_site = CustomAdminSite(name='myadmin')
admin_site.register(HoldingModel, HoldingAdmin)
admin_site.register(TransactionRecord, TransactionAdmin)
admin_site.register(PeriodicTask, PeriodicTaskAdmin)
admin_site.register(IntervalSchedule)

admin.site = admin_site