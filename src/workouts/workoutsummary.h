/**
 * @file workoutsummary.h
 * @brief Declaración de la clase WorkoutSummary, que representa un resumen de un plan de entrenamiento.
 *
 * Incluye información como el nombre del entrenamiento, descripción, autor, duración estimada,
 * y una lista de ejercicios representados por instancias de ExerciseSummary.
 */

#ifndef WORKOUTSUMMARY_H
#define WORKOUTSUMMARY_H

#include <QString>
#include <QStringList>
#include "workouts/exercisesummary.h"

/**
 * @class WorkoutSummary
 * @brief Representa un resumen de un plan de entrenamiento.
 *
 * Esta clase se utiliza para mostrar o almacenar información simplificada de un entrenamiento, incluyendo
 * una lista de ejercicios resumidos y metadatos como nombre, autor y duración.
 */
class WorkoutSummary {
public:

    /**
     * @brief Constructor del resumen de entrenamiento.
     * @param idWorkout Identificador único del entrenamiento.
     * @param title Título del entrenamiento.
     * @param description Descripción del entrenamiento.
     * @param author Nombre del autor.
     * @param estimatedTime Duración estimada en minutos (opcional).
     */
    WorkoutSummary(int idWorkout,
                   QString title,
                   QString description,
                   QString author,
                   int estimatedTime = 0);

    /**
     * @brief Añade un resumen de ejercicio si aún no está incluido.
     * @param ex Objeto ExerciseSummary a añadir.
     * @return true si se añadió; false si ya existía.
     */
    bool addExerciseSummary(ExerciseSummary ex);

    /**
     * @brief Devuelve un resumen de ejercicio específico por su ID.
     * @param idEx ID del ejercicio buscado.
     * @return Objeto ExerciseSummary encontrado, o uno vacío si no existe.
     */
    ExerciseSummary getExercise(int idEx);

    /// Getters

    int getIdWorkout() const;             ///< Devuelve el ID del workout.
    QString getTitle() const;             ///< Devuelve el título del workout.
    QString getDescription() const;       ///< Devuelve la descripción del workout.
    QString getAuthor() const;            ///< Devuelve el nombre del autor.
    int getTotalExercises() const;        ///< Devuelve el número total de ejercicios.
    QList<ExerciseSummary> getExercises();///< Devuelve la lista de ejercicios.
    int getEstimatedTime() const;         ///< Devuelve la duración estimada.

    /**
     * @brief Establece una nueva duración estimada.
     * @param newEstimatedTime Duración en minutos.
     */
    void setEstimatedTime(int newEstimatedTime);

signals:
    /**
     * @brief Señal para emitir mensajes de error a la interfaz de usuario.
     * @param msg Mensaje textual.
     * @param type Tipo del mensaje.
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    int idWorkout;
    QString title;
    QString description;
    QString author;
    int estimatedTime;
    int totalExercises;
    QList<ExerciseSummary> exercises;
};

#endif // WORKOUTSUMMARY_H
