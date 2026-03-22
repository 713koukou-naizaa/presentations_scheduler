#include "Presentation.h"

Presentation::Presentation(const unsigned short int pStudentId, const unsigned short int pDay, const unsigned short int pRoomId, const unsigned short int pStartMinute, const unsigned short int pDuration, const unsigned short int pReferentTeacherId, const unsigned short int pSecondTeacherId) :
mStudentId(pStudentId), mDay(pDay), mRoomId(pRoomId), mStartMinute(pStartMinute), mDuration(pDuration), mReferentTeacherId(pReferentTeacherId), mSecondTeacherId(pSecondTeacherId) {}