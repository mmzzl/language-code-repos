import subprocess
import os
import signal
from django.http import HttpResponse
from django.contrib.admin.views.decorators import staff_member_required
# Create your views here.
from django.shortcuts import render, redirect
from django.http import HttpResponse
from .models import HoldingModel
from django.shortcuts import redirect
from django.http import HttpResponse
from django.core.management import call_command


def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")


@staff_member_required
def start_beat(request):
    call_command('start_beat')
    return redirect('admin:index')


@staff_member_required
def stop_beat(request):
    call_command('stop_beat')
    return redirect('admin:index')


def trend_view(request, code):
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
    user_holding_info = HoldingModel.objects.get(user=user_name, code=code)
    if user_holding_info is None:
        return HttpResponse("用户未持有该股票")
    # 分析当前收益情况
    context['name'] = user_holding_info.name
    # 计算收益
    context['profit'] = user_holding_info.cost_price - user_holding_info.holding_quantity * user_holding_info.cost_price
    return render(request, 'admin/alpha_selector_trend.html', context)