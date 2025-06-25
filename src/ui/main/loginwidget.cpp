#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QSharedPointer<LoginManager> loginManager, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::LoginWidget),
    loginManager(loginManager)
{

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->emailEdit->setText("");
    ui->passwordEdit->setText("");
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::on_pushButton_2_clicked()
{
     // ui->stackedWidget->setCurrentIndex(1);
}


void LoginWidget::on_RegisterButton_2_clicked()
{
    QString email = ui->emailEdit_Register->text();
    QString userName=ui->UserNameEdit_Register->text();
    QString password = ui->passwordEdit_Register->text();


    if(!email.isEmpty()&& !userName.isEmpty()&& !password.isEmpty()){
        QHash<UserField, QVariant> data = loginManager->registerNewClient(userName,email, password);
        emit loginManager->newClientReg(data);
         ui->stackedWidget->setCurrentIndex(0);
         }
    else  { ui->statusLabel->setText("Missing data to proceed with registration");}
}


void LoginWidget::on_RegisterButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QString email = ui->emailEdit->text();
    if (email.contains("@")) ui->emailEdit_Register->setText(email);
    else ui->UserNameEdit_Register->setText(email);
    QString password = ui->passwordEdit->text();
    ui->passwordEdit_Register->setText(password);

}


void LoginWidget::on_LoginButton_clicked()
{
    QString email = ui->emailEdit->text();
    QString password = ui->passwordEdit->text();

    auto user = loginManager->login(email, password);

    if (user) {
        ui->statusLabel->setText("logging in");
        emit loginManager->loginSuccess(user);
    } else {
        ui->statusLabel->setText("email o password missiong or incorrect");
    }
}


void LoginWidget::on_CancelButton_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

