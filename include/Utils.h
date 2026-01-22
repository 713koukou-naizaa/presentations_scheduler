#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

struct Teacher;
struct Room;
struct Student;

#include "json.hpp"

using std::string;
using std::vector;

struct Utils
{
    static void setGlobalConfig(unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int);

    static void displayVectors(const vector<Student> &, const vector<Teacher> &, const vector<Room> &);

    static nlohmann::json readNextJsonFromStdin();

    static vector<Student> loadStudentsFromStdin();
    static vector<Teacher> loadTeachersFromStdin();
    static vector<Room> loadRoomsFromStdin();

    static string minutesToHHMM(unsigned short int);

    static void printParameters();

    struct Interval
    {
        unsigned short int mStart; // inclusive minutes from midnight
        unsigned short int mEnd; // exclusive minutes
        [[nodiscard]] bool overlaps(const Interval &) const;
    };
};

#endif