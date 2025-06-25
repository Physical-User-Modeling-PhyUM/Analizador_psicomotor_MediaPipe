#include "trainermetricsmainwidget.h"
#include "ui_trainermetricsmainwidget.h"
#include "ui/metrics/heatmapwidget.h"
#include "ui/metrics/timewidget.h"
//#include "ui/metrics/conditiondistributionchart.h"
#include "ui/fitnesstrainer/trainerboardwidget.h"
#include "utils/uiutils.h"
#include "ui/main/uitablemodel.h"

Q_LOGGING_CATEGORY(trainingMetricsWidget, "trainingMetricsWidget")

TrainerMetricsMainWidget::TrainerMetricsMainWidget(QSharedPointer<AppController> controller, QWidget* parent)
    : QWidget(parent), ui(new Ui_TrainerMetricsMainWidget), controller(controller)
{
    ui->setupUi(this);

    ui->clientesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->workoutsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->sesionesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->exercisesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->exercisesTable, &QTableView::clicked,this, &TrainerMetricsMainWidget::onExercisesTableClicked);
    connect(ui->clientesTable, &QTableView::clicked, this, &TrainerMetricsMainWidget::onClienteTableClicked);
    connect(ui->workoutsTable, &QTableView::clicked, this, &TrainerMetricsMainWidget::onWorkoutTableClicked);
    connect(ui->sesionesTable, &QTableView::clicked, this, &TrainerMetricsMainWidget::onSesionTableClicked);

    // heatmapWidget = new HeatmapWidget;
    // timeWidget = new TimeWidget;
    // conditionChart = new ConditionDistributionChart;

    // ui->tabWidget->addTab(heatmapWidget, tr("Mapa de calor"));
    // ui->tabWidget->addTab(timeWidget, tr("Tiempo"));
    // ui->tabWidget->addTab(conditionChart, tr("Distribución condiciones"));


    heatmapWidget = new HeatmapWidget(this);
    timeWidget = new TimeWidget(this);
    //conditionChart = new ConditionDistributionChart(this);
    angleViewer = new AngleViewerWidget(this);
    logViewer = new LogViewerWidget(controller, this);


    ui->tabWidget->clear();
    ui->tabWidget->addTab(heatmapWidget, tr("Mapa de calor"));
    ui->tabWidget->addTab(timeWidget, tr("Tiempo"));
    //ui->tabWidget->addTab(conditionChart, tr("Distribución condiciones"));
    ui->tabWidget->addTab(angleViewer, tr("Ángulos"));
    ui->tabWidget->addTab(logViewer, tr("Log de ejecución"));

    // for (int i = 0; i < ui->tabWidget->count(); ++i) {
    //     QWidget* w = ui->tabWidget->widget(i);
    //     //qDebug(trainingMetricsWidget) << " Tab" << i << "es de tipo" << w->metaObject()->className();
    // }
    metricsManager = controller->getMetricsManager();

    loadClientes();
}

TrainerMetricsMainWidget::~TrainerMetricsMainWidget()
{
    delete ui;
}

void TrainerMetricsMainWidget::setReport(const SesionReport &report)
{

}

void TrainerMetricsMainWidget::onClienteSelected(int row)
{
    currentClientId = ui->clientesTable
                          ->model()
                          ->data(ui->clientesTable->model()->index(row, 0))
                          .toInt();
    loadWorkoutsForClient(currentClientId);
}

void TrainerMetricsMainWidget::onWorkoutSelected(int row)
{
    currentWorkoutId = ui->workoutsTable
                           ->model()
                           ->data(ui->workoutsTable->model()->index(row, 0))
                           .toInt();
    loadExercisesForWorkout(currentWorkoutId);
}


void TrainerMetricsMainWidget::onSesionSelected(int row) {
    int idSesion = ui->sesionesTable->model()->data(ui->sesionesTable->model()->index(row, 0)).toInt();
    QSharedPointer<TrainingSesion> sesion = controller->getTrainingManager()->getTrainingSesion(idSesion);

    if (!sesion) {
        qWarning(trainingMetricsWidget) << "No se pudo cargar la sesión con ID:" << idSesion;
        return;
    }
   metricsManager->setReport(sesion->getReport());
     updateWidgets();

   //qDebug(trainingMetricsWidget)<<"SesionRecuperada"<<sesion->getIdSesion()<<"Report"<<sesion->getReport().printSeriesData();
}

