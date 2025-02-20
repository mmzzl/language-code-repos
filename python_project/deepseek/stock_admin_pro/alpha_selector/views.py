from django.shortcuts import render
from django.http import HttpResponse
from .models import HoldingModel
# Create your views here.

def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")

def trend_view(request):
    opts = HoldingModel._meta
    app_label = opts.app_label
    verbose_name = opts.verbose_name
    verbose_name_plural = opts.verbose_name_plural
    context = {
        'opts': opts,
        'app_label': app_label,
        'verbose_name': verbose_name,
        'verbose_name_plural': verbose_name_plural,
    }
    user_name = request.user
    # 查询当前用户的持仓情况
    user_holding_info = HoldingModel.objects.filter(user=user_name)
    # 分析当前收益情况
    
    print(user_holding_info)
    return render(request, 'admin/alpha_selector_trend.html', context)