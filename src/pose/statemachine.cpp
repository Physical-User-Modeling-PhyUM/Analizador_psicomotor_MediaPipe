/*!
 * \file statemachine.cpp
 * \brief Implementación de la clase StateMachine.
 *
 * Esta clase coordina la lógica de transición entre estados durante la ejecución de un ejercicio, evaluando condiciones
 * biomecánicas a partir de los ángulos detectados y generando reportes que se almacenan en la instancia de SesionReport.
 */

#include "statemachine.h"

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(StateMachineLog, "stateMachine")

// StateMachine::StateMachine(QList<State> states, QHash<QPair<int, int>, QSet<Condition>> transitionTable,int series,int duration, int repetition,int restTime)
//     : states(states), transitionTable(transitionTable), currentState(states.first()),series(series),duration(duration),repetitions(repetition),restTime(restTime)
// {
//     qDebug(StateMachineLog) << "Se ha creado la maquina de estados";
// }
/*!
 * \brief Constructor que inicializa la máquina de estados a partir de una especificación de ejercicio.
 *
 * Configura los parámetros del ejercicio (series, repeticiones, tiempos) y los estados con sus restricciones.
 * También construye la tabla de transiciones entre estados en función de las condiciones especificadas.
 *
 * \param espec Puntero compartido a la especificación del ejercicio.
 */
StateMachine::StateMachine(QSharedPointer<ExerciseEspec> espec)
    :states(espec->getStatesList()),
    transitionTable(espec->getTransitionTable()),
    currentState(espec->getStatesList().first()),
    series(espec->getSeries()),
    duration((espec->getDuration())*1000),// el valor está en segundos
    repetitions(espec->getRepetitions()),
    restTime((espec->getRestTime())*1000),//el valor está en  segundos
    complete(false)

{
    qDebug(StateMachineLog) << "Se ha creado la maquina de estados";
    for(State i:states){
        if (i.getId()==0) initState=i;
    }
    //se le agrega el Error State
    states.append(State());
    //ordenamos los estados por Id
    std::sort(states.begin(), states.end());

    initSetTime=0,
    initRepTime=0;
    initStateTime=0;
}


/*!
 * \brief Ejecuta un ciclo de la máquina de estados con los ángulos detectados por vista (frontal y lateral).
 *
 * Evalúa el estado actual con los ángulos proporcionados por la visión por computadora, actualiza los acumuladores
 * de métricas (rangos de movimiento, sobrecargas), y detecta condiciones. Gestiona transiciones entre estados,
 * control de repeticiones y series, y períodos de descanso.
 *
 * \param anglesByView Mapa de vistas (PoseView) a los ángulos detectados (línea, valor).
 * \param time Timestamp actual en milisegundos.
 * \return Lista de condiciones generadas en este ciclo.
 */
