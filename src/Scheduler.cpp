#include "Scheduler.h"

#include <bits/stdc++.h>
#include <iostream>
#include <stdexcept>

using namespace std;

using std::cout;
using std::endl;

// Helper to find index in vector<T> where item.mId == id, return -1 if not found
template<typename T>
static int findIndexById(const vector<T> &pVector, unsigned int pId)
{
    for (size_t i = 0; i < pVector.size(); i++)
        if (pVector[i].mId == pId) return static_cast<int>(i);

    return -1;
}

Scheduler::Scheduler(vector<Student> pStudents, vector<Teacher> pTeachers, vector<Room> pRooms) : mStudents(std::move(pStudents)), mTeachers(std::move(pTeachers)), mRooms(std::move(pRooms)) {}

// Ensure internal per-day containers are big enough
void Scheduler::ensureDayCapacity(const unsigned short int pDay) {
    for(auto &currentRoom : mRooms)
    {
        if (static_cast<unsigned short int>(currentRoom.mMorningPointerByDay.size()) <= pDay)
        {
            currentRoom.mMorningPointerByDay.resize(pDay+1, GLOBAL_CONFIG.START_MORNING_TIME);
            currentRoom.mAfternoonPointerByDay.resize(pDay+1, GLOBAL_CONFIG.START_AFTERNOON_TIME);
        }
    }

    for(auto &currentTeacher : this->mTeachers) { if (static_cast<unsigned short int>(currentTeacher.mBusyByDay.size()) <= pDay) currentTeacher.mBusyByDay.resize(pDay+1); }
}

// Try to schedule all students with greedy earliest-fit, it creates additional days if needed.
bool Scheduler::scheduleAll()
{
    constexpr unsigned short int day = 0;
    ensureDayCapacity(day);

    // Order students sorted by accommodated length descending (longer first helps greedy, and sort helps algorithm)
    vector<unsigned short int> order(this->mStudents.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](const unsigned int studentIdOne, const unsigned int studentIdTwo) {
            return this->mStudents[studentIdOne].mEffectivePresentationLength > this->mStudents[studentIdTwo].mEffectivePresentationLength;
        }
    );

    for(unsigned int currentStudentIdIndex=0; currentStudentIdIndex<static_cast<unsigned short int>(order.size()); ++currentStudentIdIndex)
    {
        const unsigned int studentIndex = order[currentStudentIdIndex];
        const Student &currentStudent = this->mStudents[studentIndex];

        // Attempt scheduling, extend days if necessary
        bool isCurrentStudentPlaced = false;
        unsigned short int tryDay = 0;
        while (!isCurrentStudentPlaced)
        {
            ensureDayCapacity(tryDay);

            // For each room try morning first then afternoon
            for(unsigned short int currentRoomId=0; currentRoomId<static_cast<unsigned short int>(mRooms.size()) && !isCurrentStudentPlaced; ++currentRoomId)
            {
                Room &room = mRooms[currentRoomId];

                const unsigned short int duration = currentStudent.mEffectivePresentationLength;

                // If the current end time is before the end of morning, try the slot at room.mMorningPointerByDay[tryDay] (morning pointer)
                unsigned short int start = room.mMorningPointerByDay[tryDay];
                unsigned short int end = start + duration;
                if (end <= GLOBAL_CONFIG.END_MORNING_TIME)
                {
                    if (Utils::Interval currentSlot{start, end}; canPlace(studentIndex, tryDay, currentSlot))
                    {
                        place(studentIndex, tryDay, currentSlot, currentRoomId);
                        isCurrentStudentPlaced = true;
                        break;
                    }
                }
                // Else try afternoon pointer
                start = room.mAfternoonPointerByDay[tryDay];
                end = start + duration;
                if (end <= GLOBAL_CONFIG.END_AFTERNOON_TIME && !isCurrentStudentPlaced)
                {
                    if (Utils::Interval currentSlot{start, end}; canPlace(studentIndex, tryDay, currentSlot))
                    {
                        place(studentIndex, tryDay, currentSlot, currentRoomId);
                        isCurrentStudentPlaced = true;
                        break;
                    }
                }
            }

            // If current student not placed during current day, try the next day
            if (!isCurrentStudentPlaced)
            {
                ++tryDay;
                ensureDayCapacity(tryDay);
            }
        }
    }
    return true;
}

