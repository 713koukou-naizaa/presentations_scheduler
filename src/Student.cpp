#include "Student.h"


Student::Student(const unsigned int pId, const bool pHasAccommodations, const unsigned short int pEffectivePresentationLength, const unsigned short int pReferentTeacherId) : mId(pId), mHasAccommodations(pHasAccommodations), mEffectivePresentationLength(pEffectivePresentationLength), mReferentTeacherId(pReferentTeacherId) {}