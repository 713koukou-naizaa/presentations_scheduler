#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

struct Teacher;
struct Room;
struct Student;

#include "json.hpp"

using std::string;
using std::vector;

/**
 * @brief Utility class
 */
struct Utils
{
    /**
     * @brief Sets the global configuration variable values
     *
     * @param pStartMorningTime
     * @param pEndMorningTime
     * @param pStartAfternoonTime
     * @param pEndAfternoonTime
     * @param pNormalPresentationLength
     * @param pAccommodatedPresentationLength
     * @param pInBetweenBreakLength
     * @param pMaxTeachersWeeklyWorkedTime
     * @param pNbStudents
     * @param pNbTeachers
     * @param pNbRooms
     *
     * @return void
     */
    static void setGlobalConfig(unsigned short int pStartMorningTime, unsigned short int pEndMorningTime, unsigned short int pStartAfternoonTime, unsigned short int pEndAfternoonTime, unsigned short int pNormalPresentationLength, unsigned short int pAccommodatedPresentationLength, unsigned short int pInBetweenBreakLength, unsigned short int pMaxTeachersWeeklyWorkedTime, unsigned short int pNbStudents, unsigned short int pNbTeachers, unsigned short int pNbRooms);

    /**
     * @brief Displays the content of the given vectors of students, teachers and rooms (debugging)
     *
     * @param pStudents
     * @param pTeachers
     * @param pRooms
     *
     * @return void
     */
    static void displayVectors(const vector<Student> &pStudents, const vector<Teacher> &pTeachers, const vector<Room> &pRooms);

    /**
     * @brief Reads the next JSON object from stdin (next line)
     *
     * @return nlohmann::json The line as a JSON object
     */
    static nlohmann::json readNextJsonFromStdin();

    /**
     * @brief Loads students from stdin, by reading the next JSON array from stdin and converting it to a vector of Student objects
     *
     * @return vector<Student>
     */
    static vector<Student> loadStudentsFromStdin();

    /**
     * @brief Loads teachers from stdin, by reading the next JSON array from stdin and converting it to a vector of Teacher objects
     *
     * @return vector<Teacher>
     */
    static vector<Teacher> loadTeachersFromStdin();

    /**
     * @brief Loads rooms from stdin, by reading the next JSON array from stdin and converting it to a vector of Room objects
     *
     * @return vector<Room>
     */
    static vector<Room> loadRoomsFromStdin();

    /**
     * @brief Converts a number of minutes from midnight to a string in the format "HH:MM"
     *
     * @param pMinutes
     *
     * @return string
     */
    static string minutesToHHMM(unsigned short int pMinutes);

    /**
     * @brief Finds the vector index of the object with the given ID in the given vector
     *
     * @tparam T Element type of the vector (Student or Teacher)
     * @param pVector
     * @param pId
     *
     * @return int Vector index of the object with the given ID, or -1 if not found
     */
    template<typename T> static int findIndexById(const vector<T> &pVector, unsigned int pId)
    {
        for (size_t i = 0; i < pVector.size(); i++)
            if (pVector[i].mId == pId) return static_cast<int>(i);

        return -1;
    }

    /**
     * @brief Prints the global configuration parameters
     *
     * @return void
     */
    static void printParameters();

    /**
     * @brief Represents an interval of time
     */
    struct Interval
    {
        unsigned short int mStart; /// Inclusive minutes from midnight
        unsigned short int mEnd; /// Exclusive minutes from midnight

        /**
         * @brief Checks if the intervals overlap
         *
         * @param pOther
         *
         * @return bool True if the intervals overlap, false otherwise
         */
        [[nodiscard]] bool overlaps(const Interval &pOther) const;
    };
};

#endif // UTILS_H