// void TrainerMetricsMainWidget::updateWidgets() {
//     for (int i = 0; i < ui->tabWidget->count(); ++i) {
//         QWidget* w = ui->tabWidget->widget(i);
//         if (auto heatmap = qobject_cast<HeatmapWidget*>(w)) {
//             // repobla lista de series y dibuja el heatmap
//             heatmap->setManager(metricsManager.data());
//         }
//         else if (auto time = qobject_cast<TimeWidget*>(w)) {
//             populateTimeWidget();
//         }
//         else if (auto condChart = qobject_cast<ConditionDistributionChart*>(w)) {
//             populateConditionDistributionChart();
//         }
//         else if (auto angle = qobject_cast<AngleViewerWidget*>(w)) {
//             angle->setManager(metricsManager.data());
//         }
//         else if (auto log = qobject_cast<LogViewerWidget*>(w)) {
//             int idSesion = ui->sesionesTable->model()->data(ui->sesionesTable->model()->index(ui->sesionesTable->currentIndex().row(), 0)).toInt();
//             QSharedPointer<TrainingSesion> sesion = controller->getTrainingManager()->getTrainingSesion(idSesion);
//             if (sesion) {
//                 log->loadSesion(sesion);
//             }
//         }

//     }
// }


void TrainerMetricsMainWidget::updateWidgets() {
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        QWidget* w = ui->tabWidget->widget(i);

        if (auto heatmap = qobject_cast<HeatmapWidget*>(w)) {
            heatmap->setManager(metricsManager.data());
        }
        else if (auto time = qobject_cast<TimeWidget*>(w)) {
            populateTimeWidget();
        }
        // else if (auto condChart = qobject_cast<ConditionDistributionChart*>(w)) {
        //     populateConditionDistributionChart();
        // }
        else if (auto angle = qobject_cast<AngleViewerWidget*>(w)) {
            angle->setManager(metricsManager.data());
        }
        else if (auto log = qobject_cast<LogViewerWidget*>(w)) {
            // Verificación completa
            auto model = ui->sesionesTable->model();
            if (!model) {
                qWarning(trainingMetricsWidget) << "[updateWidgets] sesionesTable no tiene modelo.";
                continue;
            }

            QModelIndex current = ui->sesionesTable->currentIndex();
            if (!current.isValid()) {
                qWarning(trainingMetricsWidget) << "[updateWidgets] No hay índice seleccionado en sesionesTable.";
                continue;
            }

            int row = current.row();
            QModelIndex idIndex = model->index(row, 0);
            if (!idIndex.isValid()) {
                qWarning(trainingMetricsWidget) << "[updateWidgets] Índice no válido para ID de sesión.";
                continue;
            }

            int idSesion = model->data(idIndex).toInt();
            QSharedPointer<TrainingSesion> sesion = controller->getTrainingManager()->getTrainingSesion(idSesion);

            if (!sesion) {
                qWarning(trainingMetricsWidget) << "[updateWidgets] No se encontró la sesión con ID:" << idSesion;
                continue;
            }

            log->loadSesion(sesion);
        }
    }
}

void TrainerMetricsMainWidget::onClienteTableClicked(const QModelIndex &index)
{
    onClienteSelected(index.row());
}

void TrainerMetricsMainWidget::onWorkoutTableClicked(const QModelIndex &index)
{
    onWorkoutSelected(index.row());
}

void TrainerMetricsMainWidget::onSesionTableClicked(const QModelIndex &index)
{
    onSesionSelected(index.row());
}
void TrainerMetricsMainWidget::populateTimeWidget() {
    // if (!metricsManager || !timeWidget) return;
    // timeWidget->setReport(metricsManager->getReport());

    //qDebug(trainingMetricsWidget) << "[populateTimeWidget] llamado.";

    if (!metricsManager) {
        //qWarning(trainingMetricsWidget) << " metricsManager es nullptr.";
        return;
    }

    if (!timeWidget) {
        //qWarning(trainingMetricsWidget) << " timeWidget es nullptr.";
        return;
    }

    //qDebug(trainingMetricsWidget) << " Llamando a setReport en timeWidget...";
    timeWidget->setReport(metricsManager->getReport());

}

void TrainerMetricsMainWidget::populateConditionDistributionChart() {
    // if (!metricsManager || !conditionChart) return;
    // conditionChart->setReport(metricsManager->getReport());
}

void TrainerMetricsMainWidget::populateHeatmapWidget() {
    if (!metricsManager || !heatmapWidget) return;
    heatmapWidget->setManager(metricsManager.data());
}


