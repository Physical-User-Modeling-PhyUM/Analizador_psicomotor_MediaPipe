#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QSet>
#include <QHash>
#include <QPair>
#include "state.h"
#include "workouts/exercise.h"
#include "workouts/exerciseespec.h"
#include "sesionreport.h"


Q_DECLARE_LOGGING_CATEGORY(StateMachineLog);

class StateMachine
{
public:
    //explicit StateMachine(QList<State> states, QHash<QPair<int, int>, QSet<Condition>> transitionTable,int series=-1, int repetition=-1,int duration=-1,int restTime=-1);
    explicit StateMachine(const ExerciseEspec &espec,const Exercise &ex);
    QList<Condition> run(QHash<QString, double> detectedAngles,int64_t time);
    bool isComplete();
    SesionReport getReport();

private:
    int64_t initTime,duration;
    int64_t initRestTime,restTime;
    int series,repetitions;
    SesionReport report;
    bool complete;


    State currentState;
    //QList<Condition>currentReport;
    QList<State> states;
    QHash<QPair<int,int>,QSet<Condition>> transitionTable;


};

#endif // STATEMACHINE_H
