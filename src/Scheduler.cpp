#include "Scheduler.h"

#include <bits/stdc++.h>
#include <iostream>

using namespace std;

using std::cout;
using std::endl;


Scheduler::Scheduler(vector<Student> pStudents, vector<Teacher> pTeachers, vector<Room> pRooms) : mStudents(std::move(pStudents)), mTeachers(std::move(pTeachers)), mRooms(std::move(pRooms)) {}

/*
INITIALIZATION
Prepare vector of students index (to schedule, to retry scheduling same day, to retry scheduling next week)
Initialize current day to 0, week to 1 and latest week number to 0
Initialize day capacity for day 0

MAIN LOOP
While all vectors aren't empty :
 - If we advanced to a new week (latest week number < current week), update students to retry scheduling same day and next week
 - Initialize day capacity for current day
 - For each room :
  - Try scheduling students the morning until full, and then afternoon until full
 - End of day, advance to new day
 - If current day % 5, new week, so update week and reset teachers weekly remaining minutes
 - If no progress was made this day, move remaining students to retry scheduling same day to students to retry scheduling next week
return true once all students have been scheduled
*/
bool Scheduler::scheduleAll()
{
    // Prepare students to schedule vector using student indices
    vector<unsigned short int> toScheduleStudents;
    toScheduleStudents.reserve(this->mStudents.size());
    for (size_t i = 0; i < this->mStudents.size(); ++i) toScheduleStudents.push_back(static_cast<unsigned short int>(i));

    vector<unsigned short int> toRetrySchedulingSameDay; // students to try to schedule later the same day because they failed once
    vector<unsigned short int> toRetrySchedulingNextWeekStudents; // students delayed to next week because of weekly limit (delayed until the end of the week, or their referent is close to being overworked)

    unsigned short int dayNumber = 0;
    unsigned short int weekNumber = 1;
    unsigned short int latestWeekNumber = 0;

    // Initialize per-day capacity for day 0
    this->ensureDayCapacity(dayNumber);

    // Loop until all lists empty
    while (!toScheduleStudents.empty() || !toRetrySchedulingSameDay.empty() || !toRetrySchedulingNextWeekStudents.empty())
    {
        // If we advanced to a new week, attempt students in toRetrySchedulingNextWeekStudents first
        if (latestWeekNumber < weekNumber && !toRetrySchedulingNextWeekStudents.empty())
        {
            // move toRetrySchedulingNextWeekStudents into toRetrySchedulingSameDay to attempt scheduling at beginning of new week
            toRetrySchedulingSameDay.insert(toRetrySchedulingSameDay.end(), toRetrySchedulingNextWeekStudents.begin(), toRetrySchedulingNextWeekStudents.end());
            toRetrySchedulingNextWeekStudents.clear();
        }

        // For each room, try to fill morning then afternoon for this day
        this->ensureDayCapacity(dayNumber);

        bool madeProgressThisDay = false;

        for (size_t currentRoomIdx = 0; currentRoomIdx < mRooms.size(); ++currentRoomIdx)
        {
            // Morning
            this->trySchedulingUntilTimeWindowFull(dayNumber, currentRoomIdx, mRooms[currentRoomIdx].mMorningPointerByDay[dayNumber], GLOBAL_CONFIG.END_MORNING_TIME, toRetrySchedulingSameDay, toScheduleStudents, toRetrySchedulingNextWeekStudents, madeProgressThisDay);

            // Afternoon
            this->trySchedulingUntilTimeWindowFull(dayNumber, currentRoomIdx, mRooms[currentRoomIdx].mAfternoonPointerByDay[dayNumber], GLOBAL_CONFIG.END_AFTERNOON_TIME, toRetrySchedulingSameDay, toScheduleStudents, toRetrySchedulingNextWeekStudents, madeProgressThisDay);
        }

        // End of day: advance day
        ++dayNumber;

        // Weekly reset every 5 days (assuming 5 working days per week)
        if (dayNumber % 5 == 0)
        {
            ++weekNumber;
            latestWeekNumber = weekNumber - 1;
            // reset weekly remaining minutes for all teachers
            for (auto &currentTeacher : this->mTeachers) currentTeacher.mWeeklyRemainingMinutes = GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME;
        }

        // If no progress was made this day and we still have students to schedule, move any remaining toRetrySchedulingSameDay to toRetrySchedulingNextWeekStudents to avoid infinite loop
        if (!madeProgressThisDay)
        {
            for (auto currentStudentToRetrySchedulingSameDay : toRetrySchedulingSameDay) if (std::find(toRetrySchedulingNextWeekStudents.begin(), toRetrySchedulingNextWeekStudents.end(), currentStudentToRetrySchedulingSameDay) == toRetrySchedulingNextWeekStudents.end()) toRetrySchedulingNextWeekStudents.push_back(currentStudentToRetrySchedulingSameDay);
            toRetrySchedulingSameDay.clear();
            // prevent infinite loops: if also toRetrySchedulingNextWeekStudents didn't change and toScheduleStudents same, we still advance day
        }
    }

    return true;
}

