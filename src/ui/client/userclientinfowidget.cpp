/// @brief Implementación de la clase UserClientinfoWidget.
#include "userclientinfowidget.h"
#include "ui_userclientinfowidget.h"
#include <QFileDialog>
#include <QMessageBox>

Q_LOGGING_CATEGORY(UserClientBoard, "UserClientBoardWidget")

/// @brief Constructor. Inicializa la interfaz y conecta la acción del botón de cambiar foto.

UserClientinfoWidget::UserClientinfoWidget(QSharedPointer<AppController> controller,QWidget *parent)
    : QWidget(parent),
  controller(controller),
    ui(new Ui_UserClientinfoWidget)
{
    ui->setupUi(this);
    connect(ui->newPhoto, &QPushButton::clicked, this, &UserClientinfoWidget::on_newPhoto_clicked);
}
/// @brief Destructor.
UserClientinfoWidget::~UserClientinfoWidget()
{
    delete ui;
}

/// @brief Actualiza en la interfaz los datos del usuario cliente, incluyendo su imagen de perfil.

void UserClientinfoWidget::updateUserInfo()
{
    if (!userClient) return;

    ui->IdUser_label->setText(QString::number(userClient->getId()));
    ui->UserName_label->setText(userClient->getUserName());
    ui->Email_label->setText(userClient->getEmail());
    ui->Plan_label->setText(userClient->getPlan());
    ui->lastLogging_label->setText(userClient->getLast_login().toString("dd/MM/yyyy hh:mm"));
    ui->user_since_label->setText(userClient->getJoin_up_date().toString("dd/MM/yyyy hh:mm"));
    QPixmap pixmap = QPixmap::fromImage(userClient->getProfile_Picture());
    ui->picture_label->setPixmap(pixmap);
    ui->picture_label->setScaledContents(true);


    qDebug(UserClientBoard)<<"cargado en ClientBoardWidget"<<userClient->getId()<<userClient->getUserName()
                        <<userClient->getLast_login();

}
/// @brief Asigna el usuario cliente que se mostrará en la interfaz.

void UserClientinfoWidget::setUser(QSharedPointer<Client> user)
{

    userClient = user;
    updateUserInfo();
}

/// @brief Permite al usuario seleccionar una nueva imagen de perfil desde el sistema de archivos.
/// La imagen se guarda en el perfil del usuario y se actualiza visualmente.
void UserClientinfoWidget::on_newPhoto_clicked()
{
    if (!userClient) return;

    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Selecciona una imagen de perfil"),
                                                    QString(),
                                                    tr("Imágenes (*.png *.jpg *.jpeg *.bmp)"));

    if (filePath.isEmpty())
        return;

    QImage image;
    if (!image.load(filePath)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo cargar la imagen."));
        return;
    }

    userClient->setProfile_Picture(image);
    controller->getUserManager()->updateUser(userClient);

    QPixmap pixmap = QPixmap::fromImage(image);
    ui->picture_label->setPixmap(pixmap);
    ui->picture_label->setScaledContents(true);

    QMessageBox::information(this, tr("Éxito"), tr("Imagen actualizada correctamente."));

}

