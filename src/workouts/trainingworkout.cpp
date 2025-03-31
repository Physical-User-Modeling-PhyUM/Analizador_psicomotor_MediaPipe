#include "trainingworkout.h"

TrainingWorkout::TrainingWorkout(int idWorkout):idWorkout(idWorkout) {
    qDebug()<<"Se ha creado un plan de entrenamiento vacio";
}

TrainingWorkout::TrainingWorkout(int idWorkout,const QString &name, const QString &description, int duration, const QList<ExerciseEspec> &exercises)
    :idWorkout(idWorkout),
    name(name),
    description(description),
    estimatedDuration(duration),
    exercises(exercises)
{
     qDebug()<<"Se ha creado un plan de entrenamiento";
}

int TrainingWorkout::getIdWorkout() const
{
    return idWorkout;
}

QString TrainingWorkout::getName() const
{
    return name;
}

void TrainingWorkout::setName(const QString &newName)
{
    name = newName;
}

QString TrainingWorkout::getDescription() const
{
    return description;
}

void TrainingWorkout::setDescription(const QString &newDescription)
{
    description = newDescription;
}

int TrainingWorkout::getEstimatedDuration() const
{
    return estimatedDuration;
}

void TrainingWorkout::setEstimatedDuration(int newEstimatedDuration)
{
    estimatedDuration = newEstimatedDuration;
}

QList<ExerciseEspec> TrainingWorkout::getExercises() const
{
    return exercises;
}
