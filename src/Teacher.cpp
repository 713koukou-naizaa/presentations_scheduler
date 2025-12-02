#include "Teacher.h"

#include <algorithm>
#include <utility>

Teacher::Teacher() = default;

Teacher::Teacher(const unsigned short int pId, string pName, const bool pIsTechnical, const unsigned short int pWeeklyLimit, const unsigned short int pInitialDays) : mId(pId), mName(std::move(pName)), mIsTechnical(pIsTechnical), mWeeklyRemainingMinutes(pWeeklyLimit), mBusyByDay(pInitialDays) {}

bool Teacher::isAvailable(const unsigned short int pDay, const Utils::Interval pSlot)
{
    if (this->mWeeklyRemainingMinutes < pSlot.mEnd - pSlot.mStart) return false;

    if (pDay >= static_cast<unsigned short int>(this->mBusyByDay.size())) this->mBusyByDay.resize(pDay+1);

    for(auto &it : this->mBusyByDay[pDay]) if (it.overlaps(pSlot)) return false;

    return true;
}

void Teacher::book(const unsigned short int pDay, const Utils::Interval pSlot)
{
    if (pDay >= static_cast<unsigned short int>(this->mBusyByDay.size())) this->mBusyByDay.resize(pDay+1);

    this->mBusyByDay[pDay].push_back(pSlot);
    this->mWeeklyRemainingMinutes -= (pSlot.mEnd - pSlot.mStart);
}
