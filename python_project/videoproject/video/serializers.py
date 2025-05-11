from rest_framework import serializers
from .models import Series, Video

class SeriesModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Series
        fields = '__all__'


class VideoModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Video
        fields = '__all__'