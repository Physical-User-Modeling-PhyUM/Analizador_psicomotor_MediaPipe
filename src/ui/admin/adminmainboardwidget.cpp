#include "adminmainboardwidget.h"


AdminMainBoardWidget::AdminMainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent), ui(new Ui_AdminMainBoardWidget), controller(controller) {
    ui->setupUi(this);

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &AdminMainBoardWidget::sectionChanged);

    configWidget = new AppConfigWidget(controller, this);
    usersWidget = new UsersAdminWidget(controller, this);

    ui->stackedWidget->insertWidget(0, configWidget);
    ui->stackedWidget->insertWidget(1, usersWidget);
}

AdminMainBoardWidget::~AdminMainBoardWidget() {
    delete ui;
}

void AdminMainBoardWidget::sectionChanged(int index) {
    if (index == 2) {
        emit logoutRequested();
        return;
    }
    ui->stackedWidget->setCurrentIndex(index);
}
