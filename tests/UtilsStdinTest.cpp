#include "../include/Utils.h"

#include <gtest/gtest.h>

#include <sstream>

#include "../include/config.h"
#include "../include/Student.h"
#include "../include/Teacher.h"
#include "../include/Room.h"


class UtilsStdinTest : public testing::Test
{
protected:
    std::streambuf *origCin = nullptr;
    std::istringstream testInput;

    void SetUp() override
    {
        // Backup original std::cin buffer
        origCin = std::cin.rdbuf();
    }

    void TearDown() override
    {
        // Restore original std::cin buffer
        std::cin.rdbuf(origCin);
    }

    void setCinInput(const std::string& input)
    {
        testInput.str(input);
        testInput.clear();
        std::cin.rdbuf(testInput.rdbuf());
    }
};

// ==========================
// TESTS readNextJsonFromStdin
// ==========================

TEST_F(UtilsStdinTest, ReadNextJsonFromStdin_ShouldReturnJsonObject)
{
    // GIVEN
    const std::string jsonStr = R"({"id":1,"name":"Test"})";
    const nlohmann::json expectedReadNextJsonFromStdinResult = nlohmann::json::parse(jsonStr);
    setCinInput(jsonStr);

    // WHEN
    const nlohmann::json effectiveReadNextJsonFromStdinResult = Utils::readNextJsonFromStdin();

    // THEN
    EXPECT_EQ(effectiveReadNextJsonFromStdinResult, expectedReadNextJsonFromStdinResult);
}

TEST_F(UtilsStdinTest, ReadNextJsonFromStdin_ShouldSkipEmptyLines)
{
    // GIVEN
    const std::string jsonStr = "\n\n  \n{\"id\":2}";
    setCinInput(jsonStr);
    const nlohmann::json expectedReadNextJsonFromStdinResult = nlohmann::json::parse("{\"id\":2}");

    // WHEN
    const nlohmann::json effectiveReadNextJsonFromStdinResult = Utils::readNextJsonFromStdin();

    // THEN
    EXPECT_EQ(effectiveReadNextJsonFromStdinResult, expectedReadNextJsonFromStdinResult);
}

// ==========================
// TESTS loadStudentsFromStdin
// ==========================

TEST_F(UtilsStdinTest, LoadStudentsFromStdin_ShouldReturnCorrectVector)
{
    // GIVEN
    // set ACCOMMODATED/NORMAL length
    Utils::setGlobalConfig(0,0,0,0,30,45,0,300,1,1,1);
    const std::string input = R"([{"id":1,"hasAccommodations":true,"referentTeacherId":5},{"id":2,"hasAccommodations":false,"referentTeacherId":6}])";
    setCinInput(input);

    // WHEN
    const std::vector<Student> effectiveLoadStudentsFromStdinResult = Utils::loadStudentsFromStdin();

    // THEN
    EXPECT_EQ(effectiveLoadStudentsFromStdinResult.size(), 2u);
    EXPECT_EQ(effectiveLoadStudentsFromStdinResult[0].mId, 1u);
    EXPECT_EQ(effectiveLoadStudentsFromStdinResult[0].mEffectivePresentationLength, GLOBAL_CONFIG.ACCOMMODATED_PRESENTATION_LENGTH);
    EXPECT_EQ(effectiveLoadStudentsFromStdinResult[0].mReferentTeacherId, 5u);

    EXPECT_EQ(effectiveLoadStudentsFromStdinResult[1].mId, 2u);
    EXPECT_EQ(effectiveLoadStudentsFromStdinResult[1].mEffectivePresentationLength, GLOBAL_CONFIG.NORMAL_PRESENTATION_LENGTH);
    EXPECT_EQ(effectiveLoadStudentsFromStdinResult[1].mReferentTeacherId, 6u);
}

// ==========================
// TESTS loadTeachersFromStdin
// ==========================

TEST_F(UtilsStdinTest, LoadTeachersFromStdin_ShouldReturnCorrectVector)
{
    // GIVEN
    const std::string input = R"([{"id":10,"isTechnical":true},{"id":11,"isTechnical":false}])";
    setCinInput(input);

    // WHEN
    const std::vector<Teacher> effectiveLoadTeachersFromStdinResult = Utils::loadTeachersFromStdin();

    // THEN
    EXPECT_EQ(effectiveLoadTeachersFromStdinResult.size(), 2u);
    EXPECT_EQ(effectiveLoadTeachersFromStdinResult[0].mId, 10u);
    EXPECT_TRUE(effectiveLoadTeachersFromStdinResult[0].mIsTechnical);
    EXPECT_EQ(effectiveLoadTeachersFromStdinResult[1].mId, 11u);
    EXPECT_FALSE(effectiveLoadTeachersFromStdinResult[1].mIsTechnical);
}

// ==========================
// TESTS loadRoomsFromStdin
// ==========================

TEST_F(UtilsStdinTest, LoadRoomsFromStdin_ShouldReturnCorrectVector)
{
    // GIVEN
    const std::string input = R"([{"id":101,"tag":"A1"},{"id":102,"tag":"B2"}])";
    setCinInput(input);

    // WHEN
    const std::vector<Room> effectiveLoadRoomsFromStdinResult = Utils::loadRoomsFromStdin();

    // THEN
    EXPECT_EQ(effectiveLoadRoomsFromStdinResult.size(), 2u);
    EXPECT_EQ(effectiveLoadRoomsFromStdinResult[0].mId, 101u);
    EXPECT_EQ(effectiveLoadRoomsFromStdinResult[0].mTag, "A1");
    EXPECT_EQ(effectiveLoadRoomsFromStdinResult[1].mId, 102u);
    EXPECT_EQ(effectiveLoadRoomsFromStdinResult[1].mTag, "B2");
}