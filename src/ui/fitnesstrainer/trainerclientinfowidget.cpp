#include "trainerclientinfowidget.h"
#include "ui_TrainerClientInfoWidget.h"
#include "utils/uiutils.h"
#include "ui/main/comboboxdelegate.h"
#include "enums/profileEnums.h"

/// @brief Implementación de la clase TrainerClientInfoWidget.
TrainerClientInfoWidget::TrainerClientInfoWidget(QSharedPointer<AppController> controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainerClientInfoWidget),
    controller(controller)
{
    ui->setupUi(this);

    connect(ui->clientTable, &QTableView::clicked, this, [=](const QModelIndex& index) {
        onClientSelected(index.row());
    });

    // Conexiones por tabla
    connect(ui->addGenButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onAddGeneralRow);
    connect(ui->delGenButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onDeleteGeneralRow);

    connect(ui->addFuctionalButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onAddFunctionalRow);
    connect(ui->delFunctionalButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onDeleteFunctionalRow);

    connect(ui->addMorfoButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onAddMorfologicalRow);
    connect(ui->delMorfoButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onDeleteMorfologicalRow);

    connect(ui->addROMButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onAddROMRow);
    connect(ui->delROMButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onDeleteROMRow);

    connect(ui->addMedButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onAddMedicalRow);
    connect(ui->delMedButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onDeleteMedicalRow);

    connect(ui->addLifeButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onAddLifeStyleRow);
    connect(ui->delLifeButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onDeleteLifeStyleRow);

    connect(ui->saveButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onSaveClicked);
    connect(ui->discardButton, &QPushButton::clicked, this, &TrainerClientInfoWidget::onDiscardClicked);
}

TrainerClientInfoWidget::~TrainerClientInfoWidget()
{
    delete ui;
}

void TrainerClientInfoWidget::setTrainer(QSharedPointer<FitnessTrainer> trainer)
{
    this->trainer = trainer;
    loadClientList();
}
void TrainerClientInfoWidget::onHeaderClickedClients(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->clientTable, proxyModel, logicalIndex);
}

void TrainerClientInfoWidget::loadClientList()
{
    QList<QSharedPointer<User>> users = controller->getUserManager()->listUsers(UserType::Client);

    clientModel = new UiTableModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(clientModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);

    UiUtils::fromUserList(users, clientModel);
    ui->clientTable->setModel(proxyModel);
    ui->clientTable->resizeColumnsToContents();

    connect(ui->clientTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerClientInfoWidget::onHeaderClickedClients);
}

/// @brief Carga el perfil del cliente seleccionado y lo muestra en las tablas.
void TrainerClientInfoWidget::onClientSelected(int row)
{
    QModelIndex proxyIndex = proxyModel->index(row, 0);
    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
    int clientId = clientModel->data(sourceIndex.siblingAtColumn(0)).toInt();

    auto user = controller->getUserManager()->getUser(clientId);
    currentClient = qSharedPointerCast<Client>(user);
    if (!currentClient) return;

    if (!currentClient->getProfile()) {
        QSharedPointer<ClientProfile> profile = controller->getUserManager()->getClientProfile(clientId);
        currentClient->setProfile(profile);
    }
    loadProfileToTables(currentClient->getProfile());
}

