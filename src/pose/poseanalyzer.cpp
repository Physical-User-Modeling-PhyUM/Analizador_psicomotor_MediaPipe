#include "poseanalyzer.h"
#include <cstddef>
#include <QHash>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(PoseAnalyzerLog, "poseAnalyzer")

// Esto debe tener Exercise y EspecExercise como atributo



// PoseAnalyzer::PoseAnalyzer(const ExerciseEspec &espec,const Exercise &ex)
//      : poseComputer(espec.getStatesList(),espec.getTransitionTable(),ex.getDuration(),ex.getRepetitions(),ex.getRestTime())
// {

// }

// FeedBack PoseAnalyzer::analyze(Pose* frontPose, Pose* sidePose)
// {
//     QHash<QString, double> angles;

//     //obtenemos los datos de ángulos de la pose frontal
//     angles=frontPose->getAngles();

//     // si el timestamp coincide agregamos lo
//     if(frontPose->getTimestamp()==sidePose->getTimestamp()){
//         for (auto it = sidePose->getAngles().constBegin(); it != sidePose->getAngles().constEnd(); ++it) {
//             angles.insert(it.key()+"_L", it.value());
//         }
//     }
//     //si no coinciden los timestamp priorizamos la vista frontal
//     QList<Condition> conds= poseComputer.checkTransition(angles,frontPose->getTimestamp());

//     for( Condition &cond: conds){


//     }


//     return FeedBack(conds);
// }
