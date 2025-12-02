import json
import os
import random

OUTPUT_DIR = "json"
os.makedirs(OUTPUT_DIR, exist_ok=True)

def generate_teachers(count=15):
    teachers = []
    for i in range(0, count):
        name="Teacher_"
        if i<=9: name += "0" + str(i)
        else: name += str(i)
        teachers.append({
            "id": i,
            "name": name,
            "isTechnical": random.random() < 0.5,
            "weeklyRemainingMinutes": 1200
        })
    return teachers

def generate_students(count=60):
    students = []
    for i in range(0, count):
        name="Student_"
        if i<=9: name += "0" + str(i)
        else: name += str(i)
        students.append({
            "id": i,
            "name": name,
            "hasAccommodations": random.random() < 1/4,
            "referentTeacherId": random.randint(1, 12),
            "tutorId": i,
        })
    return students

def generate_tutors(count=60):
    tutors = []
    for i in range(0, count):
        name="Tutor_"
        if i<=9: name += "0" + str(i)
        else: name += str(i)
        tutors.append({
            "id": i,
            "name": name,
        })
    return tutors

def generate_rooms(count=8):
    rooms = []
    for i in range(0, count):
        tag="Room_"
        if i<=9: tag += "0" + str(i)
        else: tag += str(i)
        rooms.append({
            "id": i,
            "tag": tag,
        })
    return rooms

def save_json(filename, data):
    path = os.path.join(OUTPUT_DIR, filename)
    with open(path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2)
    print(f"Generated {path}")

if __name__ == "__main__":
    save_json("teachers.json", generate_teachers())
    save_json("students.json", generate_students())
    save_json("tutors.json", generate_tutors())
    save_json("rooms.json", generate_rooms())
