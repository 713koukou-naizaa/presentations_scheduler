#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>

using std::vector;
using std::string;

struct Room
{
    unsigned short int mId{};
    string mTag;
    // per day, we keep two pointers: next available minute in morning and in afternoon
    vector<int> mMorningPointerByDay; // next start minute in morning for that room
    vector<int> mAfternoonPointerByDay; // next start minute in afternoon for that room

    Room();
    Room(int, string , int);
};

#endif