/*
Indefinitely :
- If the startTime is equal or past the endTime of the corresponding time window, window full, break
- For each queues (students to schedule and the students to retry scheduling the same day) :
 - For each student in the current queue :
  - Get the student's and presentation's information
  - Check if the presentation fits in the corresponding time window, if not, move the student to students to retry scheduling the same day if not already in
  - Else, try scheduling the student

  - If the result is TrySchedulingResult::SCHEDULED :
   - Remove them from all vectors
   - Update the day capacity

  - If the result is TrySchedulingResult::SAME_DAY, move the student to students to retry scheduling the same day if not already in
  - If the result isTrySchedulingResult::NEXT_WEEK, move the student to students to retry scheduling the next week if not already in

 - If a student was scheduled, break

- If no student was scheduled, break
*/
void Scheduler::trySchedulingUntilTimeWindowFull(const unsigned short dayNumber, const unsigned short int currentRoomIdx,
    int& startTime, const int endTime,
    std::vector<unsigned short int>& toRetrySchedulingSameDay, std::vector<unsigned short int>& toScheduleStudents, std::vector<unsigned short int>& toRetrySchedulingNextWeekStudents,
    bool& madeProgressThisDay)
{
    while (true)
    {
        if (startTime + 1 > endTime) break;

        bool scheduledOne = false;

        for (auto* currentQueuePtr : { &toRetrySchedulingSameDay, &toScheduleStudents })
        {
            auto& currentQueue = *currentQueuePtr;

            for (size_t idx = 0; idx < currentQueue.size(); ++idx)
            {
                const unsigned short currentStudentIdx = currentQueue[idx];
                const unsigned short int duration = this->mStudents[currentStudentIdx].mEffectivePresentationLength;
                Utils::Interval slot{static_cast<unsigned short int>(startTime), static_cast<unsigned short int>(startTime + duration)};

                if (slot.mEnd > static_cast<unsigned short int>(endTime))
                {
                    if (std::find(toRetrySchedulingSameDay.begin(), toRetrySchedulingSameDay.end(), currentStudentIdx) == toRetrySchedulingSameDay.end())
                        toRetrySchedulingSameDay.push_back(currentStudentIdx);
                    continue;
                }

                const TrySchedulingResultOptions TrySchedulingResult = this->tryScheduleStudentAtSlotAtDayAtRoom(currentStudentIdx, dayNumber, slot, static_cast<unsigned short int>(currentRoomIdx));

                if (TrySchedulingResult == SCHEDULED)
                {
                    Scheduler::removeStudentFromAllVectors(toScheduleStudents, toRetrySchedulingSameDay, toRetrySchedulingNextWeekStudents, currentStudentIdx);
                    scheduledOne = true;
                    madeProgressThisDay = true;
                    // update startTime from room pointers to reflect the booking
                    ensureDayCapacity(dayNumber);
                    Room &currentUsedRoom = mRooms[currentRoomIdx];
                    // prefer morning or afternoon pointer that matches the slot
                    if (slot.mStart >= GLOBAL_CONFIG.START_MORNING_TIME && slot.mEnd <= GLOBAL_CONFIG.END_MORNING_TIME)
                        startTime = currentUsedRoom.mMorningPointerByDay[dayNumber];
                    else if (slot.mStart >= GLOBAL_CONFIG.START_AFTERNOON_TIME && slot.mEnd <= GLOBAL_CONFIG.END_AFTERNOON_TIME)
                        startTime = currentUsedRoom.mAfternoonPointerByDay[dayNumber];
                    break;
                }

                if (TrySchedulingResult == SAME_DAY)
                {
                    if (std::find(toRetrySchedulingSameDay.begin(), toRetrySchedulingSameDay.end(), currentStudentIdx) == toRetrySchedulingSameDay.end())
                        toRetrySchedulingSameDay.push_back(currentStudentIdx);
                }
                else if (TrySchedulingResult == NEXT_WEEK)
                {
                    if (std::find(toRetrySchedulingNextWeekStudents.begin(), toRetrySchedulingNextWeekStudents.end(), currentStudentIdx) == toRetrySchedulingNextWeekStudents.end())
                        toRetrySchedulingNextWeekStudents.push_back(currentStudentIdx);

                    currentQueue.erase(currentQueue.begin() + idx);
                    --idx;
                }
            }

            if (scheduledOne) break;
        }

        if (!scheduledOne) break;
    }
}

