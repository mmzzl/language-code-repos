from django.urls import path


from . import views 

urlpatterns = [
    path('index/', views.index, name='index'),
    path('trend/<str:code>/', views.trend_view, name='trend_view'),
    path('start-beat/', views.start_beat, name='start_beat'),
    path('stop-beat/', views.stop_beat, name='stop_beat')
]