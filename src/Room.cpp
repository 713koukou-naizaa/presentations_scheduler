#include "Room.h"

#include <utility>

#include "config.h"

Room::Room(const int pId, string pTag) : mId(pId), mTag(std::move(pTag)) {}