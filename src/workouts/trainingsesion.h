/**
 * @file trainingsesion.h
 * @brief Declaración de la clase TrainingSesion, que representa una sesión de entrenamiento realizada por un cliente.
 *
 * Esta clase encapsula la información asociada a una sesión de ejecución de un ejercicio específico:
 * la fecha, duración, identificadores relevantes, el resumen del ejercicio y el informe generado durante la ejecución.
 */

#ifndef TRAININGSESION_H
#define TRAININGSESION_H

#include <QDateTime>
#include <QElapsedTimer>
#include "pose/sesionreport.h"
#include "exerciseespec.h"

/**
 * @class TrainingSesion
 * @brief Clase que representa una sesión de entrenamiento ejecutada por un cliente.
 *
 * Contiene los datos esenciales como identificador, cliente, fecha, resumen del ejercicio realizado,
 * duración y el informe detallado generado por el sistema de análisis de movimiento (`SesionReport`).
 */
class TrainingSesion
{
public:
    /**
     * @brief Constructor principal de la sesión.
     * @param ex Puntero al resumen del ejercicio realizado.
     * @param date Fecha y hora de ejecución.
     * @param idClient ID del cliente que realiza la sesión.
     * @param IdSesion (opcional) Identificador único de la sesión.
     */
    TrainingSesion(QSharedPointer<ExerciseSummary> ex, QDateTime date, int idClient, int IdSesion = -1);

    /// Acceso al informe detallado de ejecución
    SesionReport getReport() const;
    void setReport(const SesionReport &newReport);

    /// Duración total de la sesión (en milisegundos)
    qint64 getDuration() const;
    void setDuration(qint64 newDuration);

    /// Identificador único de la sesión
    int getIdSesion() const;
    void setIdSesion(int newIdSesion);

    /// ID del cliente que realizó la sesión
    int getIdClient() const;
    void setIdClient(int newIdClient);

    /// Resumen del ejercicio ejecutado
    QSharedPointer<ExerciseSummary> getExerciseSummary() const;
    void setExerciseSummary(QSharedPointer<ExerciseSummary> newEx);

    /// Fecha y hora en la que se ejecutó la sesión
    QDateTime getDate() const;
    void setDate(const QDateTime &newDate);

    /// Indica si la sesión fue marcada como completada
    bool isComplete() const;
    void setComplete(bool newComplete);

    /**
     * @brief Obtiene los datos detallados por serie desde el reporte.
     * @return Estructura anidada con métricas agregadas por serie, repetición, estado y condición.
     */
    QHash<int, QHash<int, QHash<int, QHash<Condition, int>>>> getSeriesData();

    /// ID del plan de entrenamiento al que pertenece el ejercicio
    int getIdWorkout() const;

signals:
    /**
     * @brief Señal para emitir mensajes de error a la interfaz de usuario.
     * @param msg Mensaje a mostrar.
     * @param type Tipo de mensaje (informativo, advertencia, error).
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    int IdSesion;
    int idClient;
    int idWorkout;
    QSharedPointer<ExerciseSummary> ex;
    QDateTime date;
    qint64 Duration;
    SesionReport report;
    bool complete;
};

#endif // TRAININGSESION_H
