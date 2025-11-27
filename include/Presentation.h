#ifndef PRESENTATION_H
#define PRESENTATION_H

struct Presentation
{
    unsigned short int studentId;
    unsigned short int day; // 0-based
    unsigned short int roomId;
    unsigned short int startMinute; // Minutes from midnight
    unsigned short int duration; // Presentation duration only (excluding breaks)
    unsigned short int referentTeacherId;
    unsigned short int secondTeacherId;
    unsigned short int tutorId;
};

#endif