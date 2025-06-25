/**
 * @file trainingsesion.cpp
 * @brief Implementación de la clase TrainingSesion.
 */

#include "trainingsesion.h"

/**
 * @brief Constructor principal de la sesión.
 */
TrainingSesion::TrainingSesion(QSharedPointer<ExerciseSummary> ex,QDateTime date,int idClient,int IdSesion)
    :ex(ex),IdSesion(IdSesion),idClient(idClient),date(date)
{
    idWorkout=ex->getIdWorkout();
}
/**
 * @brief Devuelve el informe detallado de la sesión (`SesionReport`).
 */
SesionReport TrainingSesion::getReport() const
{
    return report;
}
/**
 * @brief Establece el informe de la sesión. Asocia su ID a esta sesión.
 */
void TrainingSesion::setReport(const SesionReport &newReport)
{
    report.setIdSesion(IdSesion);
    report = newReport;
}
/**
 * @brief Devuelve la duración total de la sesión en milisegundos.
 */
qint64 TrainingSesion::getDuration() const
{
    return Duration;
}
/**
 * @brief Establece la duración total de la sesión.
 */
void TrainingSesion::setDuration(qint64 newDuration)
{
    Duration = newDuration;
}
/**
 * @brief Devuelve el ID único de la sesión.
 */
int TrainingSesion::getIdSesion() const
{
    return IdSesion;
}
/**
 * @brief Asigna el ID único de la sesión.
 */
void TrainingSesion::setIdSesion(int newIdSesion)
{
    IdSesion = newIdSesion;
}
/**
 * @brief Devuelve el ID del cliente que realizó la sesión.
 */
int TrainingSesion::getIdClient() const
{
    return idClient;
}
/**
 * @brief Establece el ID del cliente asociado.
 */
void TrainingSesion::setIdClient(int newIdClient)
{
    idClient = newIdClient;
}
/**
 * @brief Devuelve el resumen del ejercicio ejecutado en esta sesión.
 */
QSharedPointer<ExerciseSummary> TrainingSesion::getExerciseSummary() const
{
    return ex;
}
/**
 * @brief Establece un nuevo ejercicio asociado a la sesión.
 */
void TrainingSesion::setExerciseSummary(QSharedPointer<ExerciseSummary> newEx)
{
    ex = newEx;
}
/**
 * @brief Devuelve la fecha y hora de la sesión.
 */
QDateTime TrainingSesion::getDate() const
{
    return date;
}
/**
 * @brief Establece la fecha de ejecución de la sesión.
 */
void TrainingSesion::setDate(const QDateTime &newDate)
{
    date = newDate;
}

/**
 * @brief Indica si la sesión se marcó como completada.
 */
bool TrainingSesion::isComplete() const
{
    return complete;
}
/**
 * @brief Marca la sesión como completada o no.
 */
void TrainingSesion::setComplete(bool newComplete)
{
    complete = newComplete;
}

/**
 * @brief Devuelve los datos de análisis detallado por serie, repetición y condición.
 */
QHash<int, QHash<int, QHash<int, QHash<Condition, int> > > > TrainingSesion::getSeriesData()  {
    return report.getSeriesData();
}

/**
 * @brief Devuelve el ID del workout al que pertenece el ejercicio de esta sesión.
 */
int TrainingSesion::getIdWorkout() const
{
    return idWorkout;
}
