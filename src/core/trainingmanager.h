/**
 * @file trainingmanager.h
 * @brief Declaración de la clase TrainingManager para la gestión de entrenamientos y sesiones.
 *
 * Esta clase proporciona una capa de lógica de negocio sobre el repositorio `TrainingRepository`,
 * facilitando operaciones de consulta, modificación y ejecución de planes de entrenamiento.
 */

#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include "ui/main/uitablemodel.h"
#include "workouts/trainingworkout.h"
#include "workouts/trainingsesion.h"
#include "workouts/workoutsummary.h"
#include "workouts/exercisesummary.h"
#include "enums/TypeBDEnum.h"
#include "repositories/trainingrepository.h"
#include "utils/uiutils.h"
#include <QPointer>
#include <QDateTime>

/**
 * @class TrainingManager
 * @brief Clase encargada de coordinar el acceso a entrenamientos y sesiones mediante un repositorio.
 *
 * Esta clase encapsula la lógica de acceso a los datos de entrenamiento (workouts), ejercicios, sesiones activas,
 * y resúmenes asociados. Es el punto de entrada para crear, editar, listar y ejecutar sesiones de entrenamiento.
 */
class TrainingManager
{
public:
    /**
     * @brief Constructor que recibe un repositorio de entrenamientos.
     * @param repo Repositorio con acceso a la base de datos.
     */
    explicit TrainingManager(QSharedPointer<TrainingRepository> repo);

    // --- Gestión de workouts ---

    /**
     * @brief Recupera un entrenamiento a partir de su ID.
     * @param idWorkout Identificador del workout.
     * @return Objeto TrainingWorkout cargado.
     */
    QSharedPointer<TrainingWorkout> getWorkoutData(int idWorkout);

    /**
     * @brief Guarda un entrenamiento en el repositorio.
     * @param workout Objeto a guardar.
     * @return ID del workout guardado.
     */
    int saveWorkout(QSharedPointer<TrainingWorkout> workout);

    /**
     * @brief Elimina un entrenamiento completo.
     * @param idWorkout Identificador del workout.
     * @return true si se eliminó correctamente.
     */
    bool deleteWorkout(int idWorkout);

    /**
     * @brief Lista resúmenes de entrenamientos según un filtro.
     * @param filtro Campo a filtrar.
     * @param valor Valor del filtro.
     * @return Lista de resúmenes.
     */
    QList<WorkoutSummary> listWorkouts(DBFilter filtro = DBFilter::none, const QString& valor = {});

    /**
     * @brief Convierte el workout actual a un resumen.
     * @return Objeto WorkoutSummary.
     */
    WorkoutSummary currentWorkoutToSummary();

    // --- Gestión de ejercicios ---

    /**
     * @brief Recupera una especificación de ejercicio.
     * @param idEx Identificador del ejercicio.
     * @return Objeto ExerciseEspec.
     */
    QSharedPointer<ExerciseEspec> getExercise(int idEx);

    /**
     * @brief Guarda una especificación de ejercicio.
     * @param espec Especificación del ejercicio.
     * @return ID asignado.
     */
    int saveExercise(QSharedPointer<ExerciseEspec> espec);

    /**
     * @brief Lista resúmenes de ejercicios según filtro.
     * @param filtro Campo por el cual filtrar.
     * @param valor Valor del filtro.
     * @return Lista de resúmenes de ejercicios.
     */
    QList<ExerciseSummary> listExercises(DBFilter filtro = DBFilter::none, const QString& valor = {});

    /**
     * @brief Devuelve un resumen de un ejercicio dentro del workout actual.
     * @param idEx ID del ejercicio.
     * @return Objeto ExerciseSummary.
     */
    ExerciseSummary currentWorkoutExerciseToSummary(int idEx);

    // --- Ejecución de sesiones ---

    /**
     * @brief Inicia una nueva sesión de entrenamiento.
     * @param idClient ID del cliente.
     * @param IdEx ID del ejercicio.
     * @return Objeto TrainingSesion iniciado.
     */
    QSharedPointer<TrainingSesion> startSesion(int idClient, int IdEx);

    /**
     * @brief Finaliza la sesión actual y la guarda.
     * @return true si se guardó correctamente.
     */
    bool endCurrentSesion();

    /**
     * @brief Obtiene el siguiente ID disponible para ejercicios.
     * @return Entero ID disponible.
     */
    int getNextExerciseId();

    /**
     * @brief Obtiene el siguiente ID disponible para workouts.
     * @return Entero ID disponible.
     */
    int getNextWorkoutId();

    /**
     * @brief Recupera una sesión guardada desde su ID.
     * @param idSesion ID de la sesión.
     * @return Objeto TrainingSesion.
     */
    QSharedPointer<TrainingSesion> getTrainingSesion(int idSesion);

    /**
     * @brief Accede directamente al repositorio utilizado por el manager.
     * @return Puntero al repositorio.
     */
    QSharedPointer<TrainingRepository> getRepository() const;

    /**
     * @brief Obtiene un mapa de sesiones con fechas según los filtros aplicados.
     * @param idClient ID del cliente.
     * @param idWorkout ID del entrenamiento.
     * @param idEx ID del ejercicio.
     * @return Mapa <ID sesión, fecha>.
     */
    QMap<int,QDateTime> getSesionList(int idClient, int idWorkout, int idEx);

private:
    QSharedPointer<TrainingRepository> repository;        ///< Repositorio de entrenamientos.
    QSharedPointer<TrainingWorkout> currentWorkout;       ///< Workout actualmente en edición o ejecución.
    QSharedPointer<TrainingSesion> currentSesion;         ///< Sesión en ejecución.
};

#endif // TRAININGMANAGER_H
