/**
 * @file exerciseespec.h
 * @brief Declaración de la clase ExerciseEspec, que modela una especificación completa de un ejercicio físico, incluyendo su estructura de estados y transiciones.
 */

#ifndef EXERCISEESPEC_H
#define EXERCISEESPEC_H

#include <QPair>
#include <QSet>
#include "pose/state.h"
#include "enums/ExEspecFieldEnum.h"
#include <QVariant>
#include <QMap>
#include "workouts/exercisesummary.h"
#include <QLoggingCategory>
#include <QObject>

Q_DECLARE_LOGGING_CATEGORY(ExerciseSpec)

/**
 * @class ExerciseEspec
 * @brief Representa una especificación detallada de un ejercicio físico incluyendo su configuración y máquina de estados.
 *
 * Contiene atributos de planificación (nombre, descripción, repeticiones, series, etc.), así como
 * un conjunto de `State` que representan fases del ejercicio, y una tabla de transiciones condicionadas entre estados.
 */
class ExerciseEspec : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor por defecto.
     * @param parent Objeto padre de Qt.
     */
    explicit ExerciseEspec(QObject *parent = nullptr);

    /**
     * @brief Constructor que inicializa la especificación a partir de un mapa de campos.
     * @param data Mapa de valores del ejercicio, indexado por ExEspecField.
     * @param parent Objeto padre de Qt.
     */
    explicit ExerciseEspec(QHash<ExEspecField, QVariant> data, QObject *parent = nullptr);

    /// Métodos para acceder y modificar estados
    QList<State> getStatesList() const;
    QSet<State> getStates() const;
    State getState(int idState) const;
    bool updateState(State state);
    void addState(State state);
    void delState(int id);

    /// Métodos para acceder y modificar transiciones
    QHash<QPair<int, int>, QSet<Condition>> getTransitionTable() const;
    void addTransition(QPair<int, int> estados, QSet<Condition> condiciones);
    void addTransition(QPair<int, int> estados, const Condition& condicion);
    void delTransition(QPair<int, int> estados, const Condition& condicion);
    void setTransitionTable(const QHash<QPair<int, int>, QSet<Condition>>& newTransitionTable);

    /// Conversión a resumen
    ExerciseSummary toSummary() const;

    /// Getters y setters de campos de planificación del ejercicio
    int getIdEx() const;
    void setIdEx(int newIdEx);

    QString getName() const;
    void setName(const QString &newName);

    QString getDescription() const;
    void setDescription(const QString &newDescription);

    QString getExersiseType() const;
    void setExersiseType(const QString &newExersiseType);

    QString getTargetMuscle() const;
    void setTargetMuscle(const QString &newTargetMuscle);

    QString getEquipment() const;
    void setEquipment(const QString &newEquipment);

    int getSeries() const;
    void setSeries(int newSeries);

    int getRepetitions() const;
    void setRepetitions(int newRepetitions);

    int getDuration() const;
    void setDuration(int newDuration);

    float getWeightPercentage() const;
    void setWeightPercentage(float newWeight);

    int getRestTime() const;
    void setRestTime(int newRestTime);

    void setStates(const QSet<State> &newStates);

    friend class TestExerciseEspec;

signals:
    /**
     * @brief Señal que emite un mensaje de error para mostrar en la UI.
     * @param msg Texto del mensaje.
     * @param type Tipo del mensaje (advertencia, información, etc.).
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    int idEx;
    QString name = "not defined";
    QString description;
    QString exersiseType = "not defined";
    QString targetMuscle = "not defined";
    QString Equipment = "not defined";
    int series = -1;
    int repetitions = -1;
    int duration = -1;
    float weightPercentage = -1;
    int restTime = -1;

    QSet<State> states;
    QHash<QPair<int, int>, QSet<Condition>> transitionTable;
};

#endif // EXERCISEESPEC_H
