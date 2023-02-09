from django.urls import path
from .views import workout_log

urlpatterns = [
    path('workout_log/', workout_log, name='workout_log'),
]
