#include "userclientmetricswidget.h"
#include "core/appcontroller.h"
#include "ui_userclientmetricswidget.h"

UserClientMetricsWidget::UserClientMetricsWidget(QSharedPointer<AppController> controller, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui_UserClientMetricsWidget)
    , controller(controller)
{
    ui->setupUi(this);

    ui->clientesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->workoutsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->sesionesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->exercisesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->exercisesTable, &QTableView::clicked,this, &UserClientMetricsWidget::onExercisesTableClicked);
    connect(ui->clientesTable, &QTableView::clicked, this, &UserClientMetricsWidget::onClienteTableClicked);
    connect(ui->workoutsTable, &QTableView::clicked, this, &UserClientMetricsWidget::onWorkoutTableClicked);
    connect(ui->sesionesTable, &QTableView::clicked, this, &UserClientMetricsWidget::onSesionTableClicked);



    heatmapWidget = new HeatmapWidget(this);
    timeWidget = new TimeWidget(this);
    //conditionChart = new ConditionDistributionChart(this);
    angleViewer = new AngleViewerWidget(this);
    //logViewer = new LogViewerWidget(controller, this);

    ui->tabWidget->clear();
    ui->tabWidget->addTab(heatmapWidget, tr("Mapa de calor"));
    ui->tabWidget->addTab(timeWidget, tr("Tiempo"));
    //ui->tabWidget->addTab(conditionChart, tr("Distribución condiciones"));
    ui->tabWidget->addTab(angleViewer, tr("Ángulos"));

    metricsManager = controller->getMetricsManager();

    //loadClientes();
}

UserClientMetricsWidget::~UserClientMetricsWidget()
{
    delete ui;
}
void UserClientMetricsWidget::setUser(QSharedPointer<Client> user)
{
    currentUser = user;
    if (currentUser) {
        currentClientId = currentUser->getId();
        loadWorkoutsForClient(currentClientId);
        // Ocultar la tabla de selección de clientes si se desea
        ui->clientesTable->hide();
    }
}







void UserClientMetricsWidget::setReport(const SesionReport &report)
{

}

void UserClientMetricsWidget::onClienteSelected(int row)
{
    currentClientId = ui->clientesTable
                          ->model()
                          ->data(ui->clientesTable->model()->index(row, 0))
                          .toInt();
    loadWorkoutsForClient(currentClientId);
}

void UserClientMetricsWidget::onWorkoutSelected(int row)
{
    currentWorkoutId = ui->workoutsTable
                           ->model()
                           ->data(ui->workoutsTable->model()->index(row, 0))
                           .toInt();
    loadExercisesForWorkout(currentWorkoutId);
}


void UserClientMetricsWidget::onSesionSelected(int row) {
    int idSesion = ui->sesionesTable->model()->data(ui->sesionesTable->model()->index(row, 0)).toInt();
    QSharedPointer<TrainingSesion> sesion = controller->getTrainingManager()->getTrainingSesion(idSesion);

    if (!sesion) {
        qWarning() << "No se pudo cargar la sesión con ID:" << idSesion;
        return;
    }
    metricsManager->setReport(sesion->getReport());
    updateWidgets();


}




void UserClientMetricsWidget::updateWidgets() {
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        QWidget* w = ui->tabWidget->widget(i);

        if (auto heatmap = qobject_cast<HeatmapWidget*>(w)) {
            heatmap->setManager(metricsManager.data());
        }
        else if (auto time = qobject_cast<TimeWidget*>(w)) {
            populateTimeWidget();
        }
        else if (auto angle = qobject_cast<AngleViewerWidget*>(w)) {
            angle->setManager(metricsManager.data());
        }
        else if (auto log = qobject_cast<LogViewerWidget*>(w)) {
            // Verificación completa
            auto model = ui->sesionesTable->model();
            if (!model) {
                qWarning() << "[updateWidgets] sesionesTable no tiene modelo.";
                continue;
            }

            QModelIndex current = ui->sesionesTable->currentIndex();
            if (!current.isValid()) {
                qWarning() << "[updateWidgets] No hay índice seleccionado en sesionesTable.";
                continue;
            }

            int row = current.row();
            QModelIndex idIndex = model->index(row, 0);
            if (!idIndex.isValid()) {
                qWarning() << "[updateWidgets] Índice no válido para ID de sesión.";
                continue;
            }

            int idSesion = model->data(idIndex).toInt();
            QSharedPointer<TrainingSesion> sesion = controller->getTrainingManager()->getTrainingSesion(idSesion);

            if (!sesion) {
                qWarning() << "[updateWidgets] No se encontró la sesión con ID:" << idSesion;
                continue;
            }

            log->loadSesion(sesion);
        }
    }
}

void UserClientMetricsWidget::onClienteTableClicked(const QModelIndex &index)
{
    onClienteSelected(index.row());
}

