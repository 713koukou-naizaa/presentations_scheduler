#include <bits/stdc++.h>

#include "config.h"
#include "Utils.h"
#include "Teacher.h"
#include "Student.h"
#include "Room.h"
#include "Scheduler.h"

using namespace std;


int main(const int argc, char* argv[])
{
    if (argc != 9)
    {
        std::cerr << "Usage: " << argv[0] << " <start_morning_time> <end_morning_time> <start_afternoon_time> <end_afternoon_time> <normal_presentation_length> <accommodated_presentation_length> <in_between_break_length> <max_teachers_weekly_worked_time>\n";
        return 1;
    }

    // Parameters extraction
    GLOBAL_CONFIG.START_MORNING_TIME=std::stoi(argv[1]);
    GLOBAL_CONFIG.END_MORNING_TIME=std::stoi(argv[2]);
    GLOBAL_CONFIG.START_AFTERNOON_TIME=std::stoi(argv[3]);
    GLOBAL_CONFIG.END_AFTERNOON_TIME=std::stoi(argv[4]);

    GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH=std::stoi(argv[5]);
    GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH=std::stoi(argv[6]);
    GLOBAL_CONFIG.IN_BETWEEN_BREAK_LENGTH=std::stoi(argv[7]);

    GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME=std::stoi(argv[8]);

    // Create students, teachers and rooms from stdin JSON
    const vector<Student> students = Utils::loadStudentsFromStdin();
    const vector<Teacher> teachers = Utils::loadTeachersFromStdin();
    const vector<Room> rooms = Utils::loadRoomsFromStdin();

    GLOBAL_CONFIG.NB_STUDENTS=students.size();
    GLOBAL_CONFIG.NB_TEACHERS=teachers.size();
    GLOBAL_CONFIG.NB_ROOMS=rooms.size();

    //Utils::displayVectors(students, teachers, tutors, rooms);

    // Create scheduler
    Scheduler schedule(students, teachers, rooms);

    if (const bool schedulingState = schedule.scheduleAll(); !schedulingState)
    {
        std::cerr << "Scheduling failed.\n";
        return 1;
    }

    //Utils::printParameters();

    //schedule.printSchedule();

    cout << schedule.outputJSONResult();
    
    return 0;
}
