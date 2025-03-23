#ifndef EXERCISE_H
#define EXERCISE_H

#include "exerciseespec.h"

class Exercise
{
public:
    Exercise();
    int getRepetitions() const;

    int getDuration() const;

    int getRestTime() const;

    int getSeries() const;

private:
    int  idEspec;
    QString name;
    QString exersiseType;
    QString Equipment;
    int series=-1;
    int repetitions=-1;
    int duration=-1;
    int weight=-1;
    int restTime=-1;


};

#endif // EXERCISE_H
