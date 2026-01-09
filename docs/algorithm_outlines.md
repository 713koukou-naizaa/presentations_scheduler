# GREEDY SCHEDULING ALGORITHM OUTLINES

Une version graphique a également été faite à l'aide d'[AlgoForge](https://algoforge.fr): cf. [Fichier d'algorithme AlgoForge](./greedy_scheduling_algorithm_algoforge.json).

## Initialization process

1. **Get base data**
2. Assign each student their referent teacher.
3. Assign each student their tutor.
4. Determine each student’s presentation length.
5. Initialize the scheduler.

---

### Sub processes

#### Get base data

1. Get all teachers.
3. Get all students.
4. Get all rooms.

## Greedy earliest-fit scheduling process

1. Initialize the first day.
2. Sort students by descending presentation length.
3. For each student:

   - Start from the earliest day.
   - While the student is not placed:

     1. **Ensure day capacity**
     2. For each room in that day:

        - **Try morning slot**
        - If morning fails, **try afternoon slot**
     3. If student is not placed, **move to next day**
4. Repeat until all students are assigned.

---

#### Sub processes

#### Ensure day capacity

1. Check rooms (morning and afternoon pointers)
2. Check teachers (busy slots per day)

#### Try morning/afternoon slot

1. Check if the presentation fits before the morning cutoff.
2. Check referent teacher availability.
3. Check second teacher availability and constraints.
4. If valid, **place the student**

#### Place the student

1. Select a second teacher.
2. Reserve referent teacher, second teacher.
3. Record the presentation assignment.
4. Advance the room’s morning/afternoon pointer.

#### Move to next day

1. Increment the day.
2. Ensure capacity exists for that new day.