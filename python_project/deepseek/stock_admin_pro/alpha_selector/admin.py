from django.contrib import admin
from django.http import HttpRequest
from django.template.response import TemplateResponse
from .models import HoldingModel, TransactionRecord
from django.urls import path
from django.utils.html import format_html
from .views import trend_view
from django.urls import reverse
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
        url = reverse('trend_view')
        return format_html('<a class="button" href={}>收益</a>', url)
    
    custom_button.short_description = '操作'


@admin.register(TransactionRecord)
class TransactionAdmin(admin.ModelAdmin):
    list_display = ('transaction_id', 'amount', 'transaction_type','stock_code', 'stock_name', 'transaction_price', 'transaction_quantity',  'user', 'timestamp')
    list_filter = ('user',)
    search_fields = ('user',)


    
admin.site.site_header = "我的地盘"
admin.site.site_title = '股票管理系统后台'
admin.site.index_title = '欢迎使用股票管理系统后台'

# admin.site.register(HoldingModel, HoldingAdmin)