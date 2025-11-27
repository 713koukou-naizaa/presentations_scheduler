#include "Room.h"

#include <utility>

#include "globalConsts.h"

Room::Room() = default;

Room::Room(const int pId, string pTag, const int pInitialDays) : mId(pId), mTag(std::move(pTag)), mMorningPointerByDay(pInitialDays, globalConsts::START_MORNING_TIME), mAfternoonPointerByDay(pInitialDays, globalConsts::START_AFTERNOON_TIME) {}