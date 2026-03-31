#include "Room.h"

#include <utility>


Room::Room(const unsigned short int pId, string pTag) : mId(pId), mTag(std::move(pTag)) {}