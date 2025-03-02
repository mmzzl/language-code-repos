import win32serviceutil
from django.core.management.base import BaseCommand

class Command(BaseCommand):
    help = 'Stop Celery Beat'

    def handle(self, *args, **kwargs):
        try:
            self.stdout.write(self.style.SUCCESS('Stopping Celery Beat...'))
            win32serviceutil.StopService('CeleryBeat')
        except Exception as e:
            self.stdout.write(self.style.ERROR(f'Error stopping Celery Beat: {e}'))