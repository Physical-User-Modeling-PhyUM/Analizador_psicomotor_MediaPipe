#ifndef DUMMYPOSEMANAGER_H
#define DUMMYPOSEMANAGER_H

#include <QObject>
#include <QDir>
#include <QTextStream>
#include <QSharedPointer>
#include <opencv2/opencv.hpp>
#include "pose/pose.h"
#include "pose/statemachine.h"
#include "pose/feedback.h"
#include "workouts/trainingsesion.h"

class DummyPoseManager : public QObject {
    Q_OBJECT

public:
    explicit DummyPoseManager(QObject *parent = nullptr);
    void init(QSharedPointer<TrainingSesion> sesion, QSharedPointer<ExerciseEspec> espec);
    void setInputFolder(const QString& path);
    void runAnalysis();
    void pauseAnalysis();
    void applyConfiguration(QHash<QString, QVariant>& config,QHash<QPair<int, int>, QString>& conn,
                            QHash<int, QString>& kpts);
    void setConnections(const QHash<QPair<int, int>, QString>& conn);

signals:
    void newImage1(cv::Mat image);
    void feedbackGenerated(FeedBack feedback);
    void exerciseCompleted();


    void newImage2(cv::Mat image);



private:
    QString inputFolder;
    QSharedPointer<TrainingSesion> runningSesion;
    QSharedPointer<StateMachine> poseAnalyzer;
    QHash<QPair<int, int>, QString>connections;

    bool infoMessage=true;
    bool alerts=true;
    bool critical=true;


    int64_t lastTimestamp = 0;

    bool configured=false;
    char* shm_1;
    char* shm_2;
    bool dualMode;
    bool running,runningAnalysis;

    int WIDTH, HEIGHT, FRAME_SIZE, JSON_SIZE, FLAG_SIZE,TOTAL_SIZE;
    QString CAM_1,CAM_2,SEM_SHM1,SEM_SHM2;


    void processFrame(const QString& baseName);

public slots:
    void stopCapture();

};

#endif // DUMMYPOSEMANAGER_H
