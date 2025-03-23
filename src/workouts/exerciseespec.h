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
    QString targetMuscle;
    QList<State> statesList;
    QHash<QPair<int, int>, QSet<Condition>> transitionTable;
};

#endif // EXERCISEESPEC_H
