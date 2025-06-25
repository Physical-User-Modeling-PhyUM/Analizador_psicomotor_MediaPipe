/**
 * @file trainingworkout.h
 * @brief Declaración de la clase TrainingWorkout, que representa un plan de entrenamiento compuesto por varios ejercicios.
 */

#ifndef TRAININGWORKOUT_H
#define TRAININGWORKOUT_H

#include <QString>
#include "exerciseespec.h"
#include "enums/WorkoutFieldEnum.h"
#include "workouts/workoutsummary.h"
#include <QObject>

/**
 * @class TrainingWorkout
 * @brief Clase que encapsula un plan de entrenamiento con múltiples ejercicios.
 *
 * Permite organizar ejercicios específicos (`ExerciseEspec`) dentro de un entrenamiento, manteniendo
 * atributos como nombre, descripción, autor, duración estimada y funcionalidades para modificar la lista de ejercicios.
 */
class TrainingWorkout : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor por defecto.
     * @param parent Objeto padre de Qt.
     */
    TrainingWorkout(QObject *parent = nullptr);

    /**
     * @brief Constructor que inicializa un entrenamiento a partir de un mapa de campos.
     * @param data Hash con los datos del entrenamiento.
     * @param parent Objeto padre de Qt.
     */
    explicit TrainingWorkout(QHash<WorkoutField, QVariant> &data, QObject *parent = nullptr);

    /// Getters y setters del entrenamiento
    int getIdWorkout() const;
    void setIdWorkout(int newIdWorkout);

    QString getName() const;
    void setName(QString newName);

    QString getDescription() const;
    void setDescription(QString newDescription);

    int getEstimatedDuration() const;
    void setEstimatedDuration(int newEstimatedDuration);

    QString getAuthor() const;
    void setAuthor(const QString &newAuthor);

    /// Gestión de ejercicios dentro del entrenamiento
    QList<QSharedPointer<ExerciseEspec>> getExercises() const;

    QSharedPointer<ExerciseEspec> getExercise(int idEx);
    bool addExercise(QSharedPointer<ExerciseEspec> exercise, int order = -1);
    bool replaceExercise(QSharedPointer<ExerciseEspec> updated);
    bool removeExercise(int idEx);

    /**
     * @brief Convierte el entrenamiento actual a un objeto WorkoutSummary.
     * @return Objeto con los datos resumidos del entrenamiento.
     */
    WorkoutSummary toSummary();

    friend class TestTrainingWorkout;

signals:
    /**
     * @brief Señal utilizada para comunicar errores a la interfaz de usuario.
     * @param msg Texto del mensaje.
     * @param type Tipo de mensaje (por defecto advertencia).
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    int idWorkout;
    QString name;
    QString description;
    QString author;
    int estimatedDuration;
    QList<QSharedPointer<ExerciseEspec>> exercises;
};

#endif // TRAININGWORKOUT_H