// Check participants availability and constraints for the given student, day, slot, room
bool Scheduler::canPlace(const unsigned int pStudentIndex, const unsigned short int pDay, const Utils::Interval &pSlot)
{
    // Find referent teacher index from the referent teacher external id
    const unsigned short int referentExternalId = this->mStudents[pStudentIndex].mReferentTeacherId;
    const int referentIndex = findIndexById(this->mTeachers, referentExternalId);
    if (referentIndex < 0) return false;
    Teacher &studentReferentTeacher = this->mTeachers[static_cast<size_t>(referentIndex)];
    if (!studentReferentTeacher.isAvailable(pDay, pSlot)) return false;

    // Find a second teacher (teacher != referent) who is available and respects the "technical" constraint
    const bool needTechnicalSecond = !studentReferentTeacher.mIsTechnical;
    bool foundSecond = false;
    for(unsigned short int currentTeacherId=0; currentTeacherId<static_cast<unsigned short int>(this->mTeachers.size()); ++currentTeacherId)
    {
        if (static_cast<int>(currentTeacherId) == referentIndex) continue;
        if (needTechnicalSecond && !this->mTeachers[currentTeacherId].mIsTechnical) continue;
        if (!this->mTeachers[currentTeacherId].isAvailable(pDay, pSlot)) continue;
        foundSecond = true;
        break;
    }

    if (!foundSecond) return false;

    // Check room per-day pointer doesn't overlap other room constraint implicitly handled by pointer advance
    // But need also to ensure no other presentation in that room overlaps slot: since we always schedule at room pointer and advance, this is satisfied

    return true;
}

// Place the presentation: pick a specific second teacher (random among valid), book everything, advance room pointer
void Scheduler::place(const unsigned int pStudentIndex, const unsigned short int pDay, const Utils::Interval &pSlot, const unsigned short int pRoomId)
{
    const unsigned short int referentExternalId = this->mStudents[pStudentIndex].mReferentTeacherId;
    const int referentIndex = findIndexById(this->mTeachers, referentExternalId);
    if (referentIndex < 0) throw runtime_error("Referent teacher not found in place()");

    Teacher &studentReferentTeacher = this->mTeachers[static_cast<size_t>(referentIndex)];

    // Select second teacher candidates (use teacher indices)
    vector<unsigned short int> secondTeacherCandidates;
    const bool needTechnicalSecond = !studentReferentTeacher.mIsTechnical;
    for(unsigned short int currentTeacherId=0; currentTeacherId<static_cast<unsigned short int>(this->mTeachers.size()); ++currentTeacherId)
    {
        if (static_cast<int>(currentTeacherId) == referentIndex) continue;
        if (needTechnicalSecond && !this->mTeachers[currentTeacherId].mIsTechnical) continue;
        if (this->mTeachers[currentTeacherId].isAvailable(pDay, pSlot)) secondTeacherCandidates.push_back(currentTeacherId);
    }

    // If no candidates were found
    if (secondTeacherCandidates.empty())
    {
        // should not happen because canPlace checked, but guard
        throw runtime_error("No second teacher found in place()");
    }

    // Random pick amongst candidates (use seed (i.e. 123456) for reproducibility)
    static std::mt19937 rng(std::random_device{}());
    shuffle(secondTeacherCandidates.begin(), secondTeacherCandidates.end(), rng);
    const unsigned short int secondTeacherIndex = secondTeacherCandidates[0];

    // Book referent, second teacher
    studentReferentTeacher.book(pDay, pSlot);
    this->mTeachers[secondTeacherIndex].book(pDay, pSlot);

    // Record assignment as presentation (store vector indices for student and teachers)
    Presentation finalPresentation{};
    finalPresentation.mStudentId = pStudentIndex; // store student vector index
    finalPresentation.mDay = pDay;
    finalPresentation.mRoomId = pRoomId;
    finalPresentation.mStartMinute = pSlot.mStart;
    finalPresentation.mDuration = pSlot.mEnd - pSlot.mStart;
    finalPresentation.mReferentTeacherId = static_cast<unsigned short int>(referentIndex); // store teacher vector index
    finalPresentation.mSecondTeacherId = secondTeacherIndex; // teacher vector index
    this->mAssignments.push_back(finalPresentation);

    // Advance room pointer for that day: add presentation time + break
    ensureDayCapacity(pDay);
    // Decide whether we used morning or afternoon pointer
    if (Room &usedRoom = mRooms[pRoomId]; pSlot.mStart >= GLOBAL_CONFIG.START_MORNING_TIME && pSlot.mEnd <= GLOBAL_CONFIG.END_MORNING_TIME && pSlot.mStart == usedRoom.mMorningPointerByDay[pDay])
    { usedRoom.mMorningPointerByDay[pDay] = pSlot.mEnd + this->mBreakLength; }
    else if (pSlot.mStart >= GLOBAL_CONFIG.START_AFTERNOON_TIME && pSlot.mEnd <= GLOBAL_CONFIG.END_AFTERNOON_TIME && pSlot.mStart == usedRoom.mAfternoonPointerByDay[pDay])
    { usedRoom.mAfternoonPointerByDay[pDay] = pSlot.mEnd + this->mBreakLength; }
    else
    {
        // If no exact pointer match, we still need to make sure we avoid overlap: set pointer to max(current, pSlot.mEnd + break)
        usedRoom.mMorningPointerByDay[pDay] = max(usedRoom.mMorningPointerByDay[pDay], pSlot.mEnd + this->mBreakLength);
        usedRoom.mAfternoonPointerByDay[pDay] = max(usedRoom.mAfternoonPointerByDay[pDay], pSlot.mEnd + this->mBreakLength);
    }
}

