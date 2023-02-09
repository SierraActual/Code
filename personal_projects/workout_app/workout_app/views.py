from django.shortcuts import render, redirect
from django.contrib.auth.decorators import login_required
from .models import Workout

@login_required
def workout_log(request):
    if request.method == 'POST':
        workout = Workout(
            user=request.user,
            name=request.POST['name'],
            reps=request.POST['reps'],
            sets=request.POST['sets'],
            weight=request.POST.get('weight', None)
        )
        workout.save()
        return redirect('workout_log')
    else:
        workouts = Workout.objects.filter(user=request.user).order_by('-date')
        return render(request, 'workout_log.html', {'workouts': workouts})
