/**
 * @file exercisesummary.cpp
 * @brief Implementación de la clase ExerciseSummary.
 */

#include "exercisesummary.h"
#include <QDebug>

/**
 * @brief Constructor por defecto. Inicializa con ID inválido.
 */
ExerciseSummary::ExerciseSummary()
{
    idEx=-1;
}
/**
 * @brief Constructor que inicializa los campos sin vincular a un workout.
 */
ExerciseSummary::ExerciseSummary(int idEx, QString name, QString description, QString type, QString targetMuscle, int series, int repetitions, int duration, float weightPercentage, int restTime):
    idEx(idEx),name(name),description(description),type(type),targetMuscle(targetMuscle),series(series),repetitions(repetitions),duration(duration),weightPercentage(weightPercentage),restTime()
{
    qDebug()<<"se ha generado el resumen dek  ejerccio: " <<idEx;
}
/**
 * @brief Constructor que inicializa todos los campos incluyendo el ID del workout.
 */
ExerciseSummary::ExerciseSummary(int idEx,int idWorkout, QString name, QString description, QString type, QString targetMuscle, int series, int repetitions, int duration, float weightPercentage, int restTime):
    idEx(idEx),idWorkout(idWorkout),name(name),description(description),type(type),targetMuscle(targetMuscle),series(series),repetitions(repetitions),duration(duration),weightPercentage(weightPercentage),restTime()
{
    qDebug()<<"se ha generado el resumen dek  ejerccio: " <<idEx;
}
// ExerciseSummary:: ExerciseSummary(QSharedPointer<ExerciseEspec> spec){

//     if (spec!=nullptr){
//         idEx=spec->getIdEx();
//         name=spec->getName();
//         description=spec->getDescription();
//         type=spec->getExersiseType();
//         targetMuscle=spec->getTargetMuscle();
//         series=spec->getSeries();
//         repetitions=spec->getRepetitions();
//         duration=spec->getDuration();
//         weightPercentage=spec->getWeightPercentage();
//         restTime=spec->getRestTime();

//     }


// }
/**
 * @brief Compara si dos resúmenes hacen referencia al mismo ejercicio.
 */
bool ExerciseSummary::operator==(const ExerciseSummary& o) const {
    return idEx == o.getIdEx();
}

// Getters

int ExerciseSummary::getIdEx() const
{
    return idEx;
}

QString ExerciseSummary::getName() const
{
    return name;
}

QString ExerciseSummary::getType() const
{
    return type;
}

QString ExerciseSummary::getTargetMuscle() const
{
    return targetMuscle;
}

QString ExerciseSummary::getDescription() const
{
    return description;
}

int ExerciseSummary::getSeries() const
{
    return series;
}

int ExerciseSummary::getRepetitions() const
{
    return repetitions;
}

int ExerciseSummary::getDuration() const
{
    return duration;
}

float ExerciseSummary::getWeightPercentage() const
{
    return weightPercentage;
}

int ExerciseSummary::getRestTime() const
{
    return restTime;
}

int ExerciseSummary::getIdWorkout() const
{
    return idWorkout;
}
/**
 * @brief Asigna el ID del workout al que pertenece este ejercicio.
 */
void ExerciseSummary::setIdWorkout(int newIdWorkout)
{
    idWorkout = newIdWorkout;
}