/*
CHECKS CHAIN
Get current student from its index
Get the corresponding student's presentation duration
Check if the slot fits in either the morning or afternoon windows, if not, return TrySchedulingResultOptions::SAME_DAY

Get the corresponding referent teacher
Check if the referent teacher has enough weekly remaining minutes, if not, return TrySchedulingResultOptions::NEXT_WEEK
Check if referent teacher is available at this time, if not, return TrySchedulingResultOptions::SAME_DAY

Find second teacher candidates :
- Have to be technical if the referent is not
- Have to have enough weekly remaining minutes
- Have to be available at this time

If no candidates were found :
- If a candidate didn't have enough weekly remaining minutes, return TrySchedulingResultOptions::NEXT_WEEK
- If a candidate wasn't available at the time, return TrySchedulingResultOptions::SAME_DAY
- Else (technicality constraint not fulfilled or no other teacher), return TrySchedulingResultOptions::NEXT_WEEK
Choose best candidate based on all the candidates ratio of remaining time after assignment on max teachers weekly worked time (choose the one with the highest)

Book the teachers (referent and second teacher)
Record the assignment

Advance the room pointer for that day

return TrySchedulingResultOptions::SCHEDULED
*/
Scheduler::TrySchedulingResultOptions Scheduler::tryScheduleStudentAtSlotAtDayAtRoom(const unsigned short int &currentStudentIdx, const unsigned short int &currentDay, const Utils::Interval &currentSlot, const unsigned short int &currentRoomId)
{
    const Student &studentToTryScheduling = this->mStudents[currentStudentIdx];
    const unsigned short int currentStudentPresentationDuration = studentToTryScheduling.mEffectivePresentationLength;

    // Check slot fits into morning or afternoon windows
    const bool inMorning = currentSlot.mStart >= GLOBAL_CONFIG.START_MORNING_TIME && currentSlot.mEnd <= GLOBAL_CONFIG.END_MORNING_TIME;
    const bool inAfternoon = currentSlot.mStart >= GLOBAL_CONFIG.START_AFTERNOON_TIME && currentSlot.mEnd <= GLOBAL_CONFIG.END_AFTERNOON_TIME;
    if (!inMorning && !inAfternoon) return TrySchedulingResultOptions::SAME_DAY; // doesn't fit current day window

    // Referent teacher
    const unsigned short int correspondingReferentId = studentToTryScheduling.mReferentTeacherId;
    const int correspondingReferentIdx = Utils::findIndexById(this->mTeachers, correspondingReferentId);
    if (correspondingReferentIdx < 0) return TrySchedulingResultOptions::NOT_POSSIBLE; // malformed input
    Teacher &correspondingReferent = this->mTeachers[static_cast<size_t>(correspondingReferentIdx)];

    // Check referent weekly minutes first (overworked -> retry next week)
    if (correspondingReferent.mWeeklyRemainingMinutes < currentStudentPresentationDuration) return TrySchedulingResultOptions::NEXT_WEEK;
    // Check referent availability (overlap -> retry same day)
    if (!correspondingReferent.isAvailable(currentDay, currentSlot)) return TrySchedulingResultOptions::SAME_DAY;

    // Build second teacher candidates
    const bool needTechnicalSecond = !correspondingReferent.mIsTechnical;
    vector<unsigned short int> secondPossibleCandidates;
    bool hadOverworkedCandidate = false;
    bool hadBusyCandidate = false;

    for (size_t i = 0; i < this->mTeachers.size(); ++i)
    {
        if (static_cast<int>(i) == correspondingReferentIdx) continue;

        Teacher &possibleCurrentCandidate = this->mTeachers[i];
        if (needTechnicalSecond && !possibleCurrentCandidate.mIsTechnical) continue; // must be technical
        if (possibleCurrentCandidate.mWeeklyRemainingMinutes < currentStudentPresentationDuration) { hadOverworkedCandidate = true; continue; }
        // isAvailable checks weekly remaining too; we've already checked weekly minutes so only overlaps matter here
        if (!possibleCurrentCandidate.isAvailable(currentDay, currentSlot)) { hadBusyCandidate = true; continue; }

        secondPossibleCandidates.push_back(i);
    }

    if (secondPossibleCandidates.empty())
    {
        if (hadOverworkedCandidate) return TrySchedulingResultOptions::NEXT_WEEK;
        if (hadBusyCandidate) return TrySchedulingResultOptions::SAME_DAY;
        // No candidate because of technicality or no other teachers at all
        return TrySchedulingResultOptions::NEXT_WEEK;
    }

    double bestScore = -1.0;
    int bestCandidate = -1;
    for (const auto currentCandidateIdx : secondPossibleCandidates)
    {
        const Teacher &candidateTeacher = mTeachers[currentCandidateIdx];

        const double remainingAfterAssignment = candidateTeacher.mWeeklyRemainingMinutes - currentStudentPresentationDuration;
        const double score = remainingAfterAssignment / static_cast<double>(GLOBAL_CONFIG.MAX_TEACHERS_WEEKLY_WORKED_TIME);
        if (score > bestScore)
        {
            bestScore = score;
            bestCandidate = currentCandidateIdx;
        }
    }
    const unsigned short int chosenSecondTeacherCandidateIdx = bestCandidate;

    // Book teachers
    correspondingReferent.book(currentDay, currentSlot);
    this->mTeachers[chosenSecondTeacherCandidateIdx].book(currentDay, currentSlot);

    // TODO : Change the type of finalPresentation.mReferentTeacherId ?
    // Record assignment
    const Presentation finalPresentation(currentStudentIdx,
        currentDay, currentRoomId,
        currentSlot.mStart, currentStudentPresentationDuration,
        static_cast<unsigned short int>(correspondingReferentIdx), chosenSecondTeacherCandidateIdx);
    this->mAssignments.push_back(finalPresentation);

    // Advance room pointer for that day
    ensureDayCapacity(currentDay);

    Room &currentUsedRoom = mRooms[currentRoomId];
    if (inMorning && currentSlot.mStart == currentUsedRoom.mMorningPointerByDay[currentDay]) currentUsedRoom.mMorningPointerByDay[currentDay] = currentSlot.mEnd + this->mBreakLength;
    else if (inAfternoon && currentSlot.mStart == currentUsedRoom.mAfternoonPointerByDay[currentDay]) currentUsedRoom.mAfternoonPointerByDay[currentDay] = currentSlot.mEnd + this->mBreakLength;
    else
    {
        currentUsedRoom.mMorningPointerByDay[currentDay] = std::max(currentUsedRoom.mMorningPointerByDay[currentDay], currentSlot.mEnd + this->mBreakLength);
        currentUsedRoom.mAfternoonPointerByDay[currentDay] = std::max(currentUsedRoom.mAfternoonPointerByDay[currentDay], currentSlot.mEnd + this->mBreakLength);
    }

    return TrySchedulingResultOptions::SCHEDULED;
}

