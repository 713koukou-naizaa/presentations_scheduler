#include "Student.h"

#include <utility>


Student::Student(const unsigned int pId, string pName, const bool pHasAccommodations, const unsigned short int pEffectivePresentationLength, const unsigned short int pReferentTeacherId) : mId(pId), mName(std::move(pName)), mHasAccommodations(pHasAccommodations), mEffectivePresentationLength(pEffectivePresentationLength), mReferentTeacherId(pReferentTeacherId) {}