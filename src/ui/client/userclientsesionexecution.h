#ifndef USERCLIENTSESIONEXECUTION_H
#define USERCLIENTSESIONEXECUTION_H


#include <QDialog>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <opencv2/core/mat.hpp>
#include "core/appcontroller.h"
#include "pose/feedback.h"
#include "ui/client/feedbackgraphwidget.h"
#include "workouts/trainingsesion.h"
#include "ui_userclientsesionexecution.h"
#include "core/soundfeedbackmanager.h"

class UserClientSesionExecution : public QDialog
{
    Q_OBJECT

public:
    explicit UserClientSesionExecution(QSharedPointer<TrainingSesion> sesion,AppController* controller,
                                       QSharedPointer<SoundFeedbackManager> soundManager,
                                       QWidget *parent = nullptr);
    ~UserClientSesionExecution();

public slots:
    void onNewImage1(const cv::Mat& image);
    void onNewImage2(const  cv::Mat &image);
    void onFeedbackReceived(const  FeedBack& feedback);



private slots:

    void on_ReadyButon_clicked();
    void on_pauseButton_clicked();
    void on_EndSesionButton_clicked();




    void on_criticalButton_clicked();

    void on_alertsButton_clicked();

    void on_infoButton_clicked();

    void on_muteButton_clicked();

    void on_volumeDownButton_clicked();

    void on_volumeUpButton_clicked();

private:
    Ui_UserClientSesionExecution *ui;
    AppController* controller;
    QSharedPointer<SoundFeedbackManager> soundManager;
    QSharedPointer<TrainingSesion> currentSesion;
    QTimer* sessionTimer;
    FeedBackGraphWidget* graphWidget;
    QSharedPointer<ExerciseSummary> sum;
    int elapsedSeconds;
    bool inRest;
    int restTime;
    bool inExecution;
    int executionTime;
    int maxReps;
    int maxSets;
    int maxDuration;
    int maxRestTime;

    QHash<ConditionType, int> lastConditionTime;
    int minConditionInterval = 3;
    bool alertSoundEnabled = true;
    bool infoSoundEnable=true;
    bool criticaSoundEnable=true;
    bool mute=false;
    qreal currentVolume = 0.8;

    void updateTimeLabel();
    void appendFeedbackMessages(const FeedBack& feedback);
    bool fireEffect(ConditionType type);
};

#endif // USERCLIENTSESIONEXECUTION_H
