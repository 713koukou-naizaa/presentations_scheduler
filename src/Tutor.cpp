#include "Tutor.h"

#include <utility>

Tutor::Tutor() = default;

Tutor::Tutor(const unsigned short int pId, string pName, const unsigned short int pInitialDays) : mId(pId), mName(std::move(pName)), mBusyByDay(pInitialDays) {}

void Tutor::book(const unsigned short int pDay, const Utils::Interval pSlot)
{
    if (pDay >= static_cast<unsigned short int>(this->mBusyByDay.size())) this->mBusyByDay.resize(pDay+1);

    this->mBusyByDay[pDay].push_back(pSlot);
}