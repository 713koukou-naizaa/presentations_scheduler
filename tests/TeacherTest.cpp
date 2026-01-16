#include "Teacher.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <string>

#include "config.h"

#include "Utils.h"

using std::string;


class TeacherTest : public testing::Test
{
protected:
  Teacher teacher;

  void SetUp() override
  {
    GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME=180;

    teacher.mId = 1;
    teacher.mName = "Test Teacher";
    teacher.mIsTechnical = true;
    teacher.mWeeklyRemainingMinutes = GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME;
    teacher.mBusyByDay.clear();
  }
};

// ==========================
// TESTS Teacher::isAvailable
// ==========================

TEST_F(TeacherTest, IsAvailableWhenFreeAndSufficientTime)
{
  Utils::Interval slot{30, 60};

  EXPECT_TRUE(teacher.isAvailable(0, slot));
}

TEST_F(TeacherTest, IsNotAvailableWhenInsufficientWeeklyMinutes)
{
  teacher.mWeeklyRemainingMinutes = 20;

  Utils::Interval slot{0, 30};

  EXPECT_FALSE(teacher.isAvailable(0, slot));
}

TEST_F(TeacherTest, IsNotAvailableWhenOverlapsExistingBooking)
{
  Utils::Interval bookedSlot{10, 40};

  teacher.book(0, bookedSlot);

  Utils::Interval overlappingSlot{30, 50};
  EXPECT_FALSE(teacher.isAvailable(0, overlappingSlot));
}

TEST_F(TeacherTest, IsAvailableWhenNoOverlap)
{
  teacher.book(0, Utils::Interval{10, 20});

  Utils::Interval notOverlappingSlot{20, 30};

  EXPECT_TRUE(teacher.isAvailable(0, notOverlappingSlot));
}

// ==========================
// TESTS Teacher::book
// ==========================

TEST_F(TeacherTest, BookReducesWeeklyMinutes)
{
  Utils::Interval slot{30, 60};
  unsigned short before = teacher.mWeeklyRemainingMinutes;

  teacher.book(0, slot);

  EXPECT_EQ(teacher.mWeeklyRemainingMinutes, before - 30);
}

TEST_F(TeacherTest, BookStoresIntervalInCorrectDay)
{
  Utils::Interval slot{15, 45};

  teacher.book(2, slot);

  ASSERT_EQ(teacher.mBusyByDay.size(), 3u);
  ASSERT_EQ(teacher.mBusyByDay[2].size(), 1u);
  EXPECT_EQ(teacher.mBusyByDay[2][0].mStart, 15u);
  EXPECT_EQ(teacher.mBusyByDay[2][0].mEnd, 45u);
}