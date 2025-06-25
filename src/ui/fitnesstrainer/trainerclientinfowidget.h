/// @file trainerclientinfowidget.h
/// @brief Declaración de la clase TrainerClientInfoWidget, responsable de mostrar y editar el perfil completo de los clientes.

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

/// @class TrainerClientInfoWidget
/// @brief Widget que permite al entrenador visualizar y modificar los perfiles detallados de sus clientes.
///
/// Cada perfil incluye información general, funcional, morfológica, médica, de estilo de vida y rango articular (ROM).
/// Las tablas permiten edición directa, así como añadir o eliminar registros.
class TrainerClientInfoWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructor.
    explicit TrainerClientInfoWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~TrainerClientInfoWidget();

    /// @brief Asigna el entrenador actual al widget.
    void setTrainer(QSharedPointer<FitnessTrainer> trainer);

    /// @brief Carga la lista de clientes disponibles.
    void loadClientList();

private slots:
    void onClientSelected(int row);

    // Acciones sobre tablas
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

    /// @brief Guarda los cambios realizados en las tablas al perfil del cliente actual.
    void onSaveClicked();

    /// @brief Descarta los cambios y recarga los datos del perfil.
    void onDiscardClicked();

    /// @brief Activa el filtro sobre la tabla de clientes.
    void onHeaderClickedClients(int logicalIndex);

private:
    Ui::TrainerClientInfoWidget *ui;
    QSharedPointer<AppController> controller;
    QSharedPointer<FitnessTrainer> trainer;
    QSharedPointer<Client> currentClient;

    QPointer<UiTableModel> clientModel;
    QPointer<QSortFilterProxyModel> proxyModel;

    void loadProfileToTables(QSharedPointer<ClientProfile> profile);
    void applyTablesToProfile(QSharedPointer<ClientProfile> profile);
};

#endif // TRAINERCLIENTINFOWIDGET_H
