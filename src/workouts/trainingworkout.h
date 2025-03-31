#ifndef TRAININGWORKOUT_H
#define TRAININGWORKOUT_H
#include <QString>
#include "exerciseespec.h"
class TrainingWorkout
{
public:
     explicit TrainingWorkout(int idWorkout);
     TrainingWorkout(   int idWorkout,
                        const QString& name,
                        const QString& description,
                        int duration,
                        const QList<ExerciseEspec>& exercises
                    );

     int getIdWorkout() const;

     QString getName() const;
     void setName(const QString &newName);

     QString getDescription() const;
     void setDescription(const QString &newDescription);

     int getEstimatedDuration() const;
     void setEstimatedDuration(int newEstimatedDuration);

     QList<ExerciseEspec> getExercises() const;
     void addExercise(const ExerciseEspec& exercise);

 private:
    int idWorkout;
    QString name;
    QString description;
    int estimatedDuration;
    QList<ExerciseEspec> exercises;

};

#endif // TRAININGWORKOUT_H