void UserClientMetricsWidget::onWorkoutTableClicked(const QModelIndex &index)
{
    onWorkoutSelected(index.row());
}

void UserClientMetricsWidget::onSesionTableClicked(const QModelIndex &index)
{
    onSesionSelected(index.row());
}
void UserClientMetricsWidget::populateTimeWidget() {

    if (!metricsManager) {
        return;
    }

    if (!timeWidget) {
        return;
    }
    timeWidget->setReport(metricsManager->getReport());

}

void UserClientMetricsWidget::populateConditionDistributionChart() {
}

void UserClientMetricsWidget::populateHeatmapWidget() {
    if (!metricsManager || !heatmapWidget) return;
    heatmapWidget->setManager(metricsManager.data());
}


void UserClientMetricsWidget::loadClientes() {
    auto clients = controller->getUserManager()->listUsers(UserType::Client);
    if (clients.isEmpty()) return;

    QStandardItemModel* model = new QStandardItemModel(clients.size(), 3, this);
    model->setHorizontalHeaderLabels({"ID", "Nombre", "Email"});

    int row = 0;
    for (const auto& client : clients) {
        model->setItem(row, 0, new QStandardItem(QString::number(client->getId())));
        model->setItem(row, 1, new QStandardItem(client->getUserName()));
        model->setItem(row, 2, new QStandardItem(client->getEmail()));
        row++;
    }

    ui->clientesTable->setModel(model);
    ui->clientesTable->resizeColumnsToContents();
}

void UserClientMetricsWidget::loadWorkoutsForClient(int idClient) {
    if (!currentUser || currentUser->getId() != idClient) {
        qWarning() << "Cliente no asignado o ID no coincide.";
        return;
    }

    QMap<QDateTime, int> calendar = currentUser->getWorkoutCalendar();
    if (calendar.isEmpty()) {
        qDebug() << "Calendario vacío para cliente ID:" << idClient;
        return;
    }

    QList<QPair<int, QDateTime>> workoutsOrdenados;
    for (auto it = calendar.begin(); it != calendar.end(); ++it)
        workoutsOrdenados.append(qMakePair(it.value(), it.key()));

    std::sort(workoutsOrdenados.begin(), workoutsOrdenados.end(),
              [](const QPair<int, QDateTime>& a, const QPair<int, QDateTime>& b) {
                  return a.second < b.second;
              });

    QStandardItemModel* model = new QStandardItemModel(workoutsOrdenados.size(), 3, this);
    model->setHorizontalHeaderLabels({"ID Workout", "Nombre", "Asignado el"});

    int row = 0;
    for (const auto& pair : workoutsOrdenados) {
        int idWorkout = pair.first;
        QDateTime fecha = pair.second;

        auto workout = controller->getTrainingManager()->getWorkoutData(idWorkout);
        if (!workout) {
            qWarning() << " No se encontró el workout con ID:" << idWorkout;
            continue;
        }

        model->setItem(row, 0, new QStandardItem(QString::number(idWorkout)));
        model->setItem(row, 1, new QStandardItem(workout->getName()));
        model->setItem(row, 2, new QStandardItem(fecha.toString("dd/MM/yyyy")));
        row++;
    }

    ui->workoutsTable->setModel(model);
    ui->workoutsTable->resizeColumnsToContents();
}



void UserClientMetricsWidget::loadExercisesForWorkout(int idWorkout)
{
    auto workout = controller->getTrainingManager()->getWorkoutData(idWorkout);
    if (!workout) {
        qWarning() << "Workout con ID" << idWorkout << "no encontrado.";
        return;
    }
    UiTableModel* model = new UiTableModel(this);
    UiUtils::fromWorkoutExercises(workout, model);

    ui->exercisesTable->setModel(model);
    ui->exercisesTable->resizeColumnsToContents();
}


void UserClientMetricsWidget::loadAvailableSesions(int idWorkout, int idClient, int idEx)
{
    QMap<int,QDateTime> sesiones = controller->getTrainingManager()->getSesionList(idClient, idWorkout, idEx);

    UiTableModel* model = new UiTableModel(this);
    UiUtils::fromSessionSummaries(sesiones, model);

    ui->sesionesTable->setModel(model);
    ui->sesionesTable->resizeColumnsToContents();
}

void UserClientMetricsWidget::onSerieChanged(int index) {

}

void UserClientMetricsWidget::onExercisesTableClicked(const QModelIndex &index)
{
    onExerciseSelected(index.row());
}

void UserClientMetricsWidget::onExerciseSelected(int row)
{
    currentExerciseId = ui->exercisesTable->model()->data(ui->exercisesTable->model()->index(row, 0)).toInt();
    loadAvailableSesions(currentWorkoutId, currentClientId, currentExerciseId);
}



