#ifndef PRESENTATION_H
#define PRESENTATION_H

struct Presentation
{
    unsigned short int mStudentId;
    unsigned short int mDay; // 0-based
    unsigned short int mRoomId;
    unsigned short int mStartMinute; // Minutes from midnight
    unsigned short int mDuration; // Presentation duration only (excluding breaks)
    unsigned short int mReferentTeacherId;
    unsigned short int mSecondTeacherId;
};

#endif