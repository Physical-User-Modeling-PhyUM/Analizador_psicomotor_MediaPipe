#include "adminmainboardwidget.h"

/// @brief Implementación de la clase AdminMainBoardWidget.

/// @brief Constructor de AdminMainBoardWidget.
/// Inicializa la interfaz gráfica, crea los widgets secundarios y conecta señales para la navegación de secciones.
AdminMainBoardWidget::AdminMainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent), ui(new Ui_AdminMainBoardWidget), controller(controller) {
    ui->setupUi(this);

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &AdminMainBoardWidget::sectionChanged);

    configWidget = new AppConfigWidget(controller, this);
    usersWidget = new UsersAdminWidget(controller, this);

    ui->stackedWidget->insertWidget(0, configWidget);
    ui->stackedWidget->insertWidget(1, usersWidget);
}
/// @brief Destructor de AdminMainBoardWidget.
/// Libera los recursos de la interfaz gráfica.
AdminMainBoardWidget::~AdminMainBoardWidget() {
    delete ui;
}
/// @brief Cambia de sección en la interfaz según el índice seleccionado en la lista.
/// @param index Índice seleccionado. Si es 2, se interpreta como acción de cierre de sesión.
void AdminMainBoardWidget::sectionChanged(int index) {
    if (index == 2) {
        emit logoutRequested();
        return;
    }
    ui->stackedWidget->setCurrentIndex(index);
}
