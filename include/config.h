#ifndef GLOBALCONSTS_H
#define GLOBALCONSTS_H

namespace globalConsts
{
    // CONSTANTS
    inline constexpr unsigned short int START_MORNING_TIME = 7 * 60; // 07:00
    inline constexpr unsigned short int END_MORNING_TIME = 12 * 60 + 30; // 12:00
    inline constexpr unsigned short int START_AFTERNOON_TIME = 14 * 60; // 14:00
    inline constexpr unsigned short int END_AFTERNOON_TIME = 17 * 60; // 17:00

    inline constexpr unsigned short int NORMAL_PRESENTATION_LENGTH = 60; // 60 minutes
    inline constexpr unsigned short int ACCOMODATED_PRESENTATION_LENGTH = 80; // 80 minutes if student has accommodations
    inline constexpr unsigned short int IN_BETWEEN_BREAK_LENGTH = 5; // 5 minutes break after each presentation

    inline constexpr unsigned short int MAX_TEACHERS_WEEKLY_WORKED_TIME = 20 * 60; // 20 hours per teacher per week

    // Worst case scenario
    // inline constexpr unsigned short int NB_STUDENTS = 62;
    // inline constexpr unsigned short int NB_TEACHERS = 10;
    // inline constexpr unsigned short int NB_ROOMS = 7;
    
    // Average scenario 1
    // inline constexpr unsigned short int NB_STUDENTS = 60;
    // inline constexpr unsigned short int NB_TEACHERS = 12;
    // inline constexpr unsigned short int NB_ROOMS = 8;

    // Average scenario 2
    inline constexpr unsigned short int NB_STUDENTS = 60;
    inline constexpr unsigned short int NB_TEACHERS = 15;
    inline constexpr unsigned short int NB_ROOMS = 8;

    // Good scenario
    // inline constexpr unsigned short int NB_STUDENTS = 55;
    // inline constexpr unsigned short int NB_TEACHERS = 14;
    // inline constexpr unsigned short int NB_ROOMS = 9;

    inline constexpr unsigned short int NB_TUTORS = NB_STUDENTS; // 1 tutor per student (doesn't matter)
}

#endif