void Scheduler::ensureDayCapacity(const unsigned short int pDay)
{
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

void Scheduler::removeStudentFromAllVectors(vector<unsigned short int> &toScheduleStudents, vector<unsigned short int> &toRetrySchedulingSameDay, vector<unsigned short int> &toRetrySchedulingNextWeekStudents, const unsigned short int &toRemoveStudentIdx)
{
    Scheduler::removeStudentFromVector(toScheduleStudents, toRemoveStudentIdx);
    Scheduler::removeStudentFromVector(toRetrySchedulingSameDay, toRemoveStudentIdx);
    Scheduler::removeStudentFromVector(toRetrySchedulingNextWeekStudents, toRemoveStudentIdx);
}

void Scheduler::removeStudentFromVector(vector<unsigned short int> &vectorToRemoveFrom, const unsigned short int &toRemoveStudentIdx)
{ vectorToRemoveFrom.erase(std::remove(vectorToRemoveFrom.begin(), vectorToRemoveFrom.end(), toRemoveStudentIdx), vectorToRemoveFrom.end()); }

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

// GCOVR_EXCL_START
void Scheduler::printSchedule()
{
    // Sort assignments by day, start time
    sort(this->mAssignments.begin(), this->mAssignments.end(), [](const Presentation &assignmentOne, const Presentation &assignmentTwo) {
        if (assignmentOne.mDay != assignmentTwo.mDay) return assignmentOne.mDay < assignmentTwo.mDay;
        if (assignmentOne.mStartMinute != assignmentTwo.mStartMinute) return assignmentOne.mStartMinute < assignmentTwo.mStartMinute;
        return assignmentOne.mRoomId < assignmentTwo.mRoomId;
    });

    cout << "Full schedule (" << this->mAssignments.size() << " presentations):\n";
    for(const auto &currentAssignment : this->mAssignments)
    {
        const Student &concernedStudent = this->mStudents[currentAssignment.mStudentId];
        const Teacher &concernedReferentTeacher = this->mTeachers[currentAssignment.mReferentTeacherId];
        const Teacher &concernedSecondTeacher = this->mTeachers[currentAssignment.mSecondTeacherId];
        cout << "Day " << currentAssignment.mDay+1 << " | " << Utils::minutesToHHMM(currentAssignment.mStartMinute) << " - "
                << Utils::minutesToHHMM(currentAssignment.mStartMinute + currentAssignment.mDuration) << " (" << currentAssignment.mDuration << " mins)"
                << " | Room " << mRooms[currentAssignment.mRoomId].mId
                << " | Student: " << concernedStudent.mId
                << " | Referent: " << concernedReferentTeacher.mId << (concernedReferentTeacher.mIsTechnical ? " Tech" : "\t")
                << " | Second: " << concernedSecondTeacher.mId << (concernedSecondTeacher.mIsTechnical ? " Tech\t" : "\t\t")
                << "\n";
    }
}
// GCOVR_EXCL_STOP


