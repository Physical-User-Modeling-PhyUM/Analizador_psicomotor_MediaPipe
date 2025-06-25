#ifndef TRAINERMETRICSMAINWIDGET_H
#define TRAINERMETRICSMAINWIDGET_H

#include <QWidget>
#include "core/appcontroller.h"
#include "core/metricsmanager.h"
#include "pose/sesionreport.h"
//#include "ui/metrics/conditiondistributionchart.h"
#include "ui/metrics/heatmapwidget.h"
#include "ui/metrics/timewidget.h"
#include "ui_trainermetricsmainwidget.h"
#include "ui/metrics/angleviewwidget.h"
#include "ui/metrics/logviewerwidget.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(trainerMetricsWidget);


class TrainerMetricsMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerMetricsMainWidget(QSharedPointer<AppController> ctrl,QWidget *parent = nullptr);
    ~TrainerMetricsMainWidget();

    void setReport(const SesionReport &report);

    void populateTimeWidget();
    void populateConditionDistributionChart();
    void updateWidgets();
    void populateHeatmapWidget();
private slots:
    void onClienteSelected(int row);
    void onWorkoutSelected(int row);
    void onSesionSelected(int row);
    void onExerciseSelected(int row);
    void onExercisesTableClicked(const QModelIndex &index);
    void onClienteTableClicked(const QModelIndex &index);
    void onWorkoutTableClicked(const QModelIndex &index);
    void onSesionTableClicked(const QModelIndex &index);
    void onSerieChanged(int index);

private:
    Ui_TrainerMetricsMainWidget *ui;
    QSharedPointer<MetricsManager> metricsManager;
     QSharedPointer<AppController> controller;
    TimeWidget* timeWidget = nullptr;
    HeatmapWidget* heatmapWidget = nullptr;
    //ConditionDistributionChart* conditionChart = nullptr;
    AngleViewerWidget* angleViewer = nullptr;
    LogViewerWidget* logViewer = nullptr;


    void loadExercisesForWorkout(int idWorkout);

    int currentClientId   = -1;
    int currentWorkoutId  = -1;
    int currentExerciseId = -1;


    void loadClientes();
    void loadWorkoutsForClient(int idClient);

    void loadAvailableSesions(int idWorkout, int idClient, int idEx);


};

#endif // TRAINERMETRICSMAINWIDGET_H
