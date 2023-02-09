import sqlite3

def create_user(username, email, password):
    conn = sqlite3.connect('workouts.db')
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            username TEXT NOT NULL,
            email TEXT NOT NULL,
            password TEXT NOT NULL
        );
    ''')
    c.execute('''
        INSERT INTO users (username, email, password)
        VALUES (?, ?, ?)
    ''', (username, email, password))
    conn.commit()
    conn.close()

def get_user(username):
    conn = sqlite3.connect('workouts.db')
    c = conn.cursor()
    c.execute('''
        SELECT * FROM users WHERE username=?
    ''', (username,))
    user = c.fetchone()
    conn.close()
    return user

def add_workout(username, workout_name, sets, reps, weight):
    conn = sqlite3.connect('workouts.db')
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS workouts (
            id INTEGER PRIMARY KEY,
            username TEXT NOT NULL,
            workout_name TEXT NOT NULL,
            sets INTEGER NOT NULL,
            reps INTEGER NOT NULL,
            weight INTEGER NOT NULL
        );
    ''')
    c.execute('''
        INSERT INTO workouts (username, workout_name, sets, reps, weight)
        VALUES (?, ?, ?, ?, ?)
    ''', (username, workout_name, sets, reps, weight))
    conn.commit()
    conn.close()

def get_workouts(username):
    conn = sqlite3.connect('workouts.db')
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS workouts (
            id INTEGER PRIMARY KEY,
            username TEXT NOT NULL,
            workout_name TEXT NOT NULL,
            sets INTEGER NOT NULL,
            reps INTEGER NOT NULL,
            weight INTEGER NOT NULL
        );
    ''')
    c.execute('''
        SELECT * FROM workouts WHERE username=?
    ''', (username,))
    workouts = c.fetchall()
    conn.close()
    return workouts

def get_db():
    conn = sqlite3.connect('workouts.db')
    return conn

def query_db(query, args=(), one=False):
    cur = get_db().execute(query, args)
    rv = cur.fetchall()
    cur.close()
    return (rv[0] if rv else None) if one else rv

