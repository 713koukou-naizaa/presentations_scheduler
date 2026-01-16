#include "Utils.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "config.h"

#include "Room.h"
#include "Student.h"
#include "Teacher.h"

using std::string;


class UtilsTest : public testing::Test
{
protected:
    std::streambuf *origCin = nullptr;
    std::istringstream inputBuffer;

    void SetUp() override
    {
        // Save original cin
        origCin = std::cin.rdbuf();
        GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH=60;
        GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH=80;
    }

    void TearDown() override
    {
        // Restore cin
        std::cin.rdbuf(origCin);
    }

    void setCinInput(const std::string& input)
    {
        inputBuffer.str(input);
        inputBuffer.clear();
        std::cin.rdbuf(inputBuffer.rdbuf());
    }
};

// ==========================
// TESTS Utils::readNextJsonFromStdin
// ==========================

TEST_F(UtilsTest, ParsesJsonCorrectly)
{
    setCinInput("{\"key\":\"value\"}\n");

    nlohmann::json result = Utils::readNextJsonFromStdin();

    EXPECT_EQ(result["key"], "value");
}

TEST_F(UtilsTest, SkipsWhitespacesAndEmptyLines)
{
    setCinInput("\n   \t \n{\"a\":1}\n");

    nlohmann::json result = Utils::readNextJsonFromStdin();

    EXPECT_EQ(result["a"], 1);
}

TEST_F(UtilsTest, ThrowsParseErrorOnInvalidJson)
{
    setCinInput("not_a_json\n");

    EXPECT_THROW({Utils::readNextJsonFromStdin();}, nlohmann::json::parse_error);
}

TEST_F(UtilsTest, ThrowsRuntimeErrorOnEmptyInput)
{
    setCinInput("");

    EXPECT_THROW({Utils::readNextJsonFromStdin();}, std::runtime_error);
}

TEST_F(UtilsTest, ThrowsRuntimeErrorOnWhitespacesLines)
{
    setCinInput("\n   \n\t\n");

    EXPECT_THROW({Utils::readNextJsonFromStdin();}, std::runtime_error);
}

// ==========================
// TESTS Utils::loadStudents/Teachers/RoomsFromStdin
// ==========================

TEST_F(UtilsTest, LoadsStudentsCorrectly)
{
    setCinInput(R"([{"id":1,"name":"Alice","hasAccommodations":true,"referentTeacherId":100},{"id":2,"name":"Bob","hasAccommodations":false,"referentTeacherId":101}])");

    auto students = Utils::loadStudentsFromStdin();

    ASSERT_EQ(students.size(), 2u);
    EXPECT_EQ(students[0].mId, 1u);
    EXPECT_EQ(students[0].mName, "Alice");
    EXPECT_TRUE(students[0].mHasAccommodations);
    EXPECT_EQ(students[0].mEffectivePresentationLength, GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH);
    EXPECT_EQ(students[0].mReferentTeacherId, 100u);

    EXPECT_EQ(students[1].mId, 2u);
    EXPECT_EQ(students[1].mName, "Bob");
    EXPECT_FALSE(students[1].mHasAccommodations);
    EXPECT_EQ(students[1].mEffectivePresentationLength, GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH);
    EXPECT_EQ(students[1].mReferentTeacherId, 101u);
}

TEST_F(UtilsTest, LoadsTeachersCorrectly)
{
    setCinInput(R"([{"id":10,"name":"Mr. Smith","isTechnical":true},{"id":11,"name":"Ms. Johnson","isTechnical":false}])");

    auto teachers = Utils::loadTeachersFromStdin();

    ASSERT_EQ(teachers.size(), 2u);
    EXPECT_EQ(teachers[0].mId, 10u);
    EXPECT_EQ(teachers[0].mName, "Mr. Smith");
    EXPECT_TRUE(teachers[0].mIsTechnical);

    EXPECT_EQ(teachers[1].mId, 11u);
    EXPECT_EQ(teachers[1].mName, "Ms. Johnson");
    EXPECT_FALSE(teachers[1].mIsTechnical);
}

TEST_F(UtilsTest, LoadsRoomsCorrectly)
{
    setCinInput(R"([{"id":101,"tag":"A1"},{"id":102,"tag":"B2"}])");

    auto rooms = Utils::loadRoomsFromStdin();

    ASSERT_EQ(rooms.size(), 2u);
    EXPECT_EQ(rooms[0].mId, 101u);
    EXPECT_EQ(rooms[0].mTag, "A1");

    EXPECT_EQ(rooms[1].mId, 102u);
    EXPECT_EQ(rooms[1].mTag, "B2");
}

TEST_F(UtilsTest, ThrowsRuntimeErrorOnEmptyInputForStudents)
{
    setCinInput("");

    EXPECT_THROW(Utils::loadStudentsFromStdin(), std::runtime_error);
}

TEST_F(UtilsTest, ThrowsParseErrorOnInvalidJsonForTeachers)
{
    setCinInput("invalid_json");

    EXPECT_THROW(Utils::loadTeachersFromStdin(), nlohmann::json::parse_error);
}

TEST_F(UtilsTest, ThrowsRuntimeErrorOnWhistespacesInputForRooms)
{
    setCinInput("\n  \t\n");

    EXPECT_THROW(Utils::loadRoomsFromStdin(), std::runtime_error);
}

// ==========================
// TESTS Utils::minutesToHHMM
// ==========================

TEST_F(UtilsTest, ZeroMinutes) { EXPECT_EQ(Utils::minutesToHHMM(0), "00:00"); }

TEST_F(UtilsTest, ExactHour) { EXPECT_EQ(Utils::minutesToHHMM(60), "01:00"); }

TEST_F(UtilsTest, HoursAndMinutes) { EXPECT_EQ(Utils::minutesToHHMM(125), "02:05"); }

TEST_F(UtilsTest, LeadingZeros) { EXPECT_EQ(Utils::minutesToHHMM(9), "00:09"); }

// ==========================
// TESTS Utils::Interval::overlaps
// ==========================

TEST_F(UtilsTest, PartialOverlap)
{
    Utils::Interval a{10, 20};
    Utils::Interval b{15, 25};

    EXPECT_TRUE(a.overlaps(b));
}

TEST_F(UtilsTest, NoOverlapTouchingBoundary)
{
    Utils::Interval a{10, 20};
    Utils::Interval b{20, 30};

    EXPECT_FALSE(a.overlaps(b));
}

TEST_F(UtilsTest, OneContainedInAnother)
{
    Utils::Interval a{10, 30};
    Utils::Interval b{15, 20};

    EXPECT_TRUE(a.overlaps(b));
}

TEST_F(UtilsTest, CompletelySeparate)
{
    Utils::Interval a{10, 15};
    Utils::Interval b{20, 25};

    EXPECT_FALSE(a.overlaps(b));
}
