#include "Scheduler.h"

#include <gtest/gtest.h>

#include <string>
#include <random>

#include "config.h"

#include "Room.h"
#include "Student.h"
#include "Teacher.h"

using std::string;

class TestRng
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

class SchedulerTest : public testing::Test
{
protected:
  vector<Student> students;
  vector<Teacher> teachers;
  vector<Room> rooms;


  static bool overlaps(const unsigned short int pStartA, const unsigned short int pEndA, const unsigned short int pStartB, const unsigned short int pEndB) { return pStartA < pEndB && pStartB < pEndA; }

  static std::vector<Teacher> generateTeachers(const int pCount = 15)
  {
    std::vector<Teacher> teachers;
    teachers.reserve(pCount);

    for (int i = 0; i < pCount; ++i)
    {
      Teacher t;
      t.mId = i;
      t.mName = string("Teacher_") + (i < 10 ? "0" : "") + std::to_string(i);
      t.mIsTechnical = TestRng::randBool(2);
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
      Student s;
      s.mId = i;
      s.mName = string("Student_") + (i < 10 ? "0" : "") + std::to_string(i);
      s.mHasAccommodations = TestRng::randBool(4);
      s.mReferentTeacherId = TestRng::randInt(0, std::max(0, pTeacherCount - 1));

      s.mEffectivePresentationLength = s.mHasAccommodations ? GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH : GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH;

      students.push_back(s);
    }

    return students;
  }

  static std::vector<Room> generateRooms(const int pCount = 8)
  {
    std::vector<Room> rooms;
    rooms.reserve(pCount);

    for (int i = 0; i < pCount; ++i)
    {
      Room r;
      r.mId = i;
      r.mTag = string("Room_") + (i < 10 ? "0" : "") + std::to_string(i);
      rooms.push_back(r);
    }

    return rooms;
  }
};

// ==========================
// TESTS Utils::ensureDayCapacity
// ==========================

TEST_F(SchedulerTest, EnsureDayCapacityResizesContainers)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 1, 2, 1);
  this->teachers = generateTeachers(2);
  this->rooms = generateRooms(1);
  this->students = generateStudents(1, 2);
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  scheduler.ensureDayCapacity(2);

  EXPECT_EQ(scheduler.mRooms[0].mMorningPointerByDay.size(), 3u);
  EXPECT_EQ(scheduler.mRooms[0].mAfternoonPointerByDay.size(), 3u);
  EXPECT_EQ(scheduler.mTeachers[0].mBusyByDay.size(), 3u);
}

// ==========================
// TESTS Scheduler::canPlace
// ==========================

TEST_F(SchedulerTest, CanPlaceWhenAllConstraintsSatisfied)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 1, 2, 1);
  this->teachers = generateTeachers(2);
  this->teachers[0].mIsTechnical = true;
  this->teachers[1].mIsTechnical = false;
  this->rooms = generateRooms(1);
  this->students = generateStudents(1, 2);
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  scheduler.ensureDayCapacity(0);
  constexpr Utils::Interval slot{480, 540};

  EXPECT_TRUE(scheduler.canPlace(0, 0, slot));
}

TEST_F(SchedulerTest, CanPlaceFailsWhenReferentTeacherMissing)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 1, 2, 1);
  this->teachers = generateTeachers(2);
  this->rooms = generateRooms(1);
  this->students = generateStudents(1, 2);
  this->students[0].mReferentTeacherId = 99;
  Scheduler scheduler(this->students, this->teachers, this->rooms);
  constexpr Utils::Interval slot{480, 540};

  EXPECT_FALSE(scheduler.canPlace(0, 0, slot));
}

TEST_F(SchedulerTest, CanPlaceFailsWhenNoSecondTeacherAvailable)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 1, 2, 1);
  this->teachers = generateTeachers(2);
  this->teachers[1].mWeeklyRemainingMinutes = 0;
  teachers[0].mIsTechnical = true;
  this->rooms = generateRooms(1);
  this->students = generateStudents(1, 2);

  Scheduler scheduler(this->students, this->teachers, this->rooms);
  constexpr Utils::Interval slot{480, 540};

  EXPECT_FALSE(scheduler.canPlace(0, 0, slot));
}

// ==========================
// TESTS Scheduler::place
// ==========================

TEST_F(SchedulerTest, PlaceCreatesAssignmentAndBooksTeachers)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 1, 2, 1);
  this->teachers = generateTeachers(2);
  this->rooms = generateRooms(1);
  this->students = generateStudents(1, 2);
  Scheduler scheduler(this->students, this->teachers, this->rooms);
  constexpr Utils::Interval slot{480, 540};

  scheduler.place(0, 0, slot, 0);

  ASSERT_EQ(scheduler.mAssignments.size(), 1u);
  EXPECT_EQ(scheduler.mTeachers[0].mWeeklyRemainingMinutes, 1140u);
  EXPECT_EQ(scheduler.mTeachers[1].mWeeklyRemainingMinutes, 1140u);
}

// ==========================
// TESTS Scheduler::scheduleAll
// ==========================

