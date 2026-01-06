#include <bits/stdc++.h>

#include "config.h"
#include "Utils.h"
#include "Teacher.h"
#include "Tutor.h"
#include "Student.h"
#include "Room.h"
#include "Scheduler.h"

using namespace std;

Config GLOBAL_CONFIG;

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

    // Create teachers, tutors, students and rooms from json
    const vector<Teacher> teachers = Utils::loadTeachersFromJson("json/teachers.json");
    const vector<Tutor> tutors = Utils::loadTutorsFromJson("json/tutors.json");
    const vector<Student> students = Utils::loadStudentsFromJson("json/students.json");
    const vector<Room> rooms = Utils::loadRoomsFromJson("json/rooms.json");

    GLOBAL_CONFIG.NB_STUDENTS=students.size();
    GLOBAL_CONFIG.NB_TEACHERS=teachers.size();
    GLOBAL_CONFIG.NB_ROOMS=rooms.size();
    GLOBAL_CONFIG.NB_TUTORS=tutors.size();

    //Utils::displayVectors(students, teachers, tutors, rooms);

    // Create scheduler
    Scheduler schedule(students, teachers, tutors, rooms);

    bool schedulingState = schedule.scheduleAll();

    if (!schedulingState)
    {
        std::cerr << "Scheduling failed.\n";
        return 1;
    }

    //Utils::printParameters();

    //schedule.printSchedule();

    // Build planning summary (single object)
    unsigned short jsonJourFinNumero = 0;
    for (const auto &a : schedule.mAssignments) if (a.mDay > jsonJourFinNumero) jsonJourFinNumero = a.mDay;
    unsigned short jsonHeureDebutMatin = GLOBAL_CONFIG.START_MORNING_TIME;
    unsigned short jsonHeureFinMatin   = GLOBAL_CONFIG.END_MORNING_TIME;
    unsigned short jsonHeureDebutAprem = GLOBAL_CONFIG.START_AFTERNOON_TIME;
    unsigned short jsonHeureFinAprem   = GLOBAL_CONFIG.END_AFTERNOON_TIME;
    unsigned short jsonDureeSoutenance = GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH;

    nlohmann::json planning = nlohmann::json::array();
    planning.push_back(nlohmann::json{
        {"jourFinNumero", jsonJourFinNumero},
        {"heureDebutMatin", jsonHeureDebutMatin},
        {"heureFinMatin", jsonHeureFinMatin},
        {"heureDebutAprem", jsonHeureDebutAprem},
        {"heureFinAprem", jsonHeureFinAprem},
        {"dureeSoutenance", jsonDureeSoutenance}
    });

    std::ofstream ofsPlanning("./json/planning.json", std::ios::trunc);
    if (!ofsPlanning.is_open()) std::cerr << "Failed to write `planning.json`\n";
    else { ofsPlanning << planning.dump(4); ofsPlanning.close(); }

    // Build soutenances (one object per assignment)
    nlohmann::json soutenances = nlohmann::json::array();
    for (const auto &a : schedule.mAssignments)
    {
        soutenances.push_back(nlohmann::json{
            {"jourNumero", a.mDay},
            {"heureDebut", a.mStartMinute},
            {"heureFin", static_cast<unsigned short>(a.mStartMinute + a.mDuration)},
            {"nomSalle", schedule.mRooms[a.mRoomId].mTag},
            {"idLecteur", a.mSecondTeacherId}
        });
    }

    std::ofstream ofsSout("./json/soutenances.json", std::ios::trunc);
    if (!ofsSout.is_open()) std::cerr << "Failed to write `soutenances.json`\n";
    else { ofsSout << soutenances.dump(4); ofsSout.close(); }

    // Build salles (one object per assignment)
    nlohmann::json salles = nlohmann::json::array();
    for (const auto &a : schedule.mAssignments)
    {
        salles.push_back(nlohmann::json{
            {"nomSalle", schedule.mRooms[a.mRoomId].mTag},
            {"estDisponible", true}
        });
    }

    std::ofstream ofsSalles("./json/salles.json", std::ios::trunc);
    if (!ofsSalles.is_open()) std::cerr << "Failed to write `salles.json`\n";
    else { ofsSalles << salles.dump(4); ofsSalles.close(); }
    
    return 0;
}
