#include "../include/Scheduler.h"

#include <gtest/gtest.h>
#include <vector>
#include <random>

#include "../include/config.h"

#include "../include/Room.h"
#include "../include/Student.h"
#include "../include/Teacher.h"


class SchedulerTest : public testing::Test
{
protected:
  vector<Student> students;
  vector<Teacher> teachers;
  vector<Room> rooms;

  class TestRNG
  {
  public:
    static std::mt19937& rng()
    {
      static std::mt19937 gen(42);
      return gen;
    }

    static bool randBool(const int trueEvery = 2)
    {
      std::uniform_int_distribution<> d(1, trueEvery);
      return d(rng()) == 1;
    }

    static int randInt(const int min, const int max)
    {
      std::uniform_int_distribution<> d(min, max);
      return d(rng());
    }
  };

  static std::vector<Teacher> generateTeachers(const int pCount = 15)
  {
    std::vector<Teacher> teachers;
    teachers.reserve(pCount);

    for (int i = 0; i < pCount; ++i)
    {
      Teacher t(i, string("Teacher_") + (i < 10 ? "0": "") + std::to_string(i), TestRNG::randBool(2));
      t.mWeeklyRemainingMinutes = 1200;
      teachers.push_back(t);
    }

    return teachers;
  }

  static std::vector<Student> generateStudents(const int pStudentCount = 60, const int pTeacherCount = 15)
  {
    std::vector<Student> students;
    students.reserve(pStudentCount);

    for (int i = 0; i < pStudentCount; ++i)
    {
      Student currentStudent(i, string("Student_") + (i < 10 ? "0": "") + std::to_string(i), SchedulerTest::TestRNG::randBool(4), 0, SchedulerTest::TestRNG::randInt(0, std::max(0, pTeacherCount - 1)));
      currentStudent.mEffectivePresentationLength = currentStudent.mHasAccommodations ? GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH : GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH;
      students.push_back(currentStudent);
    }

    return students;
  }

  static std::vector<Room> generateRooms(const int pCount = 8)
  {
    std::vector<Room> rooms;
    rooms.reserve(pCount);

    for (int i = 0; i < pCount; ++i)
    {
      Room currentRoom(i, string("Room_") + (i < 10 ? "0": "") + std::to_string(i));
      rooms.push_back(currentRoom);
    }

    return rooms;
  }

  static bool overlaps(const unsigned short int pStartA, const unsigned short int pEndA, const unsigned short int pStartB, const unsigned short int pEndB)
  { return pStartA < pEndB && pStartB < pEndA; }

  // ==========================
  // TESTS Class::method
  // ==========================

};