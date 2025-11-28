#include "Room.h"

#include <utility>

#include "config.h"

Room::Room() = default;

Room::Room(const int pId, string pTag, const int pInitialDays) : mId(pId), mTag(std::move(pTag)), mMorningPointerByDay(pInitialDays, GLOBAL_CONFIG.START_MORNING_TIME), mAfternoonPointerByDay(pInitialDays, GLOBAL_CONFIG.START_AFTERNOON_TIME) {}