#include "Teacher.h"


Teacher::Teacher(const unsigned short int pId, const bool pIsTechnical) : mId(pId), mIsTechnical(pIsTechnical) {}

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
    this->mWeeklyRemainingMinutes -= pSlot.mEnd - pSlot.mStart;
}
