/**
 * @file workoutsummary.cpp
 * @brief Implementación de la clase WorkoutSummary.
 */

#include "workoutsummary.h"
#include <QDebug>

/**
 * @brief Constructor principal. Inicializa los campos del resumen.
 */
WorkoutSummary::WorkoutSummary(int idWorkout,
                               QString title,
                               QString description,
                               QString author,
                               int EstimatedTime)
    : idWorkout(idWorkout >= 0 ? idWorkout : -1),
    title(title.trimmed()),
    description(description.trimmed()),
    author(author.trimmed()),
    estimatedTime(EstimatedTime),
    totalExercises(0)
{
    if (idWorkout < 0) {
        qWarning() << "WorkoutSummary creado con idWorkout inválido:" << idWorkout;
    }
    if (title.isEmpty()) {
        qWarning() << "WorkoutSummary con título vacío.";
    }
}
/**
 * @brief Añade un resumen de ejercicio si no está ya en la lista.
 * @param ex Resumen del ejercicio a añadir.
 * @return true si se añadió correctamente; false si estaba duplicado.
 */
bool WorkoutSummary::addExerciseSummary(ExerciseSummary ex)
{
    if (!exercises.contains(ex)){
        exercises.append(ex);return true;}
    return false;
}
/**
 * @brief Devuelve la lista completa de ejercicios del entrenamiento.
 * @return Lista de objetos ExerciseSummary.
 */
QList<ExerciseSummary> WorkoutSummary::getExercises(){return exercises;}
/**
 * @brief Devuelve el tiempo estimado del entrenamiento en minutos.
 */
int WorkoutSummary::getEstimatedTime() const
{
    return estimatedTime;
}
/**
 * @brief Establece la duración estimada del entrenamiento.
 * @param newEstimatedTime Nuevo valor de duración estimada.
 */
void WorkoutSummary::setEstimatedTime(int newEstimatedTime)
{
    estimatedTime = newEstimatedTime;
}

/**
 * @brief Devuelve un resumen de ejercicio por su ID.
 * @param idEx ID del ejercicio buscado.
 * @return ExerciseSummary correspondiente, o uno vacío si no se encuentra.
 */
ExerciseSummary WorkoutSummary::getExercise(int idEx)
{
    for (ExerciseSummary ex: exercises){if (ex.getIdEx()==idEx) return ex;}
    return ExerciseSummary();
}
// --- Getters básicos ---
int WorkoutSummary::getIdWorkout() const
{
    return idWorkout;
}

QString WorkoutSummary::getTitle() const
{
    return title;
}

QString WorkoutSummary::getDescription() const
{
    return description;
}

QString WorkoutSummary::getAuthor() const
{
    return author;
}

int WorkoutSummary::getTotalExercises() const
{
    return exercises.size();
}
