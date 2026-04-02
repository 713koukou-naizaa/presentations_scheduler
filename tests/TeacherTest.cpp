#include "../include/Teacher.h"

#include <gtest/gtest.h>


class TeacherTest : public testing::Test
{
protected:
    static constexpr unsigned short int mDayNumberZero = 0;
    
    Teacher mMockTechnicalTeacher{1, true};

    Utils::Interval mOneHourSlotOne{60, 120}; // 1h
    Utils::Interval mSlotOverlappingWithOneHourSlotOne{100, 160}; // overlaps with slot1
    Utils::Interval mSlotAdjacentNotOverlappingWithOneHourSlotOneStart{120, 180}; // adjacent, does not overlap (inclusive start, exclusive end)
    Utils::Interval mNoOverlapSlot{200, 260}; // no overlap

    // 300 minutes = 5 hours
    void SetUp() override { this->mMockTechnicalTeacher.mWeeklyRemainingMinutes = 300; }
};


// ==========================
// TESTS Teacher::isAvailable
// ==========================

TEST_F(TeacherTest, IsAvailable_ShouldReturnTrueWhenEnoughTimeAndNoBookings)
{
    // GIVEN
    constexpr bool expectedAvailableResult = true;

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, IsAvailable_ShouldReturnFalseWhenNotEnoughWeeklyRemainingMinutesLeft)
{
    // GIVEN
    constexpr bool expectedAvailableResult = false;
    this->mMockTechnicalTeacher.mWeeklyRemainingMinutes = 30; // less than slot duration (60)

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, IsAvailable_ShouldReturnFalsewhenExactSameIntervalOverlaps)
{
    // GIVEN
    constexpr bool expectedAvailableResult = false;
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, IsAvailable_ShouldReturnFalseWhenContainedIntervalOverlaps)
{
    // GIVEN
    constexpr bool expectedAvailableResult = false;
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // WHEN
    // contained slot
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, Utils::Interval{60, 180});

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, IsAvailable_ShouldReturnFalseWhenOverlappingBooking)
{
    // GIVEN
    constexpr bool expectedAvailableResult = false;
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mSlotOverlappingWithOneHourSlotOne);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, IsAvailable_ShouldReturnTrueWhenAdjacentIntervalsDoNotOverlap)
{
    // GIVEN
    constexpr bool expectedAvailableResult = true;
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mSlotAdjacentNotOverlappingWithOneHourSlotOneStart);

    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, IsAvailable_ShouldReturnTrueWhenNonOverlappingBooking)
{
    // GIVEN
    constexpr bool expectedAvailableResult = true;
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mNoOverlapSlot);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, IsAvailable_ShouldReturnTrueWhenNewDayAutoResize)
{
    // GIVEN
    constexpr bool expectedAvailableResult = true;
    constexpr unsigned short int newDayNumber = 1; // new day

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(newDayNumber, this->mOneHourSlotOne);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}


// ==========================
// TESTS Teacher::book
// ==========================

TEST_F(TeacherTest, Book_ShouldAddSlotAndReduceWeeklyRemainingMinutes)
{
    // GIVEN
    constexpr unsigned short int expectedWeeklyRemainingMinutes = 240;

    // WHEN
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);
    const unsigned short int effectiveWeeklyRemainingMinutes = this->mMockTechnicalTeacher.mWeeklyRemainingMinutes;

    // THEN
    EXPECT_EQ(effectiveWeeklyRemainingMinutes, expectedWeeklyRemainingMinutes);
    EXPECT_EQ(this->mMockTechnicalTeacher.mBusyByDay[TeacherTest::mDayNumberZero].back(), this->mOneHourSlotOne);
}

TEST_F(TeacherTest, Book_ShouldRenderTeacherUnavailable)
{
    // GIVEN
    constexpr bool expectedAvailableResult = false;
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mSlotOverlappingWithOneHourSlotOne);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}

TEST_F(TeacherTest, Book_ShouldAllowMultipleNonOverlappingBookings)
{
    // GIVEN
    constexpr bool expectedSlotOverlappingWithOneHourSlotOneAvailableResult = false;
    constexpr bool expectedSlotOverlappingWithNoOverlapSlotAvailableResult = false;

    // WHEN
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);
    // overlaps this->mOneHourSlotOne
    const bool effectiveSlotOverlappingWithOneHourSlotOneAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mSlotOverlappingWithOneHourSlotOne);
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mNoOverlapSlot);
    // overlaps this->mSlotOverlappingWithNoOverlapSlot
    const bool effectiveSlotOverlappingWithNoOverlapSlotAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, Utils::Interval{210, 250});

    // THEN
    EXPECT_EQ(effectiveSlotOverlappingWithOneHourSlotOneAvailableResult, expectedSlotOverlappingWithOneHourSlotOneAvailableResult);
    EXPECT_EQ(effectiveSlotOverlappingWithNoOverlapSlotAvailableResult, expectedSlotOverlappingWithNoOverlapSlotAvailableResult);
}

TEST_F(TeacherTest, Book_ShouldNotAffectOtherDays)
{
    // GIVEN
    constexpr bool expectedSlotOverlappingWithOneHourSlotOneAvailableResult = false;
    constexpr bool expectedOneHourSlotOneAvailableResult = true;
    constexpr unsigned short int unaffectedDayNumber = 3; // unaffected day
    this->mMockTechnicalTeacher.book(3, this->mOneHourSlotOne);

    // WHEN
    const bool effectiveOneHourSlotOneAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mOneHourSlotOne);
    const bool effectiveSlotOverlappingWithOneHourSlotOne = this->mMockTechnicalTeacher.isAvailable(unaffectedDayNumber, this->mSlotOverlappingWithOneHourSlotOne);

    // THEN
    EXPECT_EQ(effectiveOneHourSlotOneAvailableResult, expectedOneHourSlotOneAvailableResult);
    EXPECT_EQ(effectiveSlotOverlappingWithOneHourSlotOne, expectedSlotOverlappingWithOneHourSlotOneAvailableResult);
}

TEST_F(TeacherTest, Book_ShouldConsumeAllWeeklyRemainingMinutes)
{
    // GIVEN
    constexpr bool expectedAvailableResult = false;
    this->mMockTechnicalTeacher.mWeeklyRemainingMinutes = 60;
    this->mMockTechnicalTeacher.book(TeacherTest::mDayNumberZero, this->mOneHourSlotOne); // no time left after booking

    // WHEN
    const bool effectiveAvailableResult = this->mMockTechnicalTeacher.isAvailable(TeacherTest::mDayNumberZero, this->mNoOverlapSlot);

    // THEN
    EXPECT_EQ(effectiveAvailableResult, expectedAvailableResult);
}