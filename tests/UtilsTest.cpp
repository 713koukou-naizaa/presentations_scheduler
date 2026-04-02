#include "../include/Utils.h"

#include <gtest/gtest.h>


class UtilsTest : public testing::Test
{
protected:
    Utils::Interval sixtyToOneHundredTwentyInterval{60, 120};
    Utils::Interval OneHundredToOneHundredSixtyInterval{100, 160};
    Utils::Interval OneHundredTwentyToOneHundredEightyInterval{120, 180};
    Utils::Interval TwoHundredToTwoHundredSixtyInterval{200, 260};
};

// ==========================
// TESTS Interval::overlaps
// ==========================

TEST_F(UtilsTest, IntervalOverlaps_ShouldReturnTrueWhenIntervalsIntersect)
{
    // GIVEN
    constexpr bool expectedMethodResult = true;

    // WHEN
    const bool effectiveMethodResult = sixtyToOneHundredTwentyInterval.overlaps(OneHundredToOneHundredSixtyInterval);

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

TEST_F(UtilsTest, IntervalOverlaps_ShouldReturnFalseWhenIntervalsDoNotIntersect)
{
    // GIVEN
    constexpr bool expectedMethodResult = false;

    // WHEN
    const bool effectiveMethodResult = sixtyToOneHundredTwentyInterval.overlaps(TwoHundredToTwoHundredSixtyInterval);

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

TEST_F(UtilsTest, IntervalOverlaps_ShouldReturnFalseWhenIntervalsAreAdjacent)
{
    // GIVEN
    constexpr bool expectedMethodResult = false;

    // WHEN
    // sixtyToOneHundredTwentyInterval.mEnd == OneHundredTwentyToOneHundredEightyInterval.mStart
    const bool effectiveMethodResult = sixtyToOneHundredTwentyInterval.overlaps(OneHundredTwentyToOneHundredEightyInterval);

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

TEST_F(UtilsTest, IntervalOverlaps_ShouldReturnTrueWhenIntervalFullyContained)
{
    // GIVEN
    constexpr bool expectedMethodResult = true;
    constexpr Utils::Interval container{50, 200};
    constexpr Utils::Interval contained{60, 120};

    // WHEN
    const bool effectiveMethodResult = container.overlaps(contained);

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

// ==========================
// TESTS Interval::operator==
// ==========================

TEST_F(UtilsTest, IntervalEquality_ShouldReturnTrueWhenSame)
{
    // GIVEN
    constexpr bool expectedMethodResult = true;
    const Utils::Interval copy{sixtyToOneHundredTwentyInterval.mStart, sixtyToOneHundredTwentyInterval.mEnd};

    // WHEN
    const bool effectiveMethodResult = sixtyToOneHundredTwentyInterval == copy;

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

TEST_F(UtilsTest, IntervalEquality_ShouldReturnFalseWhenDifferent)
{
    // GIVEN
    constexpr bool expectedMethodResult = false;

    // WHEN
    const bool effectiveMethodResult = sixtyToOneHundredTwentyInterval == OneHundredToOneHundredSixtyInterval;

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

// ==========================
// TESTS Utils::minutesToHHMM
// ==========================

TEST_F(UtilsTest, MinutesToHHMM_ShouldReturnCorrectFormat)
{
    // GIVEN
    // 150 minutes = 2h30
    constexpr char expectedMethodResult[] = "02:30";

    // WHEN
    const std::string effectiveMethodResult = Utils::minutesToHHMM(150);

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

TEST_F(UtilsTest, MinutesToHHMM_ShouldReturnMidnightCorrectly)
{
    // GIVEN
    constexpr char expectedMethodResult[] = "00:00";

    // WHEN
    const std::string effectiveMethodResult = Utils::minutesToHHMM(0);

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}

TEST_F(UtilsTest, MinutesToHHMM_ShouldReturnHourOnlyCorrectly)
{
    // GIVEN
    constexpr char expectedMethodResult[] = "03:00";

    // WHEN
    const std::string effectiveMethodResult = Utils::minutesToHHMM(180);

    // THEN
    EXPECT_EQ(effectiveMethodResult, expectedMethodResult);
}