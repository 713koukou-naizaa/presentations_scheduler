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
    vector<Student> mStudents; /// The students concerned by the scheduling
    vector<Teacher> mTeachers; /// The teachers concerned by the scheduling
    vector<Room> mRooms; /// The rooms available for the scheduling
    int mBreakLength = GLOBAL_CONFIG.IN_BETWEEN_BREAK_LENGTH;

    vector<Presentation> mAssignments; /// The resulting schedule as a vector of scheduled presentations

    /**
     * @brief Result options for a scheduling attempt
     * SCHEDULED if the student was successfully scheduled
     * SAME_DAY if the student couldn't be scheduled but could be retried later the same day
     * NEXT_WEEK if the student couldn't be scheduled and should be retried next week
     * NOT_POSSIBLE if the student couldn't be scheduled at all
     */
    enum TrySchedulingResultOptions { SCHEDULED, SAME_DAY, NEXT_WEEK, NOT_POSSIBLE };

    /**
     * @brief Construct a new Scheduler object
     *
     * @param pStudents
     * @param pTeachers
     * @param pRooms
     */
    Scheduler(vector<Student> pStudents, vector<Teacher> pTeachers, vector<Room> pRooms);

    /**
     * @brief Ensures the rooms and teacher's day pointers have the correct size for the given day.
     * For each room, resize the morning and afternoon pointer vectors if their size is lesser or equal to the given day
     * For each teacher, resize the busyByDay vector if its size is lesser or equal to the given day
     *
     * @param pDay The day for which to ensure capacity (0-based)
     *
     * @return void
     */
    void ensureDayCapacity(unsigned short int pDay);

    /**
     * @brief Tries to schedule all students
     *
     *
     * @return bool True if all students were scheduled, false otherwise
     */
    bool scheduleAll();

    /**
     * @brief Tries to schedule a student on a specific slot and day
     *
     * @param currentStudentIdx
     * @param currentDay
     * @param currentSlot
     * @param currentRoomId
     *
     * @return Scheduler::TrySchedulingResultOptions The result of the scheduling attempt
     */
    Scheduler::TrySchedulingResultOptions tryScheduleStudentAtSlotAtDayAtRoom(const unsigned short int &currentStudentIdx, const unsigned short int &currentDay, const Utils::Interval &currentSlot, const unsigned short int &currentRoomId);

    /**
     * @brief Tries to schedule all students until the time window is full for the given day, room and starting time, by iterating over the students to schedule and trying to schedule them at that slot, and updating the starting time and the lists of students to schedule accordingly
     *
     * @param dayNumber
     * @param currentRoomIdx
     * @param startTime
     * @param endTime
     * @param toRetrySchedulingSameDay
     * @param toScheduleStudents
     * @param toRetrySchedulingNextWeekStudents
     * @param madeProgressThisDay
     *
     * @return void
     */
    void trySchedulingUntilTimeWindowFull(unsigned short dayNumber, unsigned short int currentRoomIdx, int& startTime, int endTime,
                                          std::vector<unsigned short int>& toRetrySchedulingSameDay, std::vector<unsigned short int>& toScheduleStudents, std::vector<unsigned short int>& toRetrySchedulingNextWeekStudents,
                                          bool& madeProgressThisDay);

    /**
     * @brief Removes a student from all vectors of students to schedule, to retry scheduling same day and to retry scheduling next week, by student index
     *
     * @param toScheduleStudents
     * @param toRetrySchedulingSameDay
     * @param toRetrySchedulingNextWeekStudents
     * @param toRemoveStudentIdx
     *
     * @return void
     */
    static void removeStudentFromAllVectors(vector<unsigned short int> &toScheduleStudents, vector<unsigned short int> &toRetrySchedulingSameDay, vector<unsigned short int> &toRetrySchedulingNextWeekStudents, const unsigned short int &toRemoveStudentIdx);

    /**
     * @brief Removes a student from a vector of student indexes, by student index
     *
     * @param vectorToRemoveFrom
     * @param toRemoveStudentIdx
     *
     * @return void
     */
    static void removeStudentFromVector(vector<unsigned short int> &vectorToRemoveFrom, const unsigned short int &toRemoveStudentIdx);

    /**
     * @brief Prints the resulting schedule in a human-readable format to stdout (debug)
      *
      * @return void
     */
    void printSchedule();

    /**
     * @brief Returns the resulting schedule as a JSON string in the format expected by the web application
     *
     * @return string The resulting schedule as a JSON string in the format expected by the web application
     */
    [[ nodiscard ]] string outputJSONResult() const;
};

#endif // SCHEDULER_H