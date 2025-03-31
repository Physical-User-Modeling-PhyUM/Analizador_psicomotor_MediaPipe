#ifndef EXERCISEESPEC_H
#define EXERCISEESPEC_H

#include <QPair>
#include <QSet>
#include "pose/state.h"


class ExerciseEspec
{
public:
    ExerciseEspec();
    QList<State> getStatesList() const;

    QHash<QPair<int, int>, QSet<Condition> > getTransitionTable() const;

private:
    int idEx;
    QString name;
    QString description;
    QString exersiseType;
    QString targetMuscle;
    QString Equipment;
    int series=-1;
    int repetitions=-1;
    int duration=-1;
    int weight=-1;
    int restTime=-1;

    QList<State> statesList;
    QHash<QPair<int, int>, QSet<Condition>> transitionTable;
};

#endif // EXERCISEESPEC_H