//QList<Condition> StateMachine::run(QHash<QString, double> detectedAngles, int64_t time){
QList<Condition> StateMachine::run(QHash<PoseView, QHash<QString, double>> anglesByView, int64_t time) {

    repComplete = false;
    complete = false;
    QList<Condition> currentReport;
    currentReport.clear();

    //Durante el tiempo de descanso no se realizará ningun análisis, si se incluye un tiempo este sera respetado siempre

    // if ( !hasEmittedRestTime && initRestTime != -1 && restTime != -1 && time - initRestTime > restTime){
    //     currentReport.append(Condition(ConditionType::RestTime,"",restTime));
    //     //resting=false;
    //     hasEmittedRestTime = true;
    // }

    if (resting) {
        if (initRestTime != -1 && restTime != -1 && (time - initRestTime) > restTime) {
            resting = false;
             currentReport.append(Condition(ConditionType::RestTime,"",restTime));
             hasEmittedRestTime = true;

        } else {
            return currentReport;
        }
    }
     int currentId = currentState.getId();

    // QHash<QString, QPair<double, double>> MinMaxByLine=globalMinMaxByLine[currentSet][currentRep][currentId];
    // QHash<QString, double>>>>
    for (PoseView view : anglesByView.keys()) {
        QList<Condition> partial = currentState.getReport(anglesByView[view],
                                                          time,
                                                          view,
                                                          globalMinMaxByLine[currentSet][currentRep][currentId][view],
                                                          globalAngleOverloads[currentSet][currentRep][currentId][view]);
        currentReport.append(partial);
    }



    // Obtenermos el índice del estado actual por ID
    int indexCurrent = -1;
    for (int i = 0; i < states.size(); ++i) {
        if (states[i].getId() == currentId) {
            indexCurrent = i;
            break;
        }
    }

    int indexNextState = indexCurrent + 1;
    if (indexNextState >= states.size()) {
        // saltamos estado -1 (error), pasamos al estado inicial
        indexNextState = 1;
    }
    State nextState = states.at(indexNextState);

    // Verificamos si la transición al siguiente estado está definida
    QPair<int, int> transition(currentId, nextState.getId());
    QPair<int, int> transitionError(currentId, -1);

    QSet<Condition> requiredConditions;


    // Intentamos por las condiciones para ir al estado siguiente
    bool transitioned = false;
    if (transitionTable.contains(transition)) {
        requiredConditions = transitionTable.value(transition);
        bool allConditionsMet = true;

        QString conds;
        for(Condition c:requiredConditions){ conds.append(conditionTypeToString(c.type)+",");}
        qDebug(StateMachineLog) <<"Condiciones a  cumplir en el estado: "<<currentId<<", "<<conds;

        for (const Condition &cond : requiredConditions) {

            // qDebug(StateMachineLog) << "Verificando condición requerida:" << conditionTypeToString(cond.type)
            //                         << "línea:" << cond.keypointLine << "view:" << PoseViewToString(cond.view);
            for (const Condition &c : currentReport) {
                qDebug(StateMachineLog) << ">> presente en reporte:" << conditionTypeToString(c.type)
                << "línea:" << c.keypointLine << "view:" << PoseViewToString(c.view);
            }

            if (!currentReport.contains(cond)) {
                allConditionsMet = false;
                qDebug(StateMachineLog) << "CONDICIÓN NO CUMPLIDA:" << conditionTypeToString(cond.type)
                                        << cond.keypointLine << PoseViewToString(cond.view);
                break;
            }
        }

        if (allConditionsMet) {
            int previousStateId=currentState.getId();
            currentState = nextState;
            currentState.setEntryTime(time);
            transitioned = true;
            //se realiza la transición
            qDebug(StateMachineLog) << "Transición de estado" << currentId << "->" << currentState.getId();

            //initStateTime=time;
            // QString conds;
            // for(Condition c:requiredConditions){ conds.append(conditionTypeToString(c.type)+",");}
            //                          qDebug(StateMachineLog) <<"Condiciones cumplidas"<<conds;
            // Inicio de ejercicio
            if (firstRep && currentState.getId() == 1 ) {
                currentReport.append(Condition(ConditionType::exerciseInit,"",time));
                firstRep=false;
                initTime = time;
                initSetTime=time;
                initStateTime=time;
                initRepTime=time;
                initRestTime = 0;
                resting=false;

                //validRepetitionInProgress = true;
                qDebug(StateMachineLog) << "Inicio del ejercicio.";
            }
            // Repetición completada
            if (!resting && !firstRep &&
                currentState.getId() == 0 &&
                previousStateId == states.last().getId())
            {
                repCount++;
                currentReport.append(Condition(ConditionType::EndOfRepetition,QString::number(repCount),time-initRepTime ));
                qDebug(StateMachineLog) << "Repetición completada. Reps restantes:" << repetitions;
                initRepTime=0;
                //Serie completada
                if (repCount > repetitions) {
                    setCount++;
                    repCount=1;
                    initRestTime = time;
                    //initTime = -1;

                    repComplete = true;
                    currentReport.append(Condition(ConditionType::EndOfSet,QString::number(setCount),time-initSetTime));
                    qDebug(StateMachineLog) << "Serie completada. Series restantes:" << series;
                    resting=true;
                    initSetTime=0;
                    initRepTime=0;
                    hasEmittedInitSet=false;
                    initRestTime = time;
                    //firstRep=true;
                }
                if ( setCount>series) {
                    complete = true;
                    currentReport.append(Condition(ConditionType::EndOfExercise,"",time-initTime));
                    qDebug(StateMachineLog) << "Ejercicio completado.";

                    resting=true;
                   }
                // if (resting==true && initRestTime != -1 && restTime != -1 && time - initRestTime < restTime){
                //     currentReport.append(Condition(ConditionType::RestTime,"",restTime-(time-initRestTime)));

                // }
            }else if (currentState.getId() == 1 && previousStateId != states.last().getId()) {
                qDebug(StateMachineLog) << "Volvió a estado 1 pero sin pasar por todos los estados: repetición NO contada";
            }
            currentReport.append(Condition(ConditionType::EndOfMovementPhase,QString::number(currentId),(time-initStateTime)));


            // Emitimos InitSet si entramos al estado 1, es la primera repetición y venimos de un descanso
            if (!hasEmittedInitSet && currentState.getId() == 1 && repCount == 1 /*&& resting*/) {
                currentReport.append(Condition(ConditionType::InitSet, QString::number(setCount), time));
                qDebug(StateMachineLog) << "Inicio de nueva serie:" << setCount << ": condición InitSet generada.";
                hasEmittedInitSet=true;
                initSetTime=time;
                initRepTime=time;
                if (!firstRep && hasEmittedRestTime) {
                    currentReport.append(Condition(ConditionType::RestOverTime, "", (time - initRestTime) - restTime));
                    qDebug(StateMachineLog) << "Descanso excedido: RestOverTime generado con " << (time - initRestTime - restTime) << " ms adicionales.";
                    hasEmittedRestTime=false;
                }
            }

            // Emitimos InitRepetition cada vez que se entra a estado 1
            if (currentState.getId() == 1) {
                currentReport.append(Condition(ConditionType::InitRepetition, QString::number(repCount), time));
                qDebug(StateMachineLog) << "Inicio de repetición:" << repCount << ": condición InitRepetition generada.";
                //initTime=time;
                initRepTime=time;
                initRestTime=0;
            }

            // Si estamos en estado 1 y estábamos en descanso, salimos del modo resting
            if (resting && currentState.getId() == 1) {

                resting = false;
                initRestTime = 0;
                hasEmittedRestTime = false;
                qDebug(StateMachineLog) << "Movimiento detectado: saliendo del modo descanso.";
            }

            initStateTime=time;
        }
    }

    // Si no se pudo hacer transición normal, comprobamos si se cumplen las condiciones de error
    if (!transitioned && transitionTable.contains(transitionError)) {
        requiredConditions = transitionTable.value(transitionError);
        bool allConditionsMet = true;

        for (const Condition &cond : requiredConditions) {
            if (!currentReport.contains(cond)) {
                allConditionsMet = false;
                break;
            }
        }

        if (allConditionsMet) {
            currentReport.append(Condition(ConditionType::IncorrectExecution, QString::number(currentId)));
            currentState = initState;
            //validRepetitionInProgress = false;
            qDebug(StateMachineLog) << "Transición a estado de error desde" << currentId;
        }
    }

    // Evaluamos las condiciones temporales de los estados





    // if (!firstRep && hasEmittedRestTime && (initRestTime != -1 && restTime != -1 && time - initRestTime > restTime)){
    //     currentReport.append(Condition(ConditionType::RestOverTime,"",(time-initRestTime)-restTime));
    //     //resting=false;
    // }

    if (initTime != -1 && duration != -1 && time - initSetTime > duration)
        currentReport.append(Condition(ConditionType::SetTime, "", (time-initSetTime)-duration));

    // Añadir al reporte acumulado
     qDebug(StateMachineLog) <<"==Report==";
    for (const Condition &cond : currentReport) {
        report.addCondition(currentSet, currentRep, currentId,cond);
        qDebug(StateMachineLog) << "Serie" << setCount
                                << "Reps" << repCount
                                << "View:" << PoseViewToString(cond.view)
                                << "Line:" << cond.keypointLine
                                << "Condition:" << conditionTypeToString(cond.type)
                                << "valor"<<cond.value.toString();
    }
    qDebug(StateMachineLog) <<"====";
    currentSet=setCount;
    currentRep=repCount;
    return currentReport;
}

