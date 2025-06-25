#ifndef TRAINERCLIENTINFOWIDGET_H
#define TRAINERCLIENTINFOWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QPointer>
#include <QSortFilterProxyModel>
#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "profiles/client.h"
#include "ui_TrainerClientInfoWidget.h"
#include "ui/main/uitablemodel.h"

class TrainerClientInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerClientInfoWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~TrainerClientInfoWidget();

    void setTrainer(QSharedPointer<FitnessTrainer> trainer);
     void loadClientList();
private slots:
    void onClientSelected(int row);

    // Por tabla
    void onAddGeneralRow();
    void onDeleteGeneralRow();

    void onAddFunctionalRow();
    void onDeleteFunctionalRow();

    void onAddMorfologicalRow();
    void onDeleteMorfologicalRow();

    void onAddROMRow();
    void onDeleteROMRow();

    void onAddMedicalRow();
    void onDeleteMedicalRow();

    void onAddLifeStyleRow();
    void onDeleteLifeStyleRow();

    void onSaveClicked();
    void onDiscardClicked();

    // Filtro por cabecera
    void onHeaderClickedClients(int logicalIndex);

private:
    Ui::TrainerClientInfoWidget *ui;
    QSharedPointer<AppController> controller;
    QSharedPointer<FitnessTrainer> trainer;
    QSharedPointer<Client> currentClient;

    QPointer<UiTableModel> clientModel;
    QPointer<QSortFilterProxyModel> proxyModel;

    //void loadClientList();
    void loadProfileToTables(QSharedPointer<ClientProfile> profile);
    void applyTablesToProfile(QSharedPointer<ClientProfile> profile);
};

#endif // TRAINERCLIENTINFOWIDGET_H
