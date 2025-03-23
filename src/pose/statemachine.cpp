#include "statemachine.h"

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(StateMachineLog, "stateMachine")

// StateMachine::StateMachine(QList<State> states, QHash<QPair<int, int>, QSet<Condition>> transitionTable,int series,int duration, int repetition,int restTime)
//     : states(states), transitionTable(transitionTable), currentState(states.first()),series(series),duration(duration),repetitions(repetition),restTime(restTime)
// {
//     qDebug(StateMachineLog) << "Se ha creado la maquina de estados";
// }

StateMachine::StateMachine(const ExerciseEspec &espec,const Exercise &ex)
    :states(espec.getStatesList()),
    transitionTable(espec.getTransitionTable()),
    currentState(espec.getStatesList().first()),
    series(ex.getSeries()),
    duration(ex.getDuration()),
    repetitions(ex.getRepetitions()),
    restTime(ex.getRestTime()),
    complete(false)

{
    qDebug(StateMachineLog) << "Se ha creado la maquina de estados";
}

QList<Condition> StateMachine::run(QHash<QString, double> detectedAngles, int64_t time)
{
    // Obtener el reporte de condiciones del estado actual
    QList<Condition> currentReport = currentState.getReport(detectedAngles, time);

    QPair<int, int> transition;
    QSet<Condition> requiredConditions;

    // Iterar sobre cada transición posible desde el estado actual
    for (auto it = transitionTable.begin(); it != transitionTable.end(); ++it) {

        transition = it.key();
        requiredConditions = it.value();

        // Verificamos si la transición inicia en el estado actual
        if (transition.first == currentState.getId()) {
            bool allConditionsMet = true;

            // Verificamos si todas las condiciones requeridas están en el reporte actual
            for (const Condition &requiredCond : requiredConditions) {
                if (!currentReport.contains(requiredCond)) {
                    allConditionsMet = false;
                    break;
                }
            }


            if (allConditionsMet) {
                for (State &nextState : states) {
                    // Si todas las condiciones se cumplen,se realiza la transición
                    if (nextState.getId() == transition.second) {
                        qDebug(StateMachineLog) << "Transición de estado" << currentState.getId() << "->" << nextState.getId();
                        currentState = nextState;
                        //Si pasamos del primer estado (condición de reposo) al segundo, se ha iniciado la repeticion
                        if(currentState.getId()==2){
                            currentReport.append(Condition(ConditionType::exerciseInit,"" ));
                            initTime=time;
                            initRestTime=-1;
                        }
                        //Si el estado al que se cambia es el primero, se ha realizado una repetición
                        if(currentState.getId()==1){currentReport.append(Condition(ConditionType::EndOfRepetition,"" ));}
                        repetitions--;
                        //si llegamos a cumplimentar todas las repeticiones se ha realizado una serie, se inicia tiempo de descanso
                        if (repetitions==0){
                            initRestTime=time;
                            initTime=-1;
                            series--;
                            currentReport.append(Condition(ConditionType::EndOfSet,"" ));
                        }
                        if(series==0){
                            currentReport.append(Condition(ConditionType::EndOfExercise,"" ));
                            complete=true;
                        };
                        break; // Salimos si se produce una transición
                    }
                }

            }
        }
    }
    // Si la diferencia entre el tiempo actual y el de inicio del descaso es mayor, se avisa
    if (initRestTime!=-1 && time-initRestTime>restTime) {currentReport.append(Condition(ConditionType::RestOverTime,"" ));};
    if (initTime!=-1 && time-initTime>duration){currentReport.append(Condition(ConditionType::SetTime,"" ));};

    //Agregamos las condiciones al reporte de ejecucion
    for(const Condition &cond : currentReport) {report.addCondition(repetitions, cond.keypointLine, cond.type);}

    return currentReport;
}

bool StateMachine::isComplete()
{
    return complete;
}

SesionReport StateMachine::getReport()
{
    return report;
}
