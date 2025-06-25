#include "trainerworkoutassignwidget.h"
#include "ui/fitnesstrainer/trainerboardwidget.h"
#include "ui_trainerWorkoutAssignWidget.h"
#include "utils/uiutils.h"
#include <QMessageBox>

TrainerWorkoutAssignWidget::TrainerWorkoutAssignWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::TrainerWorkoutAssignWidget),
    controller(controller),
    clientModel(new UiTableModel(this)),
    assignedWorkoutModel(new UiTableModel(this)),
    availableWorkoutModel(new UiTableModel(this)),
    proxyModel(new QSortFilterProxyModel(this)),
    assignedProxyModel(new QSortFilterProxyModel(this)),
    availableProxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);

    //setupClientsTable();
    //populateWorkoutTables();

    connect(ui->assignButton, &QPushButton::clicked, this, &TrainerWorkoutAssignWidget::on_assignButton_clicked);
    connect(ui->unassignButton, &QPushButton::clicked, this, &TrainerWorkoutAssignWidget::on_unassignButton_clicked);
}

TrainerWorkoutAssignWidget::~TrainerWorkoutAssignWidget()
{
    delete ui;
}

void TrainerWorkoutAssignWidget::setTrainer(QSharedPointer<FitnessTrainer> trainer)
{
    this->trainer = trainer;
    setupClientsTable();
    populateWorkoutTables();
}

void TrainerWorkoutAssignWidget::setupClientsTable()
{
    auto clientsList = controller->getUserManager()->listUsers(UserType::Client);
    UiUtils::fromUserList(clientsList, clientModel);

    for (int i = 0; i < clientModel->columnCount(); ++i)
        clientModel->setColumnReadOnly(i);

    proxyModel->setSourceModel(clientModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    ui->clientsTable->setModel(proxyModel);
    ui->clientsTable->resizeColumnsToContents();

    connect(ui->clientsTable, &QTableView::clicked, this, &TrainerWorkoutAssignWidget::on_clientsTable_clicked);
    connect(ui->clientsTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutAssignWidget::onHeaderClickedClients);

}

void TrainerWorkoutAssignWidget::on_clientsTable_clicked(const QModelIndex &index)
{
    qDebug() << "Clic en clientsTable";
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    int id = clientModel->data(sourceIndex.siblingAtColumn(0)).toInt();
   //selectedClientId = id;

    auto parent = qobject_cast<TrainerBoardWidget*>(this->parentWidget()->parentWidget());
    if (parent==nullptr) {
        qDebug() << "Parent es nullptr";
        return;
    }
    if (!(parent->isInBuffer(id))) {
        QSharedPointer<User> user = controller->getUserManager()->getUser(id);
        currentClient = qSharedPointerCast<Client>(user);
        parent->addToBuffer( currentClient);
    }else{
        currentClient=parent->getFromBuffer(id);
    }

    UiUtils::fromWorkoutCalendar(currentClient->getWorkoutCalendar(), assignedWorkoutModel);

    for (int i = 0; i < assignedWorkoutModel->columnCount(); ++i)
        assignedWorkoutModel->setColumnReadOnly(i, true);


    assignedProxyModel->setSourceModel(assignedWorkoutModel);
    assignedProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->assignTable->setModel(assignedProxyModel);
    ui->assignTable->resizeColumnsToContents();


    connect(ui->assignTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutAssignWidget::onHeaderClickedAssignedWorkouts);

}

void TrainerWorkoutAssignWidget::populateWorkoutTables()
{
    // if (selectedClientId < 0) return;

    // auto sesion = controller->getTrainingManager()->listClientAssignedWorkouts(selectedClientId);
    // UiUtils::fromWorkoutSummaries(sesion, assignedWorkoutModel);

    // assignedProxyModel->setSourceModel(assignedWorkoutModel);
    // assignedProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    // ui->assignTable->setModel(assignedProxyModel);
    // ui->assignTable->resizeColumnsToContents();

    // connect(ui->assignTable->horizontalHeader(), &QHeaderView::sectionClicked, this, [=](int logicalIndex){
    //     UiUtils::showFilterPopup(this, ui->assignTable, assignedProxyModel, logicalIndex);
    // });

    auto todos = controller->getTrainingManager()->listWorkouts();
    UiUtils::fromWorkoutSummaries(todos, availableWorkoutModel);

    for (int i = 0; i < availableWorkoutModel->columnCount(); ++i)
        availableWorkoutModel->setColumnReadOnly(i, true);

    availableProxyModel->setSourceModel(availableWorkoutModel);
    availableProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->workoutsTable->setModel(availableProxyModel);
    ui->workoutsTable->resizeColumnsToContents();

    connect(ui->workoutsTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutAssignWidget::onHeaderClickedAvailableWorkouts);

}

void TrainerWorkoutAssignWidget::on_assignButton_clicked()
{
    if (currentClient == nullptr) {
        QMessageBox::warning(this, "Asignación", "Seleccione un cliente primero.");
        return;
    }

    QModelIndex idx = ui->workoutsTable->currentIndex();
    if (!idx.isValid()) return;

    QModelIndex sourceIndex = availableProxyModel->mapToSource(idx);
    if (!sourceIndex.isValid()) return;
    int idWorkout = availableWorkoutModel->data(sourceIndex.siblingAtColumn(0)).toInt();

    QDate fecha= ui->calendarWidget->selectedDate();

    if (fecha.isNull()){
        QMessageBox::warning(this, "Fecha", "Seleccione una fecha para el Plan de Entrenamiento.");
        return;
    }

    currentClient->setWorkout(QDateTime(fecha,QTime(0,0,0)),idWorkout);

    controller->getUserManager()->updateUser(currentClient);

    // Recorgamos el modelo
    UiUtils::fromWorkoutCalendar(currentClient->getWorkoutCalendar(), assignedWorkoutModel);
    ui->assignTable->resizeColumnsToContents();


}

void TrainerWorkoutAssignWidget::on_unassignButton_clicked()
{
    if (currentClient == nullptr) {
        QMessageBox::warning(this, "Desasignación", "Seleccione un cliente primero.");
        return;
    }

    QModelIndex assig = ui->assignTable->currentIndex();

    if (!assig.isValid()) {
        QMessageBox::warning(this, "Selección de Plan", "Seleccione un plan de entrenamiento para borrar.");
        return;
    }
    QModelIndex sourceIndex = assignedProxyModel->mapToSource(assig);
    QDateTime date;
    if (!sourceIndex.isValid()) date = availableWorkoutModel->data(sourceIndex.siblingAtColumn(0)).toDateTime();

    if (!date.isNull()){
        currentClient->delWorkout(date);
        controller->getUserManager()->updateUser(currentClient);
    }

    // Recorgamos el modelo
    UiUtils::fromWorkoutCalendar(currentClient->getWorkoutCalendar(), assignedWorkoutModel);
    ui->assignTable->resizeColumnsToContents();

}
void TrainerWorkoutAssignWidget::onHeaderClickedClients(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->clientsTable, proxyModel, logicalIndex);
}

void TrainerWorkoutAssignWidget::onHeaderClickedAssignedWorkouts(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->assignTable, assignedProxyModel, logicalIndex);
}

void TrainerWorkoutAssignWidget::onHeaderClickedAvailableWorkouts(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->workoutsTable, availableProxyModel, logicalIndex);
}
