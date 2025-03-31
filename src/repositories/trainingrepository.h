#ifndef TRAININGREPOSITORY_H
#define TRAININGREPOSITORY_H

#include "db/DBManager.h"
#include <QPointer>
#include "workouts/exerciseespec.h"
#include "workouts/trainingworkout.h"

class TrainingRepository: public QObject
{
    Q_OBJECT
public:
    explicit TrainingRepository(DBManager* dbManager, QObject* parent = nullptr);

    QPointer<ExerciseEspec>  getEspec();
    QPointer<TrainingWorkout> getWorkout();

    DBTable listWorkouts();
    DBTable listExercise();

};

#endif // TRAININGREPOSITORY_H
