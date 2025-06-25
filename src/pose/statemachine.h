/*!
 * \file statemachine.h
 * \brief Declaración de la clase StateMachine.
 *
 * La clase StateMachine coordina la lógica de evaluación y transición entre estados durante la ejecución
 * de un ejercicio. Cada estado contiene restricciones angulares evaluadas sobre los datos biomecánicos
 * proporcionados por las poses capturadas. Además, registra condiciones detectadas y genera un informe
 * estructurado de la sesión (SesionReport).
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QHash>
#include <QLoggingCategory>
#include <QSharedPointer>
#include "state.h"
#include "feedback.h"
#include "workouts/exerciseespec.h"
//#include "workouts/exercise.h"
#include "sesionreport.h"

Q_DECLARE_LOGGING_CATEGORY(StateMachineLog)
/*!
 * \class StateMachine
 * \brief Clase que implementa una máquina de estados para el análisis biomecánico de un ejercicio físico.
 *
 * Gestiona las transiciones entre estados definidos en una especificación de ejercicio (`ExerciseEspec`),
 * evalúa las restricciones de cada estado con los ángulos detectados por cámara, y genera un conjunto
 * de condiciones (`Condition`) que sirven como feedback en tiempo real. Todos los eventos se registran
 * en un `SesionReport`.
 */
class StateMachine
{
public:
    /*!
     * \brief Constructor que recibe una especificación completa del ejercicio.
     * \param espec Objeto `ExerciseEspec` que contiene los estados, transiciones y restricciones del ejercicio.
     */
    StateMachine(QSharedPointer<ExerciseEspec> espec);

    /*!
     * \brief Ejecuta la evaluación del estado actual con los ángulos detectados y decide si realizar una transición.
     *
     * Este método analiza los ángulos actuales desde múltiples vistas (e.g., frontal y lateral), compara con las
     * restricciones del estado activo y genera condiciones asociadas. En función de las condiciones detectadas,
     * puede cambiar de estado, registrar el inicio/fin de repeticiones o series, y actualizar los acumuladores
     * de métricas.
     *
     * \param anglesByView Hash con los ángulos detectados por vista.
     * \param time Timestamp actual en milisegundos.
     * \return Lista de condiciones biomecánicas detectadas.
     */
    QList<Condition> run(QHash<PoseView, QHash<QString, double>> anglesByView, int64_t time);

    /*!
     * \brief Verifica si el ejercicio ha sido completado.
     * \return true si todas las series y repeticiones se han realizado correctamente; false en caso contrario.
     */
    bool isComplete();

    /*!
     * \brief Devuelve el reporte completo de condiciones y métricas generadas durante la sesión.
     * \return Objeto `SesionReport` con información detallada.
     */
    SesionReport getReport();

private:
    int64_t initTime,duration;
    int64_t initRestTime,restTime;
    int64_t initSetTime,initRepTime,initStateTime;
    int series,repetitions;
    SesionReport report;
    bool repComplete;
    bool complete;
    bool firstRep=true;

    int repCount=1;
    int setCount=1;
    int  currentSet = 1;
    int currentRep = 1;
    bool resting=false;
    // bool validRepetitionInProgress = false;
    int previousStateId = -1;
    State currentState;
    State initState;
    //QList<Condition>currentReport;
    QList<State> states;
    QHash<QPair<int,int>,QSet<Condition>> transitionTable;
    QMap<int,QMap<int,QMap<int,QHash<PoseView,QHash<QString, QPair<double, double>>>>>> globalMinMaxByLine;
    QMap<int,QMap<int,QMap<int,QHash<PoseView,QHash<QString, double>>>>> globalAngleOverloads;

                             //!< Reporte de condiciones generadas durante la sesión.

    // /*!
    //  * \brief Inicializa la estructura interna de estados y transiciones a partir del objeto `ExerciseEspec`.
    //  */
    // void buildStateMachine();

    // /*!
    //  * \brief Evalúa si debe realizarse una transición a otro estado en base a las condiciones actuales.
    //  * \param conditions Lista de condiciones detectadas en el estado actual.
    //  */
    // void checkTransition(const QList<Condition>& conditions);
};



#endif // STATEMACHINE_H
