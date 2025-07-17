from rest_framework import serializers
from .models import Series, Video
from rest_framework_simplejwt.serializers import TokenObtainPairSerializer


class MyTokenObtainPairSerializer(TokenObtainPairSerializer):
    @classmethod
    def get_token(cls, user):
        token = super().get_token(user)
        token['username'] = user.username
        token['email'] = user.email
        return token

    def validate(self, attrs):
        data = super().validate(attrs)
        data['user'] = {
            'id': self.user.id,
            'username': self.user.username,
            'email': self.user.email
        }
        return data


class SeriesModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Series
        fields = '__all__'


class VideoModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Video
        fields = '__all__'
