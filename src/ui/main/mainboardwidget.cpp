#include "mainboardwidget.h"
#include "ui_mainboardwidget.h"

MainBoardWidget::MainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent),
    ui(new Ui_MainBoardwidget),
    controller(controller)
{
    ui->setupUi(this);

    auto user = controller->getUserManager()->getCurrentUser();
    if (user) {
        QString roleText = UserTypeToString(user->getUserType());
        ui->labelTitle->setText("Bienvenido, " + user->getUserName() + " (" + roleText + ")");
    }
}


MainBoardWidget::~MainBoardWidget() {
    delete ui;
}
