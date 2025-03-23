#include "exerciseespec.h"

ExerciseEspec::ExerciseEspec() {}

QList<State> ExerciseEspec::getStatesList() const
{
    return statesList;
}

QHash<QPair<int, int>, QSet<Condition> > ExerciseEspec::getTransitionTable() const
{
    return transitionTable;
}
