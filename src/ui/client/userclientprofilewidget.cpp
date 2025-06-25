
/// @brief Implementación de la clase UserClientprofileWidget.

#include "userclientprofilewidget.h"
#include "ui_userclientprofilewidget.h"



Q_LOGGING_CATEGORY(UserClientprofileWidget, "ProfileBoardWidget")
/// @brief Constructor del widget de perfil de cliente.
/// Inicializa la interfaz y almacena el controlador.
UserClientprofileWidget::UserClientprofileWidget(QSharedPointer<AppController> controller,QWidget *parent)
    : QWidget(parent),
    controller(controller),
    ui(new Ui_UserClientprofileWidget)
{
    ui->setupUi(this);
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

    currentUser = user;
    //updateUserInfo();
}
