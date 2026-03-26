#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <string>

using std::vector;
using std::string;

/**
 * @brief Represents a room available for use in the algorithm
 */
struct Room
{
    unsigned short int mId{};
    string mTag;
    vector<int> mMorningPointerByDay; /// Pointer to the next start minute of the morning for that room
    vector<int> mAfternoonPointerByDay; /// Pointer to the next start minute of the afternoon for that room

    /**
     * @brief Construct a new Room object
     *
     * @param pId
     * @param pTag
     */
    explicit Room(unsigned short int pId, string pTag);
};

#endif // ROOM_H