# app.py
from flask import Flask, render_template, redirect, url_for, request, session
from werkzeug.security import generate_password_hash, check_password_hash
import sqlite3

app = Flask(__name__)
app.secret_key = b'_5#y2L"F4Q8z\n\xec]/'

# User data
users = {
    "user1": {"password": generate_password_hash("password1"), "email": "user1@example.com"},
    "user2": {"password": generate_password_hash("password2"), "email": "user2@example.com"},
    # add more users as needed
}

# Connect to the database
conn = sqlite3.connect("workouts.db", check_same_thread=False)
cursor = conn.cursor()

# Create the workout table
cursor.execute("""CREATE TABLE IF NOT EXISTS workouts (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username TEXT,
                    workout_name TEXT,
                    sets INTEGER,
                    reps INTEGER,
                    weight INTEGER
                )""")
conn.commit()

@app.route("/")
def home():
    return render_template("home.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        if username in users and check_password_hash(users[username]["password"], password):
            session["username"] = username
            return redirect(url_for("workout_log"))
        else:
            return "Invalid username/password", 400
    return render_template("login.html")

@app.route("/logout")
def logout():
    session.pop("username", None)
    return redirect(url_for("home"))

@app.route("/workout_log", methods=["GET", "POST"])
def workout_log():
    if "username" not in session:
        return redirect(url_for("login"))
    if request.method == "POST":
        username = session["username"]
        workout_name = request.form.get("workout_name")
        sets = request.form.get("sets")
        reps = request.form.get("reps")
        weight = request.form.get("weight")
        cursor.execute("""INSERT INTO workouts (username, workout_name, sets, reps, weight)
                          VALUES (?, ?, ?, ?, ?)""", (username, workout_name, sets, reps, weight))
        conn.commit()
        return redirect(url_for("home"))
    return render_template("workout_log.html")

if __name__ == "__main__":
    app.run()