void TrainerMetricsMainWidget::loadClientes() {
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

void TrainerMetricsMainWidget::loadWorkoutsForClient(int idClient) {
    QSharedPointer<Client> client;

    auto board = qobject_cast<TrainerBoardWidget*>(this->parentWidget());
    if (board && board->isInBuffer(idClient)) {
        client = board->getFromBuffer(idClient);
        qDebug(trainingMetricsWidget) << " Cliente encontrado en buffer con ID:" << idClient;
    } else {
        auto user = controller->getUserManager()->getUser(idClient);
        if (!user) {
            qWarning(trainingMetricsWidget) << " No se encontró el usuario con ID:" << idClient;
            return;
        }

        qDebug() << "Usuario encontrado:" << user->getUserName()
                 << "Tipo:" << static_cast<int>(user->getUserType());

        if (user->getUserType() != UserType::Client) {
            qWarning(trainingMetricsWidget) << " El usuario no es un Client. Abortando.";
            return;
        }

        client = qSharedPointerCast<Client>(user);
        if (!client) {
            qWarning(trainingMetricsWidget) << "qSharedPointerCast a Client falló para el usuario con ID:" << idClient;
            return;
        }

        if (board) board->addToBuffer(client);
    }

    if (!client) {
        qWarning(trainingMetricsWidget) << "Cliente es nulo después de recuperación. Abortando.";
        return;
    }

    QMap<QDateTime, int> calendar = client->getWorkoutCalendar();
    if (calendar.isEmpty()) {
        qDebug(trainingMetricsWidget) << " Calendario vacío para cliente ID:" << idClient;
        return;
    }

    // Ordenar workouts por fecha de asignación
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
            qWarning(trainingMetricsWidget) << " No se encontró el workout con ID:" << idWorkout;
            continue;
        }

        model->setItem(row, 0, new QStandardItem(QString::number(idWorkout)));
        model->setItem(row, 1, new QStandardItem(workout->getName()));
        model->setItem(row, 2, new QStandardItem(fecha.toString("dd/MM/yyyy")));
        row++;
    }

    ui->workoutsTable->setModel(model);
    ui->workoutsTable->resizeColumnsToContents();

    // if (model->rowCount() > 0) {
    //     ui->workoutsTable->selectRow(0);
    //     onWorkoutSelected(0);
    // }
}


void TrainerMetricsMainWidget::loadExercisesForWorkout(int idWorkout)
{
    auto workout = controller->getTrainingManager()->getWorkoutData(idWorkout);
    if (!workout) {
        qWarning(trainingMetricsWidget) << "Workout con ID" << idWorkout << "no encontrado.";
        return;
    }
    UiTableModel* model = new UiTableModel(this);
    UiUtils::fromWorkoutExercises(workout, model);

    ui->exercisesTable->setModel(model);
    ui->exercisesTable->resizeColumnsToContents();
}


void TrainerMetricsMainWidget::loadAvailableSesions(int idWorkout, int idClient, int idEx)
{
    QMap<int,QDateTime> sesiones = controller->getTrainingManager()->getSesionList(idClient, idWorkout, idEx);

    UiTableModel* model = new UiTableModel(this);
    UiUtils::fromSessionSummaries(sesiones, model);

    ui->sesionesTable->setModel(model);
    ui->sesionesTable->resizeColumnsToContents();
}

void TrainerMetricsMainWidget::onSerieChanged(int index) {
    // if (!metricsManager || !timeWidget) return;

    // QList<int> series = metricsManager->getAvailableSeries();
    // if (index < 0 || index >= series.size()) return;

    // int serie = series[index];
    // auto segmentos = metricsManager->getSegments(serie);
    // timeWidget->setStateNames(metricsManager->getReport().getStateNames());
    // timeWidget->setRepetitionSegments(segmentos);

    // qDebug(trainingMetricsWidget) << "Serie cambiada: " << serie
    //                               << " con " << segmentos.size() << " repeticiones.";
}

void TrainerMetricsMainWidget::onExercisesTableClicked(const QModelIndex &index)
{
    onExerciseSelected(index.row());
}

void TrainerMetricsMainWidget::onExerciseSelected(int row)
{
    currentExerciseId = ui->exercisesTable->model()->data(ui->exercisesTable->model()->index(row, 0)).toInt();
    loadAvailableSesions(currentWorkoutId, currentClientId, currentExerciseId);
}