// Print final schedule
void Scheduler::printSchedule()
{
    // Sort assignments by day, start time
    sort(this->mAssignments.begin(), this->mAssignments.end(), [](const Presentation &assignmentOne, const Presentation &assignmentTwo) {
        if (assignmentOne.mDay != assignmentTwo.mDay) return assignmentOne.mDay < assignmentTwo.mDay;
        if (assignmentOne.mStartMinute != assignmentTwo.mStartMinute) return assignmentOne.mStartMinute < assignmentTwo.mStartMinute;
        return assignmentOne.mRoomId < assignmentTwo.mRoomId;
    });

    cout << "Full schedule (" << this->mAssignments.size() << " presentations):\n";
    for(auto &currentAssignment : this->mAssignments)
    {
        const Student &concernedStudent = this->mStudents[currentAssignment.mStudentId];
        const Teacher &concernedReferentTeacher = this->mTeachers[currentAssignment.mReferentTeacherId];
        const Teacher &concernedSecondTeacher = this->mTeachers[currentAssignment.mSecondTeacherId];
        cout << "Day " << currentAssignment.mDay+1 << " | " << Utils::minutesToHHMM(currentAssignment.mStartMinute) << " - "
                << Utils::minutesToHHMM(currentAssignment.mStartMinute + currentAssignment.mDuration) << " (" << currentAssignment.mDuration << " mins)"
                << " | Room " << mRooms[currentAssignment.mRoomId].mTag
                << " | Student: " << concernedStudent.mName
                << " | Referent: " << concernedReferentTeacher.mName << (concernedReferentTeacher.mIsTechnical ? " Tech" : "\t")
                << " | Second: " << concernedSecondTeacher.mName << (concernedSecondTeacher.mIsTechnical ? " Tech\t" : "\t\t")
                << "\n";
    }
}

// Output final schedule to a JSON file
string Scheduler::outputJSONResult() const
{
    nlohmann::json schedule = nlohmann::json::array();

    for (const auto &currentAssignment : this->mAssignments)
    {
        const Student &student = this->mStudents[currentAssignment.mStudentId];
        const Teacher &second_teacher = this->mTeachers[currentAssignment.mSecondTeacherId];
        const Room &room = this->mRooms[currentAssignment.mRoomId];

        const unsigned short int startTime = currentAssignment.mStartMinute;
        const unsigned short int endTime = currentAssignment.mStartMinute + currentAssignment.mDuration;

        nlohmann::json obj = {
            {"idUPPA", student.mId},
            {"idLecteur", second_teacher.mId},
            {"date",  currentAssignment.mDay},
            {"heureDebut", startTime},
            {"heureFin", endTime},
            {"nomSalle", room.mTag},
        };

        schedule.push_back(obj);
    }

    return schedule.dump();
}

