#ifndef TEACHER_H
#define TEACHER_H

#include <vector>

#include "config.h"
#include "Utils.h"

using std::vector;

struct Teacher
{
    unsigned short int mId{};
    bool mIsTechnical{};
    unsigned short int mWeeklyRemainingMinutes=GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME; // minutes left for presentations this week
    // schedule for avoiding overlaps: vector of (day -> intervals)
    vector<vector<Utils::Interval>> mBusyByDay; // mBusyByDay[day] = list of intervals

    Teacher(unsigned short int, bool);

    bool isAvailable(unsigned short int, Utils::Interval);
    void book(unsigned short int, Utils::Interval);
};

#endif