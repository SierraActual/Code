from flask import Flask, render_template, request, redirect, url_for, session, flash
from database import create_user, get_user, add_workout, get_workouts, get_db, query_db

app = Flask(__name__)
app.secret_key = 'secret_key'

@app.route('/')
def home():
    if not session.get('logged_in'):
        return redirect(url_for('login'))
    workouts = get_workouts(session['username'])
    return render_template('home.html', workouts=workouts)

@app.route('/login', methods=['GET', 'POST'])
def login():
    error = None
    if request.method == 'POST':
        user = query_db('select * from users where username = ?',
                        [request.form['username']], one=True)
        if user is None:
            error = 'Incorrect username'
        elif user[3] != request.form['password']:
            error = 'Incorrect password'
        else:
            session['logged_in'] = True
            session['username'] = request.form['username']
            flash('You were logged in')
            return redirect(url_for('home'))
        print(user)
        print(error)
    return render_template('login.html', error=error)


@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        email = request.form['email']
        password = request.form['password']
        create_user(username, email, password)
        return redirect(url_for('login'))
    return render_template('register.html')

@app.route('/log_workout', methods=['GET', 'POST'])
def log_workout():
    if request.method == 'POST':
        workout_name = request.form['workout_name']
        sets = request.form['sets']
        reps = request.form['reps']
        weight = request.form['weight']
        add_workout(session['username'], workout_name, sets, reps, weight)
        return redirect(url_for('home'))
    return render_template('log_workout.html')

@app.route('/logout')
def logout():
    session.pop('username', None)
    return redirect(url_for('home'))

if __name__ == '__main__':
    app.run(debug=True)
