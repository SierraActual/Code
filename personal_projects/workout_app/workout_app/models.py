from django.contrib.auth.models import User
from django.db import models

class Workout(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    name = models.CharField(max_length=100)
    reps = models.PositiveIntegerField()
    sets = models.PositiveIntegerField()
    weight = models.FloatField(null=True, blank=True)
    date = models.DateField(auto_now_add=True)
