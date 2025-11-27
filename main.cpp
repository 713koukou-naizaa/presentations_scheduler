#include <bits/stdc++.h>

#include "globalConsts.h"
#include "Utils.h"
#include "Teacher.h"
#include "Tutor.h"
#include "Student.h"
#include "Room.h"
#include "Scheduler.h"

using namespace std;

int main()
{
    // Create teachers
    vector<Teacher> teachers;
    for(unsigned short int i=0; i<globalConsts::NB_TEACHERS; ++i)
    {
        string name = (i<9) ? "Teacher_0" + to_string(i+1) : "Teacher_" + to_string(i+1);

        bool isTech = (i % 2 == 0); // Half are technical

        teachers.emplace_back(i, name, isTech, globalConsts::MAX_TEACHERS_WEEKLY_WORKED_TIME, 1);
    }

    // Create tutors
    vector<Tutor> tutors;
    for(unsigned short int i=0; i<globalConsts::NB_TUTORS; ++i)
    {
        string name = (i<9) ? "Tutor_0" + to_string(i+1) : "Tutor_" + to_string(i+1);

        tutors.emplace_back(i, name, 1);
    }

    // Create students and assign referent teachers and assign tutors
    vector<Student> students;
    for(unsigned short int i=0; i<globalConsts::NB_STUDENTS; ++i)
    {
        Student currentStudent;
        currentStudent.mId = i;
        currentStudent.mName = (i<9) ? "Student_0" + to_string(i+1) : "Student_" + to_string(i+1);
        currentStudent.mHasAccommodations = (i % 4 == 0); // Quarter have accommodations
        currentStudent.mEffectivePresentationLength = currentStudent.mHasAccommodations ? globalConsts::ACCOMODATED_PRESENTATION_LENGTH : globalConsts::NORMAL_PRESENTATION_LENGTH;

        // assign referent teacher using round-robin format
        currentStudent.mReferentTeacherId = i % globalConsts::NB_TEACHERS;

        currentStudent.mTutorId = i;

        students.push_back(currentStudent);
    }

    // Create rooms
    vector<Room> rooms;
    for(unsigned short int i=0; i<globalConsts::NB_ROOMS; ++i)
    {
        string name = (i<9) ? "Room_0" + to_string(i+1) : "Room_" + to_string(i+1);

        rooms.emplace_back(i, name, 1);
    }

    // Create scheduler
    Scheduler schedule(students, teachers, tutors, rooms);

    bool schedulingState = schedule.scheduleAll();

    if (!schedulingState)
    {
        std::cerr << "Scheduling failed.\n";
        return 1;
    }

    Utils::printParameters();

    schedule.printSchedule();
    
    return 0;
}
