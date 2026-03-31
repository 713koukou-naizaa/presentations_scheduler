  #include "../include/Scheduler.h"

#include <gtest/gtest.h>

#include <vector>
#include <random>

#include "../include/Room.h"
#include "../include/Student.h"
#include "../include/Teacher.h"


class SchedulerTest : public testing::Test
{
protected:
    void SetUp() override
    {
      Utils::setGlobalConfig(
          0, 300, // morning
          300, 600, // afternoon
          60, 60, // durations
          0,
          300, // max teacher time
          1, 2, 1 // counts
      );
    }

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
      Teacher t(i, TestRNG::randBool(2));
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
      Student currentStudent(i, SchedulerTest::TestRNG::randBool(4), 0, SchedulerTest::TestRNG::randInt(0, std::max(0, pTeacherCount - 1)));
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
      Room currentRoom(i, string("Room_") + (i < 10 ? "0" + std::to_string(i) : std::to_string(i)));
      rooms.push_back(currentRoom);
    }

    return rooms;
  }

  static bool overlaps(const unsigned short int pStartA, const unsigned short int pEndA, const unsigned short int pStartB, const unsigned short int pEndB)
  { return pStartA < pEndB && pStartB < pEndA; }
};


// ==========================
// TESTS Scheduler::scheduleAll
// ==========================
TEST_F(SchedulerTest, ScheduleAll_ShouldScheduleAllStudents)
{
  // GIVEN
  Utils::setGlobalConfig(0, 300, 300, 600, 60, 60, 0, 1200, 10, 4, 2);

  const vector<Teacher> teachers = generateTeachers(4);
  const vector<Room> rooms = generateRooms(2);
  const vector<Student> students = generateStudents(10, static_cast<unsigned short>(teachers.size()));

  Scheduler scheduler(students, teachers, rooms);

  constexpr bool expectedScheduleAllResult = true;
  const unsigned short int expectedAssignmentsSize = students.size();

  // WHEN
  const bool effectiveScheduleAllResult = scheduler.scheduleAll();
  const unsigned short int effectiveAssignmentsSize = scheduler.mAssignments.size();

  // THEN
  EXPECT_EQ(effectiveScheduleAllResult, expectedScheduleAllResult);
  EXPECT_EQ(effectiveAssignmentsSize, expectedAssignmentsSize);
}

// ==========================
// TESTS Scheduler::trySchedulingUntilTimeWindowFull
// ==========================
TEST_F(SchedulerTest, TrySchedulingUntilTimeWindowFull_ShouldMoveToSameDayWhenDoesNotFit)
{
  // GIVEN
  Utils::setGlobalConfig(0, 30, 30, 60, 60, 60, 0, 1200, 1, 2, 1);

  const vector<Teacher> teachers = generateTeachers(2);
  const vector<Room> rooms = generateRooms(1);
  const vector<Student> students = generateStudents(1, 2);

  Scheduler scheduler(students, teachers, rooms);

  constexpr bool expectedMadeProgress = false;
  constexpr unsigned short int expectedToRetrySchedulingSameDaySize = 1u;

  std::vector<unsigned short> toScheduleStudents = {0};
  std::vector<unsigned short> toRetrySchedulingSameDay;
  std::vector<unsigned short> toRetrySchedulingNextWeekStudents;

  bool madeProgress = false;
  int startTime = 0;

  // WHEN
  scheduler.trySchedulingUntilTimeWindowFull(0, 0, startTime, 30, toRetrySchedulingSameDay, toScheduleStudents, toRetrySchedulingNextWeekStudents, madeProgress);
  const bool effectiveMadeProgress = madeProgress;
  const unsigned short int effectiveToRetrySchedulingSameDaySize = toRetrySchedulingSameDay.size();

  // THEN
  EXPECT_EQ(effectiveMadeProgress, expectedMadeProgress);
  EXPECT_EQ(effectiveToRetrySchedulingSameDaySize, expectedToRetrySchedulingSameDaySize);
}

