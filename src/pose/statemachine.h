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
#include <QDateTime>
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
    /**
     * @brief Interrumpe la serie actual de forma manual y fuerza el inicio de una nueva serie.
     *
     * Este método permite interrumpir el flujo natural de la máquina de estados,
     * sumando una serie (setCount++), reiniciando el contador de repeticiones (repCount = 1),
     * y posicionando el estado actual en el estado inicial (`initState`).
     *
     * También reinicia los temporizadores relevantes (`initTime`, `initSetTime`, `initRestTime`)
     * y activa el modo descanso (`resting = true`) para que el sistema espere el inicio
     * de la nueva serie de forma controlada.
     *
     * Este método está pensado para ser invocado desde la interfaz de usuario o un controlador externo,
     * por ejemplo, cuando el usuario desee finalizar manualmente una serie incompleta.
     *
     * No emite ninguna condición (`Condition`) directamente, pero deja el sistema en un estado coherente
     * para que en la siguiente ejecución de `run()` se genere `InitSet` y `InitRepetition` al detectar movimiento.
     */
    void newSerie();
private:
    int64_t initTime,duration;
    int64_t initRestTime,restTime;
    int64_t initSetTime,initRepTime,initStateTime;
    int series,repetitions;
    SesionReport report;
    bool repComplete;
    bool complete;
    bool firstRep=true;
    bool hasEmittedRestTime = false;

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



};



#endif // STATEMACHINE_H
