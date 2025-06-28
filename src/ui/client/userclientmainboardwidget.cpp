/// @brief Implementación de la clase UserClientMainBoardWidget.
#include "userclientmainboardwidget.h"
#include "ui/client/userclientconfigwidget.h"
#include "ui_userclientmainboardwidget.h"
#include <QMessageBox>

/// @brief Constructor. Inicializa los widgets hijos, conecta las señales y prepara la navegación.

UserClientMainBoardWidget::UserClientMainBoardWidget(QSharedPointer<AppController> controller,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_UserClientMainBoardWidget),
    controller(controller)
{
    ui->setupUi(this);

    // Inicializar widgets y añadirlos al stackedWidget
    infoWidget = new UserClientinfoWidget(controller, this);
    ui->stackedWidget->insertWidget(0, infoWidget);


    trainingSelectionWidget = new UserClientTrainingSesionWidget(controller, this);
    ui->stackedWidget->insertWidget(1, trainingSelectionWidget);


    profileWidget = new UserClientprofileWidget(controller, this);
    ui->stackedWidget->insertWidget(2, profileWidget);

    configWidget = new UserClientConfigWidget(controller, this);
    ui->stackedWidget->insertWidget(3, configWidget);


    // Escuchar errores del repositorio
    connect(controller->getTrainingManager()->getRepository().data(),
            &TrainingRepository::ui_errorMessage,
            this,
            &UserClientMainBoardWidget::onUiErrorMessage);

    connect(ui->listWidget, &QListWidget::currentRowChanged,
            this, &UserClientMainBoardWidget::onSectionChanged);

    connect(controller->getValidationManager().data(),
            &ValidationManager::ui_errorMessage,
            this,
            &UserClientMainBoardWidget::onUiErrorMessage);

}
/// @brief Destructor.
UserClientMainBoardWidget::~UserClientMainBoardWidget()
{
    delete ui;
}
/// @brief Asigna el cliente autenticado y carga su configuración personalizada.

void UserClientMainBoardWidget::setUser(QSharedPointer<Client> user)
{

    currentUser = user;

    // Cargar su perfil desde el UserManager
    auto profile = controller->getUserManager()->getClientProfile(user->getId());

    if (profile) {
        currentUser->setProfile(profile);
        qDebug() << "Perfil cargado correctamente en el cliente";
    } else {
        qWarning() << "No se encontró el perfil del cliente";
    }

    configWidget->loadConfig();

}
/// @brief Cambia la sección visible en el stackedWidget.
/// Si se selecciona la última entrada, se interpreta como logout.
void UserClientMainBoardWidget::onSectionChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);

    //QWidget* currentPage = ui->stackedWidget->currentWidget();
    // if (currentPage) {
    //QSize size = currentPage->sizeHint();
    //ui->stackedWidget->adjustSize();
    //this->adjustSize();
    // }

    if (index == ui->listWidget->count() - 1) {
        emit logoutRequested();
        return;
    }

    if (index == 0) {
        if (infoWidget)  infoWidget->setUser(currentUser);
        infoWidget->updateUserInfo();
    }
    switch (index) {
    case 1:{

            //if (trainingSelectionWidget) trainingSelectionWidget
        trainingSelectionWidget->setUser(currentUser);

        //WorkoutSummary summary = controller->getTrainingManager()->currentWorkoutToSummary();
        //updateWorkoutInfo(summary);
        //populateExerciseTable(summary.getExercises());
        break;}
    case 2:


        profileWidget->setUser(currentUser);
        break;
    case 3:


        break;
    case 4:

        break;
    default:
        break;
    }
}
/// @brief Muestra un mensaje modal según el tipo de mensaje recibido (info, warning o error).

void UserClientMainBoardWidget::onUiErrorMessage(const QString& msg, QtMsgType type)
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