TEST_F(SchedulerTest, TrySchedulingUntilTimeWindowFull_ShouldMoveToNextWeekWhenTeacherOverworked)
{
  // GIVEN
  vector<Teacher> teachers = generateTeachers(2);
  teachers[0].mWeeklyRemainingMinutes = 0; // force NEXT_WEEK

  const vector<Room> rooms = generateRooms(1);
  vector<Student> students = generateStudents(1, 2);
  students[0].mReferentTeacherId = teachers[0].mId;

  Scheduler scheduler(students, teachers, rooms);

  constexpr bool expectedMadeProgress = false;
  constexpr unsigned short int expectedToRetrySchedulingNextWeekStudentsSize = 1u;

  std::vector<unsigned short> toScheduleStudents = {0};
  std::vector<unsigned short> toRetrySchedulingSameDay;
  std::vector<unsigned short> toRetrySchedulingNextWeekStudents;

  bool madeProgress = false;
  int startTime = 0;

  // WHEN
  scheduler.trySchedulingUntilTimeWindowFull(0, 0, startTime, GLOBAL_CONFIG.END_MORNING_TIME, toRetrySchedulingSameDay, toScheduleStudents, toRetrySchedulingNextWeekStudents, madeProgress);
  const bool effectiveMadeProgress = madeProgress;
  const unsigned short int effectiveToRetrySchedulindNextWeekStudentsSize = toRetrySchedulingNextWeekStudents.size();

  // THEN
  EXPECT_EQ(effectiveMadeProgress, expectedMadeProgress);
  EXPECT_EQ(effectiveToRetrySchedulindNextWeekStudentsSize, expectedToRetrySchedulingNextWeekStudentsSize);
}

TEST_F(SchedulerTest, TrySchedulingUntilTimeWindowFull_ShouldStopWhenWindowFull)
{
  // GIVEN
  const vector<Teacher> teachers = generateTeachers(2);
  const vector<Room> rooms = generateRooms(1);
  const vector<Student> students = generateStudents(10, 2);

  Scheduler scheduler(students, teachers, rooms);

  constexpr unsigned short int expectedMaxScheduledStudents = 2u;

  std::vector<unsigned short> toScheduleStudents;
  for (size_t i = 0; i < students.size(); ++i) toScheduleStudents.push_back(i);

  std::vector<unsigned short> toRetrySchedulingSameDay;
  std::vector<unsigned short> toRetrySchedulingNextWeekStudents;

  bool madeProgress = false;
  int startTime = GLOBAL_CONFIG.START_MORNING_TIME;

  // WHEN
  // only 2 slots max
  scheduler.trySchedulingUntilTimeWindowFull(0, 0, startTime, GLOBAL_CONFIG.START_MORNING_TIME + 120, toRetrySchedulingSameDay, toScheduleStudents, toRetrySchedulingNextWeekStudents,  madeProgress);
  const unsigned short int effectiveMaxScheduledStudents = scheduler.mAssignments.size();

  // THEN
  EXPECT_LE(effectiveMaxScheduledStudents, expectedMaxScheduledStudents);
}

// ==========================
// TESTS Scheduler::tryScheduleStudentsAtSlotAtDayAtRoom
// ==========================
TEST_F(SchedulerTest, TrySchedule_ShouldReturnSameDayWhenSlotOutsideWindows)
{
  // GIVEN
  Student student(1, false, 60, 1);
  Teacher teacher1(1, true);
  Teacher teacher2(2, true);
  Room room(1, "A");

  Scheduler scheduler({student}, {teacher1, teacher2}, {room});
  constexpr Utils::Interval outsideWindowsSlot{700, 800}; // outside

  constexpr Scheduler::TrySchedulingResultOptions expectedTrySchedulingResult = Scheduler::SAME_DAY;

  // WHEN
  const Scheduler::TrySchedulingResultOptions effectiveTrySchedulingResult = scheduler.tryScheduleStudentAtSlotAtDayAtRoom(0, 0, outsideWindowsSlot, 0);

  // THEN
  EXPECT_EQ(effectiveTrySchedulingResult, expectedTrySchedulingResult);
}

