#include "Utils.h"

#include <iostream>

#include "config.h"

using std::cout;
using std::endl;

string Utils::minutesToHHMM(const unsigned short int pMinutes)
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
    cout << "\tNumber of students: " << GLOBAL_CONFIG.NB_STUDENTS << endl;
    cout << "\tNumber of teachers: " << GLOBAL_CONFIG.NB_TEACHERS << endl;
    cout << "\tNumber of rooms: " << GLOBAL_CONFIG.NB_ROOMS << endl;
    cout << "\tNumber of tutors: " << GLOBAL_CONFIG.NB_TUTORS << endl;

    cout << endl;

    cout << "Time lengths:" << endl;
    cout << "\tPresentation length: " << GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH << " minutes" << endl;
    cout << "\tAccommodated presentation length: " << GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH << " minutes" << endl;
    cout << "\tIn-between break length: " << GLOBAL_CONFIG.IN_BETWEEN_BREAK_LENGTH << " minutes" << endl;
    cout << "\tMax teachers weekly worked time: " << GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME << " minutes" << endl;

    cout << endl;

    cout << "Times:" << endl;
    cout << "\tStart morning time: " << Utils::minutesToHHMM(GLOBAL_CONFIG.START_MORNING_TIME) << endl;
    cout << "\tEnd morning time: " << Utils::minutesToHHMM(GLOBAL_CONFIG.END_MORNING_TIME) << endl;
    cout << "\tStart afternoon time: " << Utils::minutesToHHMM(GLOBAL_CONFIG.START_AFTERNOON_TIME) << endl;
    cout << "\tEnd afternoon time: " << Utils::minutesToHHMM(GLOBAL_CONFIG.END_AFTERNOON_TIME) << endl;

    cout << endl;
}

bool Utils::Interval::overlaps(const Interval &pOther) const { return !(this->mEnd <= pOther.mStart || pOther.mEnd <= this->mStart); }