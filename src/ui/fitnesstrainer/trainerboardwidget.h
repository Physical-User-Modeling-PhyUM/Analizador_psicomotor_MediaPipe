#ifndef TRAINERBOARDWIDGET_H
#define TRAINERBOARDWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QPointer>
#include <QLoggingCategory>

#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "ui/fitnesstrainer/trainerinfowidget.h"
#include "ui/fitnesstrainer/trainerclientinfowidget.h"
#include "ui/fitnesstrainer/trainerworkoutdesignwidget.h"
#include "ui/fitnesstrainer/trainerworkoutassignwidget.h"
#include "ui/fitnesstrainer/trainermetricsmainwidget.h"
#include "ui_trainerboardwidget.h"
#include <QQueue>

Q_DECLARE_LOGGING_CATEGORY(TrainerBoard)

class TrainerBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~TrainerBoardWidget();

    void setUser(QSharedPointer<FitnessTrainer> user);
    bool isInBuffer(QSharedPointer<Client> client) const;
    bool isInBuffer(int id) const;
    QSharedPointer<Client> getFromBuffer(int id) const;
    void addToBuffer(QSharedPointer<Client> client);

private slots:
    void sectionChanged(int index);
    void onUiErrorMessage(const QString& msg, QtMsgType type);
    void populateMetricsTable();
signals:
    void logoutRequested();
private:
    Ui_TrainerBoardWidget *ui;
    QSharedPointer<AppController> controller;
    QSharedPointer<FitnessTrainer> currentTrainer;
    QList< QSharedPointer<TrainingWorkout>> buffer;

    QPointer<TrainerInfoWidget>infoWidget ;
    QPointer<TrainerClientInfoWidget> infoClientWidget;
    QPointer<TrainerMetricsMainWidget> metricsWidget;
    QPointer<TrainerWorkoutDesignWidget> workoutDesignWidget;
    QPointer<TrainerWorkoutAssignWidget> assignWidget;

    QHash<int, QSharedPointer<Client>> clientBuffer;
    QQueue<int> bufferOrder;

    void updateTrainerInfo();
};

#endif // TRAINERBOARDWIDGET_H