TEST_F(SchedulerTest, ScheduleAllProducesOneAssignment)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 1, 2, 1);
  this->teachers = generateTeachers(2);
  this->rooms = generateRooms(1);
  this->students = generateStudents(1, static_cast<unsigned short int>(this->teachers.size()));
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  EXPECT_TRUE(scheduler.scheduleAll());
  EXPECT_EQ(scheduler.mAssignments.size(), 1u);
}

TEST_F(SchedulerTest, SingleStudentSingleSlot)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 1, 2, 1);
  this->teachers = generateTeachers(2);
  this->teachers[0].mIsTechnical = true;
  this->students = generateStudents(1, static_cast<unsigned short int>(this->teachers.size()));
  this->students[0].mReferentTeacherId = this->teachers[0].mId;
  this->students[0].mHasAccommodations = false;
  this->rooms = generateRooms(1);
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  scheduler.scheduleAll();

  ASSERT_EQ(scheduler.mAssignments.size(), 1);
  EXPECT_EQ(scheduler.mAssignments[0].mStudentId, students[0].mId);
  EXPECT_LT(scheduler.mAssignments[0].mStartMinute, GLOBAL_CONFIG.END_MORNING_TIME);
}

TEST_F(SchedulerTest, EachPresentationHasAtLeastOneTechnicalTeacher)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 10, 6, 3);
  this->teachers = generateTeachers(6);
  this->students = generateStudents(10, static_cast<unsigned short int>(this->teachers.size()));
  this->rooms = generateRooms(3);
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  scheduler.scheduleAll();

  for (const auto& a : scheduler.mAssignments)
  {
    const auto& ref = teachers[a.mReferentTeacherId];
    const auto& sec = teachers[a.mSecondTeacherId];

    ASSERT_TRUE(ref.mIsTechnical || sec.mIsTechnical);
  }
}

TEST_F(SchedulerTest, TeachersDoNotOverlap)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 20, 8, 4);
  this->teachers = generateTeachers(8);
  this->students = generateStudents(20, static_cast<unsigned short int>(this->teachers.size()));
  this->rooms = generateRooms(4);
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  scheduler.scheduleAll();

  for (size_t i = 0; i < scheduler.mAssignments.size(); ++i)
  {
    for (size_t j = i + 1; j < scheduler.mAssignments.size(); ++j)
    {
      const auto& a = scheduler.mAssignments[i];
      const auto& b = scheduler.mAssignments[j];

      if (a.mDay != b.mDay) continue;

      if (a.mReferentTeacherId == b.mReferentTeacherId || a.mReferentTeacherId == b.mSecondTeacherId || a.mSecondTeacherId   == b.mReferentTeacherId || a.mSecondTeacherId   == b.mSecondTeacherId)
      { ASSERT_FALSE(overlaps(a.mStartMinute, a.mStartMinute + a.mDuration, b.mStartMinute, b.mStartMinute + b.mDuration)); }
    }
  }
}

TEST_F(SchedulerTest, RoomsDoNotOverlap)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 30, 10, 2);
  this->teachers = generateTeachers(10);
  this->students = generateStudents(30, static_cast<unsigned short int>(this->teachers.size()));
  this->rooms = generateRooms(2);
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  scheduler.scheduleAll();

  for (size_t i = 0; i < scheduler.mAssignments.size(); ++i)
  {
    for (size_t j = i + 1; j < scheduler.mAssignments.size(); ++j)
    {
      const auto& a = scheduler.mAssignments[i];

      if (const auto& b = scheduler.mAssignments[j]; a.mDay == b.mDay && a.mRoomId == b.mRoomId)
      { ASSERT_FALSE(overlaps(a.mStartMinute, a.mStartMinute + a.mDuration, b.mStartMinute, b.mStartMinute + b.mDuration));}
    }
  }
}



// Limit values tests (samples relative to reality : small, medium, average, large, very large)


// ==========================
// TESTS Scheduler::outputJSONResult
// ==========================

TEST_F(SchedulerTest, OutputJSONIsValidAndComplete)
{
  Utils::setGlobalConfig(420, 750, 840, 1020, 60, 80, 60, 1200, 5, 4, 2);
  this->teachers = generateTeachers(4);
  this->students = generateStudents(5, static_cast<unsigned short int>(this->teachers.size()));
  this->rooms = generateRooms(2);
  Scheduler scheduler(this->students, this->teachers, this->rooms);

  scheduler.scheduleAll();

  auto jsonStr = scheduler.outputJSONResult();
  auto json = nlohmann::json::parse(jsonStr);

  ASSERT_TRUE(json.is_array());
  ASSERT_EQ(json.size(), this->students.size());

  for (const auto& obj : json)
  {
    ASSERT_TRUE(obj.contains("idUPPA"));
    ASSERT_TRUE(obj.contains("idLecteur"));
    ASSERT_TRUE(obj.contains("date"));
    ASSERT_TRUE(obj.contains("heureDebut"));
    ASSERT_TRUE(obj.contains("heureFin"));
    ASSERT_TRUE(obj.contains("nomSalle"));
  }
}
