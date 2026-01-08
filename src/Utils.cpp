#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "config.h"

#include "Student.h"
#include "Teacher.h"
#include "Tutor.h"
#include "Room.h"

using std::cout;
using std::endl;

void Utils::displayVectors(const vector<Student> &pStudents, const vector<Teacher> &pTeachers, const vector<Tutor> &pTutors, const vector<Room> &pRooms)
{
    // display all vectors
    cout << "Students: " << endl;
    for (const auto &student : pStudents)
    {
        cout << "\t" << student.mId << " " << student.mName << " " << student.mHasAccommodations << " " << student.mEffectivePresentationLength << " " << student.mReferentTeacherId << " " << student.mTutorId << endl;
    }

    cout << "Teachers: " << endl;
    for (const auto &teacher : pTeachers)
    {
        cout << "\t" << teacher.mId << " " << teacher.mName << " " << teacher.mIsTechnical << " " << teacher.mWeeklyRemainingMinutes << endl;
    }

    cout << "Tutors: " << endl;
    for (const auto &tutor : pTutors)
    {
        cout << "\t" << tutor.mId << " " << tutor.mName << endl;
    }

    cout << "Rooms: " << endl;
    for (const auto &room : pRooms)
    {
        cout << "\t" << room.mId << " " << room.mTag << endl;
    }
}

string Utils::loadFileToString(const string &pFilePath)
{
    std::ifstream pFileStream(pFilePath);
    if (!pFileStream.is_open()) { throw std::runtime_error("Unable to open file: " + pFilePath); }

    std::stringstream buffer;
    buffer << pFileStream.rdbuf();
    pFileStream.close();
    return buffer.str();
}

vector<Teacher> Utils::loadTeachersFromJson(const string &pJsonPath)
{
    string content = loadFileToString(pJsonPath);

    nlohmann::json jsonInstance = nlohmann::json::parse(content);

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

vector<Tutor> Utils::loadTutorsFromJson(const string &pJsonPath)
{
    string content = loadFileToString(pJsonPath);

    nlohmann::json jsonInstance = nlohmann::json::parse(content);

    vector<Tutor> tutors;
    tutors.reserve(jsonInstance.size());

    for (const auto &currentJsonItem : jsonInstance)
    {
        Tutor currentTutor;
        currentTutor.mId = currentJsonItem.at("id").get<unsigned short int>();
        currentTutor.mName = currentJsonItem.at("name").get<string>();

        tutors.push_back(currentTutor);
    }

    return tutors;
}

vector<Room> Utils::loadRoomsFromJson(const string &pJsonPath)
{
    string content = loadFileToString(pJsonPath);

    nlohmann::json jsonInstance = nlohmann::json::parse(content);

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

vector<Student> Utils::loadStudentsFromJson(const string &pJsonPath)
{
    string content = loadFileToString(pJsonPath);

    nlohmann::json jsonInstance = nlohmann::json::parse(content);

    vector<Student> students;
    students.reserve(jsonInstance.size());

    for (const auto &currentJsonItem : jsonInstance)
    {
        Student currentStudent;
        currentStudent.mId = currentJsonItem.at("id").get<unsigned short int>();
        currentStudent.mName = currentJsonItem.at("name").get<string>();
        currentStudent.mHasAccommodations = currentJsonItem.at("hasAccommodations").get<bool>();
        currentStudent.mEffectivePresentationLength = currentStudent.mHasAccommodations ? GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH : GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH;
        currentStudent.mReferentTeacherId = currentJsonItem.at("referentTeacherId").get<unsigned short int>();
        currentStudent.mTutorId = currentJsonItem.at("tutorId").get<unsigned short int>();

        students.push_back(currentStudent);
    }

    return students;
}


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
    cout << "\tStart morning time: " << minutesToHHMM(GLOBAL_CONFIG.START_MORNING_TIME) << endl;
    cout << "\tEnd morning time: " << minutesToHHMM(GLOBAL_CONFIG.END_MORNING_TIME) << endl;
    cout << "\tStart afternoon time: " << minutesToHHMM(GLOBAL_CONFIG.START_AFTERNOON_TIME) << endl;
    cout << "\tEnd afternoon time: " << minutesToHHMM(GLOBAL_CONFIG.END_AFTERNOON_TIME) << endl;

    cout << endl;
}

bool Utils::Interval::overlaps(const Interval &pOther) const { return !(this->mEnd <= pOther.mStart || pOther.mEnd <= this->mStart); }