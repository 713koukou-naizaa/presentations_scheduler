#ifndef CONFIG_H
#define CONFIG_H

#include <string>

using std::string;

struct Config
{
    // CONSTANTS
   unsigned short int START_MORNING_TIME;
   unsigned short int END_MORNING_TIME;
   unsigned short int START_AFTERNOON_TIME;
   unsigned short int END_AFTERNOON_TIME;

   unsigned short int NORMAL_PRESENTATION_LENGTH;
   unsigned short int ACCOMMODATED_PRESENTATION_LENGTH;
   unsigned short int IN_BETWEEN_BREAK_LENGTH;

   unsigned short int MAX_TEACHERS_WEEKLY_WORKED_TIME;

   unsigned short int NB_STUDENTS;
   unsigned short int NB_TEACHERS;
   unsigned short int NB_ROOMS;

   unsigned short int NB_TUTORS;

    string JSON_DIR_PATH;
};

extern Config GLOBAL_CONFIG;

#endif