#ifndef TEACHER_H
#define TEACHER_H

#include <vector>

#include "config.h"
#include "Utils.h"

using std::vector;

/**
 * @brief Represents a teacher concerned by the scheduling
 */
struct Teacher
{
    unsigned short int mId{};
    bool mIsTechnical{}; /// Whether the teacher is technical or not (technical meaning they have recognized computer science skills)
    unsigned short int mWeeklyRemainingMinutes=GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME;
    vector<vector<Utils::Interval>> mBusyByDay; /// Schedule for avoiding overlaps: vector of day -> intervals (this->mBusyByDay[day] = list of intervals)

    /**
     * @brief Construct a new Teacher object
     *
     * @param pId
     * @param pIsTechnical
     */
    Teacher(unsigned short int pId, bool pIsTechnical);

    /**
     * @brief Checks if the teacher is available for the given interval on the given day.
     * Checks if the teacher has enough weekly remaining minutes to book the given interval,
     * and if the teacher is not busy for the given interval on the given day.
     *
     * @param pDay Concerned day
     * @param pSlot Concerned interval
     *
     * @return true if the teacher is available for the given interval on the given day, false otherwise
     */
    bool isAvailable(unsigned short int pDay, Utils::Interval pSlot);

    /**
     * @brief Books the teacher for the given interval on the given day.
     * Adds the given interval to the teacher's busy schedule for that day,
     * and decreases the teacher's weekly remaining minutes by the length of the given interval.
     *
     * @param pDay Concerned day
     * @param pSlot Concerned interval
     *
     * @return void
     */
    void book(unsigned short int pDay, Utils::Interval pSlot);
};

#endif // TEACHER_H