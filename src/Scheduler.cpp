#include "Scheduler.h"

#include <bits/stdc++.h>
#include <iostream>
#include <stdexcept>

using namespace std;

using std::cout;
using std::endl;

Scheduler::Scheduler(vector<Student> pStudents, vector<Teacher> pTeachers, vector<Tutor> pTutors, vector<Room> pRooms) : mStudents(std::move(pStudents)), mTeachers(std::move(pTeachers)), mTutors(std::move(pTutors)), mRooms(std::move(pRooms)) {}

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
    for(auto &currentTutor : this->mTutors) { if (static_cast<unsigned short int>(currentTutor.mBusyByDay.size()) <= pDay) currentTutor.mBusyByDay.resize(pDay+1); }
}

// Try to schedule all students with greedy earliest-fit, it creates additional days if needed.
bool Scheduler::scheduleAll()
{
    unsigned short int day = 0;
    ensureDayCapacity(day);

    // Order students sorted by accommodated length descending (longer first helps greedy, and sort helps algorithm)
    vector<unsigned short int> order(this->mStudents.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](unsigned short int studentIdOne, unsigned short int studentIdTwo) {
            return this->mStudents[studentIdOne].mEffectivePresentationLength > this->mStudents[studentIdTwo].mEffectivePresentationLength;
        }
    );

    for(unsigned short int currentStudentIdIndex=0; currentStudentIdIndex<static_cast<unsigned short int>(order.size()); ++currentStudentIdIndex)
    {
        const unsigned short int studentIndex = order[currentStudentIdIndex];
        Student &currentStudent = this->mStudents[studentIndex];

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
                    Utils::Interval currentSlot{start, end};

                    if (canPlace(currentStudent, tryDay, currentSlot))
                    {
                        place(currentStudent, tryDay, currentSlot, currentRoomId);
                        isCurrentStudentPlaced = true;
                        break;
                    }
                }
                // Else try afternoon pointer
                start = room.mAfternoonPointerByDay[tryDay];
                end = start + duration;
                if (end <= GLOBAL_CONFIG.END_AFTERNOON_TIME && !isCurrentStudentPlaced)
                {
                    Utils::Interval currentSlot{start, end};

                    if (canPlace(currentStudent, tryDay, currentSlot))
                    {
                        place(currentStudent, tryDay, currentSlot, currentRoomId);
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
bool Scheduler::canPlace(const Student &pStudent, unsigned short int pDay, const Utils::Interval &pSlot)
{
    // Referent teacher must be available
    Teacher &studentReferentTeacher = this->mTeachers[pStudent.mReferentTeacherId];
    if (!studentReferentTeacher.isAvailable(pDay, pSlot)) return false;

    // Find a second teacher (teacher != referent) who is available and respects the "technical" constraint
    const bool needTechnicalSecond = !studentReferentTeacher.mIsTechnical;
    bool foundSecond = false;
    for(unsigned short int currentTeacherId=0; currentTeacherId<static_cast<unsigned short int>(this->mTeachers.size()); ++currentTeacherId)
    {
        if (currentTeacherId == studentReferentTeacher.mId) continue;
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
void Scheduler::place(const Student &pStudent, unsigned short int pDay, const Utils::Interval &pSlot, unsigned short int pRoomId)
{
    Teacher &studentReferentTeacher = this->mTeachers[pStudent.mReferentTeacherId];
    Tutor &studentTutor = this->mTutors[pStudent.mTutorId];

    // Select second teacher candidates
    vector<unsigned short int> secondTeacherCandidates;
    bool needTechnicalSecond = !studentReferentTeacher.mIsTechnical;
    for(unsigned short int currentTeacherId=0; currentTeacherId<static_cast<unsigned short int>(this->mTeachers.size()); ++currentTeacherId)
    {
        if (currentTeacherId == studentReferentTeacher.mId) continue;
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
    const unsigned short int secondTeacherId = secondTeacherCandidates[0];

    // Book referent, second teacher, and tutor
    studentReferentTeacher.book(pDay, pSlot);
    this->mTeachers[secondTeacherId].book(pDay, pSlot);
    studentTutor.book(pDay, pSlot);

    // Record assignment as presentation
    Presentation finalPresentation{};
    finalPresentation.studentId = pStudent.mId;
    finalPresentation.day = pDay;
    finalPresentation.roomId = pRoomId;
    finalPresentation.startMinute = pSlot.mStart;
    finalPresentation.duration = pSlot.mEnd - pSlot.mStart;
    finalPresentation.referentTeacherId = studentReferentTeacher.mId;
    finalPresentation.secondTeacherId = secondTeacherId;
    finalPresentation.tutorId = pStudent.mTutorId;
    this->mAssignments.push_back(finalPresentation);

    // Advance room pointer for that day: add presentation time + break
    ensureDayCapacity(pDay);
    Room &usedRoom = mRooms[pRoomId];
    // Decide whether we used morning or afternoon pointer
    if (pSlot.mStart >= GLOBAL_CONFIG.START_MORNING_TIME && pSlot.mEnd <= GLOBAL_CONFIG.END_MORNING_TIME && pSlot.mStart == usedRoom.mMorningPointerByDay[pDay])
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
        if (assignmentOne.day != assignmentTwo.day) return assignmentOne.day < assignmentTwo.day;
        if (assignmentOne.startMinute != assignmentTwo.startMinute) return assignmentOne.startMinute < assignmentTwo.startMinute;
        return assignmentOne.roomId < assignmentTwo.roomId;
    });

    cout << "Full schedule (" << this->mAssignments.size() << " presentations):\n";
    for(auto &currentAssignment : this->mAssignments)
    {
        const Student &concernedStudent = this->mStudents[currentAssignment.studentId];
        const Teacher &concernedReferentTeacher = this->mTeachers[currentAssignment.referentTeacherId];
        const Teacher &concernedSecondTeacher = this->mTeachers[currentAssignment.secondTeacherId];
        const Tutor &concernedTutor = this->mTutors[currentAssignment.tutorId];
        cout << "Day " << currentAssignment.day+1 << " | " << Utils::minutesToHHMM(currentAssignment.startMinute) << " - "
                << Utils::minutesToHHMM(currentAssignment.startMinute + currentAssignment.duration) << " (" << currentAssignment.duration << " mins)" 
                << " | Room " << mRooms[currentAssignment.roomId].mTag
                << " | Student: " << concernedStudent.mName
                << " | Referent: " << concernedReferentTeacher.mName << (concernedReferentTeacher.mIsTechnical ? " Tech" : "\t")
                << " | Second: " << concernedSecondTeacher.mName << (concernedSecondTeacher.mIsTechnical ? " Tech\t" : "\t\t")
                << " | Tutor: " << concernedTutor.mName
                << "\n";
    }
}
