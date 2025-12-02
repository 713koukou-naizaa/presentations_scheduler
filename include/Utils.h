#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

struct Teacher;
struct Tutor;
struct Room;
struct Student;

#include "json.hpp"

using std::string;
using std::vector;

struct Utils
{
    static void displayVectors(const vector<Student> &, const vector<Teacher> &, const vector<Tutor> &, const vector<Room> &);
    static string loadFileToString(const string &);

    static vector<Teacher> loadTeachersFromJson(const string &);
    static vector<Tutor> loadTutorsFromJson(const string &);
    static vector<Room> loadRoomsFromJson(const string &);
    static vector<Student> loadStudentsFromJson(const string &);

    static string minutesToHHMM(unsigned short int);

    static void printParameters();

    struct Interval
    {
        unsigned short int mStart; // inclusive minutes from midnight
        unsigned short int mEnd; // exclusive minutes
        bool overlaps(const Interval &) const;
    };
};

#endif