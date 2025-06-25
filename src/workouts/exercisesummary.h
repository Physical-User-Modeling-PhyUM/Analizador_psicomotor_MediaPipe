/**
 * @file exercisesummary.h
 * @brief Declaración de la clase ExerciseSummary, que representa un resumen de un ejercicio en un plan de entrenamiento.
 *
 * Esta clase encapsula la información esencial de un ejercicio: identificadores, descripción, parámetros físicos
 * y detalles para su ejecución. Se utiliza para mostrar listas de ejercicios o almacenar referencias resumidas
 * en otras estructuras más grandes como entrenamientos o sesiones.
 */

#ifndef EXERCISESUMMARY_H
#define EXERCISESUMMARY_H

#include <QString>
#include <QStringList>

/**
 * @class ExerciseSummary
 * @brief Contenedor de datos simplificado que representa un resumen de un ejercicio.
 *
 * Permite manipular y visualizar de forma ligera la información principal de un ejercicio.
 * Puede usarse en listas, vistas, estructuras de almacenamiento o exportación.
 */
class ExerciseSummary {
public:
    /**
     * @brief Constructor por defecto. Inicializa los valores con parámetros inválidos o vacíos.
     */
    ExerciseSummary();

    /**
     * @brief Constructor sin asignación a workout.
     * @param idEx ID del ejercicio.
     * @param name Nombre del ejercicio.
     * @param description Descripción textual del ejercicio.
     * @param type Tipo de ejercicio (fuerza, movilidad...).
     * @param targetMuscle Grupo muscular objetivo.
     * @param series Número de series.
     * @param repetitions Repeticiones por serie.
     * @param duration Duración estimada.
     * @param weightPercentage Porcentaje de carga respecto al máximo.
     * @param restTime Tiempo de descanso entre series.
     */
    ExerciseSummary(int idEx, QString name, QString description, QString type, QString targetMuscle,
                    int series = -1, int repetitions = -1, int duration = -1,
                    float weightPercentage = -1, int restTime = -1);

    /**
     * @brief Constructor con asignación a un workout.
     * @param idEx ID del ejercicio.
     * @param idWorkout ID del workout que lo contiene.
     * @param name Nombre del ejercicio.
     * @param description Descripción.
     * @param type Tipo de ejercicio.
     * @param targetMuscle Grupo muscular.
     * @param series Series.
     * @param repetitions Repeticiones.
     * @param duration Duración.
     * @param weightPercentage Porcentaje de peso.
     * @param restTime Tiempo de descanso.
     */
    ExerciseSummary(int idEx, int idWorkout, QString name, QString description, QString type, QString targetMuscle,
                    int series = -1, int repetitions = -1, int duration = -1,
                    float weightPercentage = -1, int restTime = -1);

    /**
     * @brief Operador de igualdad basado en el ID del ejercicio.
     * @param other Objeto a comparar.
     * @return true si los IDs coinciden.
     */
    bool operator==(const ExerciseSummary& other) const;

    /// Getters de todos los atributos

    int getIdEx() const;
    QString getName() const;
    QString getType() const;
    QString getTargetMuscle() const;
    QString getDescription() const;
    int getSeries() const;
    int getRepetitions() const;
    int getDuration() const;
    float getWeightPercentage() const;
    int getRestTime() const;

    int getIdWorkout() const;

    /**
     * @brief Establece el ID del workout al que pertenece este ejercicio.
     * @param newIdWorkout ID del workout.
     */
    void setIdWorkout(int newIdWorkout);

signals:
    /**
     * @brief Señal para mostrar mensajes de error en la interfaz gráfica.
     * @param msg Texto del mensaje.
     * @param type Tipo de mensaje (por defecto advertencia).
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    int idEx;
    int idWorkout;
    QString name;
    QString description;
    QString type;
    QString targetMuscle;
    int series = -1;
    int repetitions = -1;
    int duration = -1;
    float weightPercentage = -1;
    int restTime = -1;
};

#endif // EXERCISESUMMARY_H
