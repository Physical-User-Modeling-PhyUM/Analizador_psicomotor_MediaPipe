/// @brief Implementación de la clase AppConfigWidget.
#include "appconfigwidget.h"
#include "ui_appconfigwidget.h"

/// @brief Constructor de AppConfigWidget.
/// Inicializa la interfaz, carga la configuración actual y conecta el botón de guardado.
AppConfigWidget::AppConfigWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent), ui(new Ui_AppConfigWidget), controller(controller) {
    ui->setupUi(this);

    configModel = new UiTableModel(this);
    ui->tableView->setModel(configModel);

    loadConfigToForm();

    configModel->setColumnReadOnly(0); // clave no editable
    connect(ui->SaveButton, &QPushButton::clicked, this, &AppConfigWidget::onSaveClicked);
}
/// @brief Destructor de AppConfigWidget.
/// Libera los recursos de la interfaz.
AppConfigWidget::~AppConfigWidget() {
    delete ui;
}
/// @brief Carga los datos de configuración actuales desde el controlador y los muestra en la tabla.
void AppConfigWidget::loadConfigToForm() {
    QHash<QString, QVariant> config = controller->getPoseCaptureConfig();

    QList<QList<QVariant>> rows;
    for (auto it = config.begin(); it != config.end(); ++it) {
        rows.append({ it.key(), it.value() });
    }

    configModel->setDataSet(rows, { "Key", "Value" });
}

/// @brief Recolecta los datos introducidos por el usuario desde la tabla.
/// @return Hash con las claves y valores actualizados.
QHash<QString, QVariant> AppConfigWidget::collectConfigData() {
    QHash<QString, QVariant> updatedConfig;
    const auto rows = configModel->getDataSet();

    for (const auto& row : rows) {
        QString key = row.at(0).toString();
        QVariant val = row.at(1);
        updatedConfig.insert(key, val);
    }
    return updatedConfig;
}
/// @brief Aplica los cambios realizados en la tabla a la configuración del sistema.
/// También imprime por consola las nuevas claves y valores.
void AppConfigWidget::onSaveClicked() {
    QHash<QString, QVariant> config = collectConfigData();
    qDebug() << "Nueva configuración:";
    for (auto it = config.begin(); it != config.end(); ++it) {
        qDebug() << it.key() << "=" << it.value();
    }
    controller->setPoseCaptureConfig(config);
}
