#include "Utils.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "config.h"

#include "Student.h"
#include "Teacher.h"
#include "Room.h"

using std::cout;
using std::endl;

void Utils::setGlobalConfig(const unsigned short int pStartMorningTime, const unsigned short int pEndMorningTime, const unsigned short int pStartAfternoonTime, const unsigned short int pEndAfternoonTime, const unsigned short int pNormalPresentationLength, const unsigned short int pAccommodatedPresentationLength, const unsigned short int pInBetweenBreakLength, const unsigned short int pMaxTeachersWeeklyWorkedTime, const unsigned short int pNbStudents, const unsigned short int pNbTeachers, const unsigned short int pNbRooms)
{
    GLOBAL_CONFIG.START_MORNING_TIME = pStartMorningTime;
    GLOBAL_CONFIG.END_MORNING_TIME = pEndMorningTime;
    GLOBAL_CONFIG.START_AFTERNOON_TIME = pStartAfternoonTime;
    GLOBAL_CONFIG.END_AFTERNOON_TIME = pEndAfternoonTime;

    GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH = pNormalPresentationLength;
    GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH = pAccommodatedPresentationLength;
    GLOBAL_CONFIG.IN_BETWEEN_BREAK_LENGTH = pInBetweenBreakLength;

    GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME = pMaxTeachersWeeklyWorkedTime;

    GLOBAL_CONFIG.NB_STUDENTS = pNbStudents;
    GLOBAL_CONFIG.NB_TEACHERS = pNbTeachers;
    GLOBAL_CONFIG.NB_ROOMS = pNbRooms;
}

// GCOVR_EXCL_START
void Utils::displayVectors(const vector<Student> &pStudents, const vector<Teacher> &pTeachers, const vector<Room> &pRooms)
{
    // display all vectors
    cout << "Students: " << endl;
    for (const auto &student : pStudents)
    {
        cout << "\t" << student.mId << " " << student.mName << " " << student.mHasAccommodations << " " << student.mEffectivePresentationLength << " " << student.mReferentTeacherId << endl;
    }

    cout << "Teachers: " << endl;
    for (const auto &teacher : pTeachers)
    {
        cout << "\t" << teacher.mId << " " << teacher.mName << " " << teacher.mIsTechnical << " " << teacher.mWeeklyRemainingMinutes << endl;
    }

    cout << "Rooms: " << endl;
    for (const auto &room : pRooms)
    {
        cout << "\t" << room.mId << " " << room.mTag << endl;
    }
}
// GCOVR_EXCL_STOP

nlohmann::json Utils::readNextJsonFromStdin()
{
    string line;
    // Read next non-empty line from stdin
    while (std::getline(std::cin, line))
    {
        // trim simple whitespace-only lines
        bool allSpace = true;
        for (const char c : line) { if (!std::isspace(static_cast<unsigned char>(c))) { allSpace = false; break; } }
        if (!allSpace) break;
    }

    if (line.empty()) { throw std::runtime_error("Unable to read JSON array from stdin"); }

    return nlohmann::json::parse(line);
}

vector<Student> Utils::loadStudentsFromStdin()
{
    nlohmann::json jsonInstance = readNextJsonFromStdin();

    vector<Student> students;
    students.reserve(jsonInstance.size());

    for (const auto &currentJsonItem : jsonInstance)
    {
        Student currentStudent;
        currentStudent.mId = currentJsonItem.at("id").get<unsigned int>();
        currentStudent.mName = currentJsonItem.at("name").get<string>();
        currentStudent.mHasAccommodations = currentJsonItem.at("hasAccommodations").get<bool>();
        currentStudent.mEffectivePresentationLength = currentStudent.mHasAccommodations ? GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH : GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH;
        currentStudent.mReferentTeacherId = currentJsonItem.at("referentTeacherId").get<unsigned short int>();

        students.push_back(currentStudent);
    }

    return students;
}

vector<Teacher> Utils::loadTeachersFromStdin()
{
    nlohmann::json jsonInstance = readNextJsonFromStdin();

    vector<Teacher> teachers;
    teachers.reserve(jsonInstance.size());

    for (const auto &currentJsonItem : jsonInstance)
    {
        Teacher currentTeacher;
        currentTeacher.mId = currentJsonItem.at("id").get<unsigned short int>();
        currentTeacher.mName = currentJsonItem.at("name").get<string>();
        currentTeacher.mIsTechnical = currentJsonItem.at("isTechnical").get<bool>();

        teachers.push_back(currentTeacher);
    }

    return teachers;
}

vector<Room> Utils::loadRoomsFromStdin()
{
    nlohmann::json jsonInstance = readNextJsonFromStdin();

    vector<Room> rooms;
    rooms.reserve(jsonInstance.size());

    for (const auto &currentJsonItem : jsonInstance)
    {
        Room currentRoom;
        currentRoom.mId = currentJsonItem.at("id").get<unsigned short int>();
        currentRoom.mTag = currentJsonItem.at("tag").get<string>();

        rooms.push_back(currentRoom);
    }

    return rooms;
}

string Utils::minutesToHHMM(const unsigned short int pMinutes)
{
    const unsigned short int hh = pMinutes / 60;
    const unsigned short int mm = pMinutes % 60;
    char buf[8];
    (void)snprintf(buf, sizeof(buf), "%02d:%02d", hh, mm);
    return buf;
}

// GCOVR_EXCL_START
void Utils::printParameters()
{
    cout << "Parameters:" << endl;

    cout << endl;

    cout << "Quantities:" << endl;
    cout << "\tNumber of students: " << GLOBAL_CONFIG.NB_STUDENTS << endl;
    cout << "\tNumber of teachers: " << GLOBAL_CONFIG.NB_TEACHERS << endl;
    cout << "\tNumber of rooms: " << GLOBAL_CONFIG.NB_ROOMS << endl;

    cout << endl;

    cout << "Time lengths:" << endl;
    cout << "\tPresentation length: " << GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH << " minutes" << endl;
    cout << "\tAccommodated presentation length: " << GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH << " minutes" << endl;
    cout << "\tIn-between break length: " << GLOBAL_CONFIG.IN_BETWEEN_BREAK_LENGTH << " minutes" << endl;
    cout << "\tMax teachers weekly worked time: " << GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME << " minutes" << endl;

    cout << endl;

    cout << "Times:" << endl;
    cout << "\tStart morning time: " << minutesToHHMM(GLOBAL_CONFIG.START_MORNING_TIME) << endl;
    cout << "\tEnd morning time: " << minutesToHHMM(GLOBAL_CONFIG.END_MORNING_TIME) << endl;
    cout << "\tStart afternoon time: " << minutesToHHMM(GLOBAL_CONFIG.START_AFTERNOON_TIME) << endl;
    cout << "\tEnd afternoon time: " << minutesToHHMM(GLOBAL_CONFIG.END_AFTERNOON_TIME) << endl;

    cout << endl;
}
// GCOVR_EXCL_STOP

bool Utils::Interval::overlaps(const Interval &pOther) const { return !(this->mEnd <= pOther.mStart || pOther.mEnd <= this->mStart); }