TEST_F(SchedulerTest, TrySchedule_ShouldReturnNextWeekWhenReferentOverworked)
{
  // GIVEN
  Student student(1, false, 60, 1);
  Teacher teacher1(1, true);
  teacher1.mWeeklyRemainingMinutes = 30;

  Teacher teacher2(2, true);
  Room room(1, "A");

  Scheduler scheduler({student}, {teacher1, teacher2}, {room});
  constexpr Utils::Interval slot{0, 60};

  constexpr Scheduler::TrySchedulingResultOptions expectedTrySchedulingResult = Scheduler::NEXT_WEEK;

  // WHEN
  const Scheduler::TrySchedulingResultOptions effectiveTrySchedulingResult = scheduler.tryScheduleStudentAtSlotAtDayAtRoom(0, 0, slot, 0);

  // THEN
  EXPECT_EQ(effectiveTrySchedulingResult, expectedTrySchedulingResult);
}

TEST_F(SchedulerTest, TrySchedule_ShouldReturnSameDayWhenReferentBusy)
{
  // GIVEN
  Student student(1, false, 60, 1);
  Teacher teacher1(1, true);
  Teacher teacher2(2, true);

  teacher1.book(0, Utils::Interval{0, 60}); // already booked

  Room room(1, "A");
  Scheduler scheduler({student}, {teacher1, teacher2}, {room});

  constexpr Utils::Interval slot{0, 60};

  constexpr Scheduler::TrySchedulingResultOptions expectedTrySchedulingResult = Scheduler::SAME_DAY;

  // WHEN
  const Scheduler::TrySchedulingResultOptions effectiveTrySchedulingResult = scheduler.tryScheduleStudentAtSlotAtDayAtRoom(0, 0, slot, 0);

  // THEN
  EXPECT_EQ(effectiveTrySchedulingResult, expectedTrySchedulingResult);
}

TEST_F(SchedulerTest, TrySchedule_ShouldReturnNextWeek_WhenNoSecondTeacherAvailable)
{
  // GIVEN
  Student student(1, false, 60, 1);
  Teacher teacher1(1, false); // needs technical second
  Teacher teacher2(2, false); // not technical -> invalid

  Room room(1, "A");

  Scheduler scheduler({student}, {teacher1, teacher2}, {room});
  constexpr Utils::Interval slot{0, 60};

  constexpr Scheduler::TrySchedulingResultOptions expectedTrySchedulingResult = Scheduler::NEXT_WEEK;

  // WHEN
  const Scheduler::TrySchedulingResultOptions effectiveTrySchedulingResult = scheduler.tryScheduleStudentAtSlotAtDayAtRoom(0, 0, slot, 0);

  // THEN
  EXPECT_EQ(effectiveTrySchedulingResult, expectedTrySchedulingResult);
}

// ==========================
//TESTS Scheduler::removeStudentFromVector
// ==========================
TEST_F(SchedulerTest, RemoveStudentFromVector_ShouldRemoveCorrectly)
{
  // GIVEN
  std::vector<unsigned short int> vec = {1, 2, 3};
  constexpr unsigned short int expectedSize = 2u;
  constexpr unsigned short int expectedFirstValue = 1u;
  constexpr unsigned short int expectedSecondValue = 3u;

  // WHEN
  Scheduler::removeStudentFromVector(vec, 2);
  const unsigned short int effectiveSize = vec.size();
  const unsigned short int effectiveFirstValue = vec[0];
  const unsigned short int effectiveSecondValue = vec[1];

  // THEN
  EXPECT_EQ(effectiveSize, expectedSize);
  EXPECT_EQ(effectiveFirstValue, expectedFirstValue);
  EXPECT_EQ(effectiveSecondValue, expectedSecondValue);
}

// ==========================
//TESTS Scheduler::outputJSONResult
// ==========================
TEST_F(SchedulerTest, OutputJSONResult_ShouldReturnValidJson)
{
  // GIVEN
  Student student(1, false, 60, 1);
  Teacher teacher1(1, true);
  Teacher teacher2(2, true);
  Room room(1, "A");

  Scheduler scheduler({student}, {teacher1, teacher2}, {room});
  scheduler.tryScheduleStudentAtSlotAtDayAtRoom(0, 0, Utils::Interval{0, 60}, 0);

  // WHEN
  const std::string JSONResult = scheduler.outputJSONResult();

  // THEN
  EXPECT_FALSE(JSONResult.empty());
  EXPECT_NE(JSONResult.find("\"idUPPA\""), std::string::npos);
  EXPECT_NE(JSONResult.find("\"nomSalle\""), std::string::npos);
}