from django.db import models

# Create your models here.
class RSSFeed(models.Model):
    name = models.CharField(max_length=255, unique=True)
    url = models.URLField(unique=True)

    def __str__(self):
        return self.name
