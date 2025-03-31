#ifndef POSEMANAGER_H
#define POSEMANAGER_H

#include <QMainWindow>
#include <QLoggingCategory>
#include <QProcess>
#include <QVector>
#include <QPair>
#include <QTimer>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include "pose/pose.h"
//#include "poseanalyzer.h"
#include "pose/statemachine.h"
#include "pose/feedback.h"
#include <QDateTime>
#include <qthread.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <QCoreApplication>
#include <qfile.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "workouts/trainingSesion.h"
#include "QMainWindow.h"
#include "pose/StateMachine.h"

Q_DECLARE_LOGGING_CATEGORY(PoseManagerLog)

class PoseManager : public QMainWindow {
    friend class TestPoseManager;
    Q_OBJECT

public:
    explicit PoseManager(StateMachine& poseAnalyzer,QWidget *parent = nullptr, bool dualMode = false,bool test=false);
    ~PoseManager();

    void init(TrainingSesion sesion);


private:
    QProcess *process_cam1, *process_cam2;
    //unsigned char* shm_1, shm_2;
    char* shm_1;
    char* shm_2;
    bool dualMode;
    bool running;
    QTimer *timer = nullptr;
    int WIDTH, HEIGHT, FRAME_SIZE, JSON_SIZE, FLAG_SIZE,TOTAL_SIZE;
    QString CAM_1,CAM_2,SEM_SHM1,SEM_SHM2;
    QString pythonScript;
    sem_t* sem1,*sem2;
    int shm_fd1, shm_fd2;

    StateMachine& poseAnalyzer;
    //PoseAnalyzer poseAnalyzer;

    void loadConfig();
    void startPythonProcesses();
    bool connectSharedMemory();
    void stopPythonProcesses();
    void releaseMemory();
    Pose* getNewPose(QString cam_name,sem_t* sem,uchar *shm_pose, int64_t &lastTimestamp);
    void testShareMemory();
    QVector<QPair<int, int>> connections;


signals:
    void newImage1(cv::Mat image);
    void newImage2(cv::Mat image);
    void feedbackGenerated(FeedBack feedback);

public slots:
    void stopCapture();
    void PythonProccesLogOutput();
    void PythonProccesErrorOutput();
};

#endif // POSEMANAGER_H
