#ifndef TRAINERWORKOUTASSIGNWIDGET_H
#define TRAINERWORKOUTASSIGNWIDGET_H

#include <QWidget>
#include <QHash>
#include <QSharedPointer>
#include <QSortFilterProxyModel>
#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "profiles/client.h"
#include "ui/main/uitablemodel.h"
#include "ui_trainerWorkoutAssignWidget.h"



class TrainerWorkoutAssignWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerWorkoutAssignWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~TrainerWorkoutAssignWidget();

    void setTrainer(QSharedPointer<FitnessTrainer> trainer);

private slots:
    void on_clientsTable_clicked(const QModelIndex &index);
    void on_assignButton_clicked();
    void on_unassignButton_clicked();
    void onHeaderClickedClients(int logicalIndex);
    void onHeaderClickedAssignedWorkouts(int logicalIndex);
    void onHeaderClickedAvailableWorkouts(int logicalIndex);

private:
    Ui_TrainerWorkoutAssignWidget *ui;
    QSharedPointer<AppController> controller;
    QSharedPointer<FitnessTrainer> trainer=nullptr;
    QSharedPointer<Client> currentClient=nullptr;
    //TrainerBoardWidget* parentBoard = nullptr;

    QPointer<UiTableModel> clientModel;
    QPointer<UiTableModel> assignedWorkoutModel;
    QPointer<UiTableModel> availableWorkoutModel;
    QSortFilterProxyModel* proxyModel;
    QSortFilterProxyModel* assignedProxyModel;
    QSortFilterProxyModel* availableProxyModel;

    //QHash<int, QSharedPointer<Client>> clientBuffer;


    void setupClientsTable();
    void populateWorkoutTables();
};

#endif // TRAINERWORKOUTASSIGNWIDGET_H
