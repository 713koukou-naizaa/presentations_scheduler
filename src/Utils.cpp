#include "Utils.h"

#include <iostream>

#include "globalConsts.h"

using std::cout;
using std::endl;

string Utils::minutesToHHMM(unsigned short int pMinutes)
{
    const unsigned short int hh = pMinutes / 60;
    const unsigned short int mm = pMinutes % 60;
    char buf[8];
    snprintf(buf, sizeof(buf), "%02d:%02d", hh, mm);
    return buf;
}

void Utils::printParameters()
{
    cout << "Parameters:" << endl;

    cout << endl;

    cout << "Quantities:" << endl;
    cout << "\tNumber of students: " << globalConsts::NB_STUDENTS << endl;
    cout << "\tNumber of teachers: " << globalConsts::NB_TEACHERS << endl;
    cout << "\tNumber of rooms: " << globalConsts::NB_ROOMS << endl;
    cout << "\tNumber of tutors: " << globalConsts::NB_TUTORS << endl;

    cout << endl;

    cout << "Time lengths:" << endl;
    cout << "\tPresentation length: " << globalConsts::NORMAL_PRESENTATION_LENGTH << " minutes" << endl;
    cout << "\tAccommodated presentation length: " << globalConsts::ACCOMODATED_PRESENTATION_LENGTH << " minutes" << endl;
    cout << "\tIn-between break length: " << globalConsts::IN_BETWEEN_BREAK_LENGTH << " minutes" << endl;
    cout << "\tMax teachers weekly worked time: " << globalConsts::MAX_TEACHERS_WEEKLY_WORKED_TIME << " minutes" << endl;

    cout << endl;

    cout << "Times:" << endl;
    cout << "\tStart morning time: " << Utils::minutesToHHMM(globalConsts::START_MORNING_TIME) << endl;
    cout << "\tEnd morning time: " << Utils::minutesToHHMM(globalConsts::END_MORNING_TIME) << endl;
    cout << "\tStart afternoon time: " << Utils::minutesToHHMM(globalConsts::START_AFTERNOON_TIME) << endl;
    cout << "\tEnd afternoon time: " << Utils::minutesToHHMM(globalConsts::END_AFTERNOON_TIME) << endl;

    cout << endl;
}

bool Utils::Interval::overlaps(const Interval &pOther) const { return !(this->mEnd <= pOther.mStart || pOther.mEnd <= this->mStart); }