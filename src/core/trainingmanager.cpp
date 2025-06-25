/**
 * @file trainingmanager.cpp
 * @brief Implementación de la clase TrainingManager para control de lógica de entrenamientos.
 */
#include "trainingmanager.h"
/**
 * @brief Constructor que inicializa el repositorio.
 */

TrainingManager::TrainingManager(QSharedPointer<TrainingRepository> repo)
    : repository(repo)
{
}

/**
 * @brief Obtiene los datos completos de un entrenamiento.
 */
QSharedPointer<TrainingWorkout> TrainingManager::getWorkoutData(int idWorkout)
{
    currentWorkout = repository->getWorkout(idWorkout);
    return currentWorkout;
}
/**
 * @brief Guarda un entrenamiento en el repositorio.
 */
int TrainingManager::saveWorkout(QSharedPointer<TrainingWorkout> workout)
{
    return repository->saveWorkout(workout);
}
/**
 * @brief Elimina completamente un entrenamiento.
 */
bool TrainingManager::deleteWorkout(int idWorkout)
{
    return repository->deleteWorkoutfull(idWorkout);
}
/**
 * @brief Lista todos los entrenamientos resumidos aplicando un filtro.
 */
QList<WorkoutSummary> TrainingManager::listWorkouts(DBFilter filtro, const QString& valor)
{
    QList<WorkoutSummary> list;
    DBTable table = repository->listWorkouts(filtro, valor);

    for (int i = 0; i < table.rowCount(); ++i) {
        QList<QVariant> row = table.getRow(i);

        WorkoutSummary summary(
            row[0].toInt(),     // idWorkout
            row[1].toString(),  // name
            row[2].toString(),  // description
            row[3].toString()   // author
            );

        list.append(summary);
    }

    return list;
}
/**
 * @brief Devuelve el resumen del entrenamiento actual.
 */
WorkoutSummary TrainingManager::currentWorkoutToSummary()
{
    if (currentWorkout)
        return currentWorkout->toSummary();
    return WorkoutSummary(-1, "", "", "");
}

/**
 * @brief Devuelve los datos de una especificación de ejercicio.
 */

QSharedPointer<ExerciseEspec> TrainingManager::getExercise(int idEx)
{
    return repository->getEspec(idEx);
}
/**
 * @brief Guarda una especificación de ejercicio.
 */
int TrainingManager::saveExercise(QSharedPointer<ExerciseEspec> espec)
{
    return repository->saveEspec(espec);
}
/**
 * @brief Lista resúmenes de ejercicios desde el repositorio.
 */
QList<ExerciseSummary> TrainingManager::listExercises(DBFilter filtro, const QString& valor)
{
    QList<ExerciseSummary> list={};
    DBTable table = repository->listExercises(filtro, valor);

    for (int i = 0; i < table.rowCount(); ++i) {
        QList<QVariant> row = table.getRow(i);

        ExerciseSummary summary(
            row[0].toInt(),       // idEx
            row[1].toString(),    // name
            row[2].toString(),    // description
            row[3].toString(),    // type
            row[4].toString()     // targetMuscle
            );

        list.append(summary);
    }
    return list;
}
/**
 * @brief Devuelve el resumen de un ejercicio del workout actual.
 */
ExerciseSummary TrainingManager::currentWorkoutExerciseToSummary(int idEx)
{
    if (!currentWorkout) return ExerciseSummary();

    auto ex = currentWorkout->getExercise(idEx);
    if (ex) return ex->toSummary();

    return ExerciseSummary();
}
/**
 * @brief Obtiene una sesión de entrenamiento existente desde su ID.
 */
QSharedPointer<TrainingSesion>TrainingManager::getTrainingSesion(int idSesion)
{
        return repository->getTrainingSesion(idSesion);
}

/**
 * @brief Inicia una nueva sesión de entrenamiento para un cliente y un ejercicio.
 */
QSharedPointer<TrainingSesion> TrainingManager::startSesion(int idClient, int IdEx)
{
    auto exercise = repository->getEspec(IdEx);
    if (!exercise) return nullptr;

    auto summary = QSharedPointer<ExerciseSummary>::create(exercise->toSummary());
    summary->setIdWorkout(currentWorkout->getIdWorkout());

    currentSesion = QSharedPointer<TrainingSesion>::create(summary, QDateTime::currentDateTime(), idClient);
    return currentSesion;
}
/**
 * @brief Finaliza la sesión actual y la guarda en el repositorio.
 */
bool TrainingManager::endCurrentSesion()
{
    if (!currentSesion) return false;

    currentSesion->setDuration(currentSesion->getDate().msecsTo(QDateTime::currentDateTime()));

    return repository->SaveTrainingSesion(currentSesion);
}

/**
 * @brief Obtiene el siguiente ID disponible para ejercicios.
 */
int TrainingManager::getNextExerciseId(){return repository->getNextExerciseId();}
/**
 * @brief Obtiene el siguiente ID disponible para entrenamientos.
 */
int TrainingManager::getNextWorkoutId(){return repository->getNextWorkoutId();}
/**
 * @brief Devuelve el repositorio utilizado.
 */
QSharedPointer<TrainingRepository> TrainingManager::getRepository() const
{
    return repository;
}
/**
 * @brief Devuelve una lista de sesiones filtradas por cliente, entrenamiento y ejercicio.
 */
QMap<int,QDateTime> TrainingManager::getSesionList(int idClient, int idWorkout, int idEx) {
  return repository->listSesionSummaries(idClient, idWorkout, idEx);
}

