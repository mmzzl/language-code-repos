import win32serviceutil
from django.core.management.base import BaseCommand

class Command(BaseCommand):
    help = 'Start Celery Beat'

    def handle(self, *args, **kwargs):
        
        try:
            self.stdout.write(self.style.SUCCESS('Starting Celery Beat...'))
            win32serviceutil.StartService('CeleryBeat')
            self.stdout.write(self.style.SUCCESS('Celery Beat started successfully.'))
        except Exception as e:
            self.stdout.write(self.style.ERROR('Failed to start Celery Beat: {}'.format(str(e))))
        