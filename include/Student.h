#ifndef STUDENT_H
#define STUDENT_H

#include <string>

using std::string;

struct Student
{
    unsigned int mId{};
    string mName;
    bool mHasAccommodations{};
    unsigned short int mEffectivePresentationLength{};
    unsigned short int mReferentTeacherId{};

    Student(unsigned int pId, string pName, bool pHasAccommodations, unsigned short int pEffectivePresentationLength, unsigned short int pReferentTeacherId);
};

#endif