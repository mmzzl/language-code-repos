from django.shortcuts import render

# Create your views here.
def play_m3u8(request, file_path):
    context = {
        'url': 'm3u8/demo/demo.m3u8'
    }
    return render(request, 'play_m3u8.html', context)