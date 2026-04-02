#ifndef STUDENT_H
#define STUDENT_H

/**
 * @brief Represents a student concerned by the scheduling
 */
struct Student
{
    unsigned int mId{}; /// The student's UPPA ID
    bool mHasAccommodations{}; /// Whether the student has accommodations ("tier-temps" or anything else) for the length of his presentation
    unsigned short int mEffectivePresentationLength{}; /// The effective length of the student's presentation, which is either the normal presentation length or the accommodated presentation length, depending on whether the student has accommodations or not (computed from the global configuration when reading the data from stdin)
    unsigned short int mReferentTeacherId{};

    /**
     * @brief Construct a new Student object
     *
     * @param pId
     * @param pHasAccommodations
     * @param pEffectivePresentationLength
     * @param pReferentTeacherId
     */
    Student(unsigned int pId, bool pHasAccommodations, unsigned short int pEffectivePresentationLength, unsigned short int pReferentTeacherId);
};

#endif // STUDENT_H