#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <vector>

#include "config.h"
#include "Student.h"
#include "Teacher.h"
#include "Room.h"
#include "Presentation.h"
#include "Utils.h"

using std::string;
using std::vector;

struct Scheduler
{
    vector<Student> mStudents;
    vector<Teacher> mTeachers;
    vector<Room> mRooms;
    int mBreakLength = GLOBAL_CONFIG.IN_BETWEEN_BREAK_LENGTH;

    vector<Presentation> mAssignments;

    // Initialise scheduler with some capacity of days
    Scheduler(vector<Student>, vector<Teacher>, vector<Room>);

    // Ensure internal per-day containers are big enough
    void ensureDayCapacity(unsigned short int);

    // Try to schedule all students. Greedy earliest-fit; creates additional days if needed.
    bool scheduleAll();

    // Check participants availability and constraints for the given student, day, slot, room
    bool canPlace(const Student &, unsigned short int, const Utils::Interval &);

    // Place the presentation: pick a specific second teacher (random among valid), book everything, advance room pointer
    void place(const Student &, unsigned short int, const Utils::Interval &, unsigned short int);

    // Print final schedule
    void printSchedule();

    // Output final schedule to a JSON file
    [[ nodiscard ]] string outputJSONResult() const;
};

#endif