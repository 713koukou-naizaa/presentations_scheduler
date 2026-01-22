cmake_minimum_required(VERSION 3.16)
project(presentations_scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Main library ----
add_library(core
        src/config.cpp
        src/Room.cpp
        src/Scheduler.cpp
        src/Teacher.cpp
        src/Utils.cpp
)

target_include_directories(core PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# ---- GoogleTest ----
find_package(GTest REQUIRED)

# ---- Tests ----
add_executable(tests
        tests/SchedulerTest.cpp
        tests/TeacherTest.cpp
        tests/UtilsTest.cpp
)

target_link_libraries(tests
        PRIVATE
        core
        GTest::gtest
        GTest::gtest_main
)

enable_testing()
add_test(NAME SchedulerTests COMMAND tests)
add_test(NAME TeacherTests COMMAND tests)
add_test(NAME UtilsTests COMMAND tests)
