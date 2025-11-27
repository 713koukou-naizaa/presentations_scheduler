#ifndef TUTOR_H
#define TUTOR_H

#include <string>
#include <vector>

#include "Utils.h"

using std::string;
using std::vector;

struct Tutor {
    unsigned short int mId{};
    string mName;
    vector<vector<Utils::Interval>> mBusyByDay;
    
    Tutor();
    Tutor(unsigned short int, string , unsigned short int);

    void book(unsigned short int, Utils::Interval);
};

#endif