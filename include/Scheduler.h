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

    enum TrySchedulingResultOptions { SCHEDULED, SAME_DAY, NEXT_WEEK, NOT_POSSIBLE };

    // Initialize scheduler with some capacity of days
    Scheduler(vector<Student>, vector<Teacher>, vector<Room>);

    // Ensure internal per-day containers are big enough
    void ensureDayCapacity(unsigned short int);

    // Try to schedule all students. Greedy earliest-fit; creates additional days if needed.
    bool scheduleAll();

    Scheduler::TrySchedulingResultOptions tryScheduleStudentAtSlotAtDayAtRoom(const unsigned short int &, const unsigned short int &, const Utils::Interval &, const unsigned short int &);

    void trySchedulingUntilTimeWindowFull(unsigned short dayNumber, unsigned short int currentRoomIdx, int& startTime, int endTime,
    std::vector<unsigned short int>& toRetrySchedulingSameDay, std::vector<unsigned short int>& toScheduleStudents, std::vector<unsigned short int>& toRetrySchedulingNextWeekStudents,
    bool& madeProgressThisDay);

    static void removeStudentFromAllVectors(vector<unsigned short int> &, vector<unsigned short int> &, vector<unsigned short int> &, const unsigned short int &);

    static void removeStudentFromVector(vector<unsigned short int> &, const unsigned short int &);

    // Print final schedule
    void printSchedule();

    // Output final schedule to a JSON file
    [[ nodiscard ]] string outputJSONResult() const;
};

#endif