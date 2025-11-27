#ifndef UTILS_H
#define UTILS_H

#include <string>

using std::string;

struct Utils
{
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