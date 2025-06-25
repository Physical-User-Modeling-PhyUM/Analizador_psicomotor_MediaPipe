#include "userclientprofilewidget.h"
#include "ui_userclientprofilewidget.h"



Q_LOGGING_CATEGORY(UserClientprofileWidget, "ProfileBoardWidget")

UserClientprofileWidget::UserClientprofileWidget(QSharedPointer<AppController> controller,QWidget *parent)
    : QWidget(parent),
    controller(controller),
    ui(new Ui_UserClientprofileWidget)
{
    ui->setupUi(this);
}

UserClientprofileWidget::~UserClientprofileWidget()
{
    delete ui;
}
void UserClientprofileWidget::setUser(QSharedPointer<Client> user)
{

    currentUser = user;
    //updateUserInfo();
}
