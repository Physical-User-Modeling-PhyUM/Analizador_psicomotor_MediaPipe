#ifndef ASSIGNWORKOUTDIALOG_H
#define ASSIGNWORKOUTDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "core/appcontroller.h"
#include "ui/main/uitablemodel.h"

namespace Ui {
class AssignWorkoutDialog;
}

class AssignWorkoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignWorkoutDialog(QSharedPointer<AppController> controller,int idWorkout, QWidget *parent = nullptr);
    ~AssignWorkoutDialog();

    int getSelectedClientId() const;
   // QDate getSelectedDate() const;

private slots:
    void onAssignButtonClicked();
    void onCancelButtonClicked();

    void on_clientsTable_clicked(const QModelIndex &index);
    void onHeaderClickedClients(int logicalIndex);
private:
    Ui::AssignWorkoutDialog *ui;
    QSharedPointer<AppController> controller;
    UiTableModel* clientModel;
    QSortFilterProxyModel* proxyModel;
    //int idClient;
    int idWorkout;
    void setupClientsTable();
};

#endif // ASSIGNWORKOUTDIALOG_H
