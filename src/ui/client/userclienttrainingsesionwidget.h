#ifndef USERCLIENTTRAININGSESIONWIDGET_H
#define USERCLIENTTRAININGSESIONWIDGET_H

#include <QWidget>
#include <QLoggingCategory>
#include "ui_userclienttrainingsesionwidget.h"
#include "core/appcontroller.h"
#include "ui/main/uitablemodel.h"
#include "workouts/workoutsummary.h"
#include "workouts/exercisesummary.h"
#include "profiles/client.h"

Q_DECLARE_LOGGING_CATEGORY(TrainigSesionBoard)

class UserClientTrainingSesionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserClientTrainingSesionWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~UserClientTrainingSesionWidget();

    void setUser(QSharedPointer<Client> user);
    void updateWorkoutInfo(const WorkoutSummary& summary);
    void populateExerciseTable(const QList<ExerciseSummary>& exercises);

private slots:
    void onAssignedWorkoutSelected(const QModelIndex& index);
    void onStartSesionButtonClicked();

private:
    void loadAssignedWorkouts();
    void loadCurrentWorkoutData();

    Ui_UserClientTrainingSesionWidget *ui;
    QSharedPointer<Client> currentUser;
    QSharedPointer<AppController> controller;

    UiTableModel* assignedModel;
    UiTableModel* exerciseModel;

    int selectedExerciseId = -1;
};

#endif // USERCLIENTTRAININGSESIONWIDGET_H