/*!
 * \brief Indica si el ejercicio ha sido completado.
 *
 * Retorna verdadero si todas las repeticiones y series han sido ejecutadas correctamente
 * y se ha alcanzado el estado final según la configuración.
 *
 * \return true si el ejercicio ha terminado; false en caso contrario.
 */
bool StateMachine::isComplete()
{
    return complete;
}
/*!
 * \brief Devuelve el reporte completo generado por la ejecución de la máquina de estados.
 *
 * El objeto SesionReport incluye las condiciones detectadas clasificadas por serie, repetición y estado.
 * También incorpora métricas agregadas como rangos angulares y cargas articulares.
 *
 * \return Reporte completo de la sesión.
 */
SesionReport StateMachine::getReport()
{
    report.setLineAngleRange(globalMinMaxByLine);
    report.setGlobalAngleOverloads(globalAngleOverloads);
    return report;
}

void StateMachine::newSerie() {
    setCount++;
    repCount = 1;

    currentState = initState;
    currentState.setEntryTime(QDateTime::currentMSecsSinceEpoch());

    initTime = -1;
    initSetTime = QDateTime::currentMSecsSinceEpoch();
    initStateTime = initSetTime;
    initRestTime = initSetTime;

    resting = true;
    hasEmittedRestTime = false;

    repComplete = false;
    complete = false;

    qDebug(StateMachineLog) << "[Interrupción] Serie interrumpida manualmente. Se inicia nueva serie #" << setCount;
}
