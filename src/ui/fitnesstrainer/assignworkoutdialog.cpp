#include "assignworkoutdialog.h"
#include "ui_assignworkoutdialog.h"
#include "utils/uiutils.h"
#include <QMessageBox>

AssignWorkoutDialog::AssignWorkoutDialog(QSharedPointer<AppController> controller,int idWorkout, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AssignWorkoutDialog),
    controller(controller),
    clientModel(new UiTableModel(this)),
    proxyModel(new QSortFilterProxyModel(this)),
    idWorkout(idWorkout)
{
    ui->setupUi(this);

    setupClientsTable();
    connect(ui->Assign_Button, &QPushButton::clicked, this, &AssignWorkoutDialog::onAssignButtonClicked);
    connect(ui->cancel_Button, &QPushButton::clicked, this, &AssignWorkoutDialog::onCancelButtonClicked);
}

AssignWorkoutDialog::~AssignWorkoutDialog()
{
    delete ui;
}

void AssignWorkoutDialog::setupClientsTable()
{

    auto clientsList = controller->getUserManager()->listUsers(UserType::Client);
    UiUtils::fromUserList(clientsList, clientModel);

    // deshabilitamos la edición en las columnas de la tablaa
    for (int i = 0; i < clientModel->columnCount(); ++i) {
        clientModel->setColumnReadOnly(i);
    }

    proxyModel->setSourceModel(clientModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    ui->clientsTable->setModel(proxyModel);
    ui->clientsTable->resizeColumnsToContents();
}

int AssignWorkoutDialog::getSelectedClientId() const
{
    QModelIndex index = ui->clientsTable->currentIndex();
    if (!index.isValid()) return -1;

    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    QVariant idVariant = clientModel->data(sourceIndex.siblingAtColumn(0));

    return idVariant.toInt();
}


void AssignWorkoutDialog::on_clientsTable_clicked(const QModelIndex &index)
{
    // if (!index.isValid()) return;
    // QModelIndex sourceIndex = proxyModel->mapToSource(index);
    //  if (!sourceIndex.isValid()) return;
    // idClient= clientModel->data(sourceIndex.siblingAtColumn(0)).toInt();


}
// QDate AssignWorkoutDialog::getSelectedDate() const
// {
//     return ui->dateEdit->date();
// }

void AssignWorkoutDialog::onAssignButtonClicked()
{
    int id = getSelectedClientId();
    if (id <= 0) {
        QMessageBox::warning(this, "Asignar Workout", "Por favor selecciona un cliente.");
        return;
    }

    QDate fecha = ui->calendarWidget->selectedDate();
    if (!fecha.isValid()) {
        QMessageBox::warning(this, "Fecha", "Seleccione una fecha para el Plan de Entrenamiento.");
        return;
    }

    QDateTime d(fecha, QTime(0, 0, 0));

    if (!controller->getUserManager()->assignWorkoutToClient(d, idWorkout, id)) {
        QMessageBox::warning(this, "Error", "No se ha podido asignar el Plan de Entrenamiento.");
        reject();
        return;
    }

    accept();
}


void AssignWorkoutDialog::onCancelButtonClicked()
{
    reject();
}

void AssignWorkoutDialog::onHeaderClickedClients(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->clientsTable, proxyModel, logicalIndex);
}
