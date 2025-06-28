
/// @brief Implementación de la clase UserClientprofileWidget.

#include "userclientprofilewidget.h"
#include "ui_userclientprofilewidget.h"



Q_LOGGING_CATEGORY(UserClientprofileWidget, "ProfileBoardWidget")
/// @brief Constructor del widget de perfil de cliente.
/// Inicializa la interfaz y almacena el controlador.
UserClientprofileWidget::UserClientprofileWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent),
    ui(new Ui_UserClientprofileWidget),
    controller(controller)
{
    ui->setupUi(this);

    // Deshabilitar edición en todas las tablas
    ui->generalRecords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->functionalRecords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->morfoRecords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->romRecords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->medRecords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lifeStylerecords->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/// @brief Destructor.
UserClientprofileWidget::~UserClientprofileWidget()
{
    delete ui;
}
/// @brief Establece el usuario cliente actual para mostrar su información en el widget.
/// En la versión actual, no se realiza actualización automática.
void UserClientprofileWidget::setUser(QSharedPointer<Client> user)
{

    currentClient = user;
    //updateUserInfo();
    if (!currentClient || !currentClient->getProfile()) {
        qDebug()<< "No usuario nulo o no posee un profile";
        return;};

     loadProfileToTables(currentClient->getProfile());
}
void UserClientprofileWidget::setClient(QSharedPointer<Client> client)
{
    currentClient = client;{
        qDebug()<< "No usuario nulo o no posee un profile";
        return;}
    loadProfileToTables(currentClient->getProfile());
}

void UserClientprofileWidget::loadProfileToTables(QSharedPointer<ClientProfile> profile)
{
    UiUtils::populateEnumTable(ui->generalRecords, profile->getAllRecords(ClientProfileData::General), GeneralDataFieldNames());
    UiUtils::populateEnumTable(ui->functionalRecords, profile->getAllRecords(ClientProfileData::Functional), FunctionalFieldNames());
    UiUtils::populateEnumTable(ui->morfoRecords, profile->getAllRecords(ClientProfileData::Morfological), MorfologicalFieldNames());
    UiUtils::populateEnumTable(ui->romRecords, profile->getAllRecords(ClientProfileData::ROM), ROMJointNames());
    UiUtils::populateEnumTable(ui->medRecords, profile->getAllRecords(ClientProfileData::Medical), MedicalRecordsFieldNames());
    UiUtils::populateEnumTable(ui->lifeStylerecords, profile->getAllRecords(ClientProfileData::LifeStyle), LifeStyleFieldNames());
}
