#ifndef POSEANALYZER_H
#define POSEANALYZER_H

#include "pose.h"
#include "feedback.h"
#include "statemachine.h"
#include "sesionreport.h"
#include "workouts/exercise.h"
#include "workouts/exerciseespec.h"

Q_DECLARE_LOGGING_CATEGORY(PoseAnalyzerLog);

class PoseAnalyzer
{
public:

    explicit PoseAnalyzer(const ExerciseEspec &espec,const Exercise &ex);
    FeedBack analyze(Pose* frontPose, Pose* sidePose);
    bool isComplete();
    SesionReport getReport();


private:

    StateMachine poseComputer;
    SesionReport report;
signals:
    void feedbackGenerated(const FeedBack& feedback);

};

#endif // POSEANALYZER_H