/// @brief Carga los datos del perfil en las tablas correspondientes.
void TrainerClientInfoWidget::loadProfileToTables(QSharedPointer<ClientProfile> profile)
{

    UiUtils::populateEnumTable(ui->generalRecords, profile->getAllRecords(ClientProfileData::General), GeneralDataFieldNames());
    UiUtils::populateEnumTable(ui->functionalRecords, profile->getAllRecords(ClientProfileData::Functional), FunctionalFieldNames());
    UiUtils::populateEnumTable(ui->morfoRecords, profile->getAllRecords(ClientProfileData::Morfological), MorfologicalFieldNames());
    UiUtils::populateEnumTable(ui->romRecords, profile->getAllRecords(ClientProfileData::ROM), ROMJointNames());
    UiUtils::populateEnumTable(ui->medRecords, profile->getAllRecords(ClientProfileData::Medical), MedicalRecordsFieldNames());
    UiUtils::populateEnumTable(ui->lifeStylerecords, profile->getAllRecords(ClientProfileData::LifeStyle), LifeStyleFieldNames());
}
/// @brief Aplica los valores de las tablas al perfil del cliente.
void TrainerClientInfoWidget::applyTablesToProfile(QSharedPointer<ClientProfile> profile)
{


    profile->setAllRecords(ClientProfileData::General, UiUtils::tableToVariantMap(ui->generalRecords));
    profile->setAllRecords(ClientProfileData::Functional, UiUtils::tableToVariantMap(ui->functionalRecords));
    profile->setAllRecords(ClientProfileData::Morfological, UiUtils::tableToVariantMap(ui->morfoRecords));
    profile->setAllRecords(ClientProfileData::ROM, UiUtils::tableToVariantMap(ui->romRecords));
    profile->setAllRecords(ClientProfileData::Medical, UiUtils::tableToVariantMap(ui->medRecords));
    profile->setAllRecords(ClientProfileData::LifeStyle, UiUtils::tableToVariantMap(ui->lifeStylerecords));
}

void TrainerClientInfoWidget::onAddGeneralRow() {
    auto* m = qobject_cast<QStandardItemModel*>(ui->generalRecords->model());
    m->appendRow({new QStandardItem(), new QStandardItem()});
}

void TrainerClientInfoWidget::onDeleteGeneralRow() {
    ui->generalRecords->model()->removeRow(ui->generalRecords->currentIndex().row());
}

void TrainerClientInfoWidget::onAddFunctionalRow() {
    auto* m = qobject_cast<QStandardItemModel*>(ui->functionalRecords->model());
    m->appendRow({new QStandardItem(), new QStandardItem()});
}

void TrainerClientInfoWidget::onDeleteFunctionalRow() {
    ui->functionalRecords->model()->removeRow(ui->functionalRecords->currentIndex().row());
}

void TrainerClientInfoWidget::onAddMorfologicalRow() {
    auto* m = qobject_cast<QStandardItemModel*>(ui->morfoRecords->model());
    m->appendRow({new QStandardItem(), new QStandardItem()});
}

void TrainerClientInfoWidget::onDeleteMorfologicalRow() {
    ui->morfoRecords->model()->removeRow(ui->morfoRecords->currentIndex().row());
}

void TrainerClientInfoWidget::onAddROMRow() {
    auto* m = qobject_cast<QStandardItemModel*>(ui->romRecords->model());
    m->appendRow({new QStandardItem(), new QStandardItem()});
}

void TrainerClientInfoWidget::onDeleteROMRow() {
    ui->romRecords->model()->removeRow(ui->romRecords->currentIndex().row());
}

void TrainerClientInfoWidget::onAddMedicalRow() {
    auto* m = qobject_cast<QStandardItemModel*>(ui->medRecords->model());
    m->appendRow({new QStandardItem(), new QStandardItem()});
}

void TrainerClientInfoWidget::onDeleteMedicalRow() {
    ui->medRecords->model()->removeRow(ui->medRecords->currentIndex().row());
}

void TrainerClientInfoWidget::onAddLifeStyleRow() {
    auto* m = qobject_cast<QStandardItemModel*>(ui->lifeStylerecords->model());
    m->appendRow({new QStandardItem(), new QStandardItem()});
}

void TrainerClientInfoWidget::onDeleteLifeStyleRow() {
    ui->lifeStylerecords->model()->removeRow(ui->lifeStylerecords->currentIndex().row());
}
/// @brief Guarda los cambios realizados en las tablas en el sistema.
void TrainerClientInfoWidget::onSaveClicked()
{
    if (!currentClient) return;

    applyTablesToProfile(currentClient->getProfile());
    controller->getUserManager()->updateClientProfile(currentClient->getProfile());
}
/// @brief Descarta cambios y recarga la información del perfil desde el modelo.
void TrainerClientInfoWidget::onDiscardClicked()
{
    if (!currentClient) return;

    loadProfileToTables(currentClient->getProfile());
}
