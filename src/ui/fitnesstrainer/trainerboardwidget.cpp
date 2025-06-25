/// @brief Implementación de la clase TrainerBoardWidget.
#include "trainerboardwidget.h"

#include "ui/fitnesstrainer/trainermetricsmainwidget.h"
#include "ui_trainerboardwidget.h"
#include <QMessageBox>
#include <QPixmap>

Q_LOGGING_CATEGORY(TrainerBoard, "TrainerBoardWidget")

/// @brief Constructor. Inicializa los widgets y conecta eventos de navegación y error.
TrainerBoardWidget::TrainerBoardWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent),
    ui(new Ui_TrainerBoardWidget),
    controller(controller)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &TrainerBoardWidget::sectionChanged);

    // Escuchar errores del repositorio
    connect(controller->getTrainingManager()->getRepository().data(),
            &TrainingRepository::ui_errorMessage,
            this,
            &TrainerBoardWidget::onUiErrorMessage);

    connect(controller->getValidationManager().data(),
            &ValidationManager::ui_errorMessage,
            this,
            &TrainerBoardWidget::onUiErrorMessage);


    // Inicializar widgets y añadirlos al stackedWidget
    infoWidget = new TrainerInfoWidget(controller, this);
    ui->stackedWidget->insertWidget(0, infoWidget);

    infoClientWidget = new TrainerClientInfoWidget(controller, this);
    ui->stackedWidget->insertWidget(1, infoClientWidget);

    metricsWidget = new TrainerMetricsMainWidget(controller, this);
    ui->stackedWidget->insertWidget(2, metricsWidget);

    workoutDesignWidget = new TrainerWorkoutDesignWidget(controller, this);
    ui->stackedWidget->insertWidget(3, workoutDesignWidget);

    assignWidget = new TrainerWorkoutAssignWidget(controller, this);
    ui->stackedWidget->insertWidget(4, assignWidget);

    // metricsMainWidget = new TrainerMetricsMainWidget(controller, this);
    // ui->stackedWidget->insertWidget(5, metricsMainWidget);
}
/// @brief Destructor.
TrainerBoardWidget::~TrainerBoardWidget()
{
    delete ui;
}
/// @brief Asigna el usuario entrenador y actualiza los widgets correspondientes.
void TrainerBoardWidget::setUser(QSharedPointer<FitnessTrainer> user)
{
    currentTrainer = user;
    updateTrainerInfo();

    if (infoWidget) infoWidget->setTrainer(currentTrainer);
    if (infoClientWidget) infoClientWidget->setTrainer(currentTrainer);
    if (workoutDesignWidget) workoutDesignWidget->setTrainer(currentTrainer);
    if (assignWidget) assignWidget->setTrainer(currentTrainer);

}
/// @brief Actualiza los campos de encabezado con los datos del entrenador.
void TrainerBoardWidget::updateTrainerInfo()
{
    ui->IdUserLabel->setText(QString::number(currentTrainer->getId()));
    ui->userNameLabel->setText(currentTrainer->getUserName());
    ui->emailLabel->setText(currentTrainer->getEmail());
    ui->logLabel->setText(currentTrainer->getLast_login().toString("dd/MM/yyyy hh:mm"));
    ui->sinceLabel->setText(currentTrainer->getJoin_up_date().toString("dd/MM/yyyy hh:mm"));
    // QPixmap pixmap = QPixmap::fromImage(currentTrainer->getProfile_Picture());
    // ui->pictureLabel->setPixmap(pixmap);
    // ui->pictureLabel->setScaledContents(true);
    QImage profileImage = currentTrainer->getProfile_Picture();
    if (!profileImage.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(profileImage);
        ui->pictureLabel->setPixmap(pixmap);
        ui->pictureLabel->setScaledContents(true);
    } else {
        ui->pictureLabel->clear();
    }
}
/// @brief Cambia de sección en el stackedWidget según el índice del menú lateral.
void TrainerBoardWidget::sectionChanged(int index)
{

    int maxIndex = ui->stackedWidget->count() - 1;
    qDebug() << "Index:" << index << "max:" << maxIndex;

    if (index < 0 || index > maxIndex) {
        qWarning() << "//sectionChanged// Índice fuera de rango:" << index;
        return;
    }
    if (index == ui->listWidget->count() - 1) {
        emit logoutRequested();
        return;
    }
    ui->stackedWidget->setCurrentIndex(index);

    switch (index) {
        case 0:if (infoWidget) infoWidget->updateTrainerInfo();
            break;
        case 1:if (infoClientWidget) infoClientWidget->loadClientList();
            break;
        case 2:if (workoutDesignWidget) workoutDesignWidget->loadWorkoutTable();
            break;
        case 3:if (metricsWidget) { metricsWidget->updateWidgets(); }
            break;
        case 4:if (assignWidget) assignWidget->setTrainer(currentTrainer);
            //if (metricsWidget) { metricsWidget->updateWidgets(); }
            break;
        // case 5:  emit logoutRequested();
        //     break;
        default:
            break;
        }
}

void TrainerBoardWidget::populateMetricsTable()
{

}
/// @brief Muestra un mensaje modal según el tipo de mensaje de error recibido.
void TrainerBoardWidget::onUiErrorMessage(const QString& msg, QtMsgType type)
{
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        QMessageBox::information(this, "Información", msg);
        break;
    case QtWarningMsg:
        QMessageBox::warning(this, "Advertencia", msg);
        break;
    case QtCriticalMsg:
    case QtFatalMsg:
        QMessageBox::critical(this, "Error Crítico", msg);
        break;
    }
}
/// @brief Devuelve si un cliente ya está en el buffer.
bool TrainerBoardWidget::isInBuffer(QSharedPointer<Client> client) const
{
    if(client!=nullptr &&  clientBuffer.contains(client->getId())) return true;
    else return false;
}
/// @brief Devuelve si un ID de cliente está en el buffer
bool TrainerBoardWidget::isInBuffer(int id) const
{
    return clientBuffer.contains(id);
}
/// @brief Obtiene un cliente del buffer por ID.
QSharedPointer<Client> TrainerBoardWidget::getFromBuffer(int id) const
{
    return clientBuffer.value(id, nullptr);
}
/// @brief Añade un cliente al buffer y mantiene el tamaño máximo.
void TrainerBoardWidget::addToBuffer(QSharedPointer<Client> client)
{
    if (client==nullptr) return;

    int id = client->getId();

    if (!clientBuffer.contains(id)) {
        clientBuffer.insert(id, client);
        bufferOrder.enqueue(id);
    }

    int maxSize = controller->getMaxBufferSize();
    while (clientBuffer.size() > maxSize) {
        int toRemoveId = bufferOrder.dequeue();
        clientBuffer.remove(toRemoveId);
    }
}
