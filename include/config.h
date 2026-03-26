#ifndef CONFIG_H
#define CONFIG_H

#include <string>

using std::string;

struct Config
{
    // CONSTANTS
   unsigned short int START_MORNING_TIME; /// Starting morning time in minutes for the schedule
   unsigned short int END_MORNING_TIME; /// Ending morning time in minutes for the schedule
   unsigned short int START_AFTERNOON_TIME; /// Starting afternoon time in minutes for the schedule
   unsigned short int END_AFTERNOON_TIME; /// Ending afternoon time in minutes for the schedule

   unsigned short int NORMAL_PRESENTATION_LENGTH; /// Default presentation time if a student doesn't have accomodations
   unsigned short int ACCOMMODATED_PRESENTATION_LENGTH; /// Presentation time if a student has accomodations
   unsigned short int IN_BETWEEN_BREAK_LENGTH; /// Length in minutes of the break between each presentation

   unsigned short int MAX_TEACHERS_WEEKLY_WORKED_TIME; /// Maximum weekly worked time in minutes for teachers

   unsigned short int NB_STUDENTS; /// Number of students to schedule
   unsigned short int NB_TEACHERS; /// Number of teachers available for the schedule
   unsigned short int NB_ROOMS; /// Number of rooms available for the schedule
};

extern Config GLOBAL_CONFIG; /// Global configuration variable

#endif // CONFIG_H