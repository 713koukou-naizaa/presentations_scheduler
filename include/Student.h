#ifndef STUDENT_H
#define STUDENT_H


struct Student
{
    unsigned int mId{};
    bool mHasAccommodations{};
    unsigned short int mEffectivePresentationLength{};
    unsigned short int mReferentTeacherId{};

    Student(unsigned int pId, bool pHasAccommodations, unsigned short int pEffectivePresentationLength, unsigned short int pReferentTeacherId);
};

#endif