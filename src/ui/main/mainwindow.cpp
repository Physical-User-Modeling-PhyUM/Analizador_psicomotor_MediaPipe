#include "ui/main/mainwindow.h"

#include <QDialog>
#include <QMessageBox>



MainWindow::MainWindow(QSharedPointer<AppController> controller, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), appController(controller)
{
    ui->setupUi(this);

    // Iniciar controlador de la aplicación
    //appController = QSharedPointer<AppController>::create();
    //appController->initialize();

    // Creamos las pantallas de la aplicación
    loginWidget = new LoginWidget(appController->getLoginManager());
     clientBoard = new UserClientMainBoardWidget(appController);
    trainerBoard = new TrainerBoardWidget(appController);
    adminBoard = new AdminMainBoardWidget(appController);

    // Añadirlas al QStackedWidget del .ui
    ui->centralStack->addWidget(loginWidget);
    ui->centralStack->addWidget(clientBoard);
    ui->centralStack->addWidget(trainerBoard);
    ui->centralStack->addWidget(adminBoard);

    ui->centralStack->setCurrentWidget(loginWidget);
    resize(loginWidget->size());
    //showLoginScreen();

    // Conectar señales del UserManager
    connect(appController->getUserManager().data(), &UserManager::userLoggedIn,this, &MainWindow::onLoginSuccess);
    connect(appController->getUserManager().data(), &UserManager::userLoggedOut,this, &MainWindow::showLoginScreen);
    connect(trainerBoard, &TrainerBoardWidget::logoutRequested,appController->getUserManager().data(), &UserManager::logout);
    connect(clientBoard, &UserClientMainBoardWidget::logoutRequested,appController->getUserManager().data(), &UserManager::logout);
    connect(adminBoard, &AdminMainBoardWidget::logoutRequested,appController->getUserManager().data(), &UserManager::logout);


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLoginSuccess(QSharedPointer<User> user) {

   // mostrarImagenEnPopUp(user->getProfile_Picture());
    switch (user->getUserType()) {
    case UserType::Client: {
        resize(clientBoard->size());

        QSharedPointer<Client> client = qSharedPointerCast<Client>(user);
        clientBoard->setUser(client);

        ui->centralStack->setCurrentWidget(clientBoard);

        break;
    }
    case UserType::Trainer:{
        resize(trainerBoard->size());
        QSharedPointer<FitnessTrainer> trainer = qSharedPointerCast<FitnessTrainer>(user);
        trainerBoard->setUser(trainer);
        ui->centralStack->setCurrentWidget(trainerBoard);

        break;
    }
    case UserType::Admin:
         ui->centralStack->setCurrentWidget(adminBoard);
        break;
    case UserType::Unknown:
        break;
    }

}

void MainWindow::showLoginScreen() {
     loginWidget->clearFields();
    ui->centralStack->setCurrentWidget(loginWidget);
}


void MainWindow::mostrarImagenEnPopUp(QImage image) {
    // if (image.isNull()) {
    //     QMessageBox::warning(nullptr, "Imagen no válida", "La imagen no pudo cargarse correctamente.");
    //     return;
    // }

    // QDialog* dialog = new QDialog();
    // dialog->setWindowTitle("Imagen de perfil cargada");
    // dialog->resize(image.width() + 20, image.height() + 20);

    // QLabel* label = new QLabel(dialog);
    // label->setPixmap(QPixmap::fromImage(image));
    // label->setAlignment(Qt::AlignCenter);

    // QVBoxLayout* layout = new QVBoxLayout(dialog);
    // layout->addWidget(label);
    // dialog->setLayout(layout);

    // dialog->exec(); // Modal

}
