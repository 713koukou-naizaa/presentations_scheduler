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
     * @brief Ensures that the schedule has enough days to try scheduling a student on the given day, by adding new days if needed
     *
     * @param pDay The day for which to ensure capacity (0-based)
     *
     * @return void
     */
    void ensureDayCapacity(unsigned short int pDay);

    /**
     * @brief Tries to schedule all students
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
    Scheduler::TrySchedulingResultOptions tryScheduleStudentAtSlotAtDayAtRoom(const unsigned short int &, const unsigned short int &, const Utils::Interval &, const unsigned short int &);

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