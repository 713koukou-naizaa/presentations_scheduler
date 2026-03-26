#ifndef PRESENTATION_H
#define PRESENTATION_H

/**
 * @brief Represents a scheduled presentation
 */
struct Presentation
{
    unsigned short int mStudentId;
    unsigned short int mDay; /// 0-based
    unsigned short int mRoomId;
    unsigned short int mStartMinute; /// Start minutes from midnight
    unsigned short int mDuration; /// Presentation + committee decision duration (excluding in-between breaks)
    unsigned short int mReferentTeacherId;
    unsigned short int mSecondTeacherId;

    /**
     * @brief Construct a new Presentation object
     *
     * @param pStudentId
     * @param pDay
     * @param pRoomId
     * @param pStartMinute
     * @param pDuration
     * @param pReferentTeacherId
     * @param pSecondTeacherId
     */
    Presentation(unsigned short int pStudentId, unsigned short int pDay, unsigned short int pRoomId, unsigned short int pStartMinute, unsigned short int pDuration, unsigned short int pReferentTeacherId, unsigned short int pSecondTeacherId);
};

#endif // PRESENTATION_H