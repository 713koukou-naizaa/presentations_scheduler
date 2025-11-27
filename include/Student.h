#ifndef STUDENT_H
#define STUDENT_H

#include <string>

using std::string;

struct Student
{
    unsigned short int mId;
    string mName;
    bool mHasAccommodations;
    unsigned short int mEffectivePresentationLength;
    unsigned short int mReferentTeacherId;
    unsigned short int mTutorId;
};

#endif