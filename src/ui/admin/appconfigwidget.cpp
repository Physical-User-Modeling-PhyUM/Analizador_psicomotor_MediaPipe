#include "appconfigwidget.h"
#include "ui_appconfigwidget.h"

AppConfigWidget::AppConfigWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent), ui(new Ui_AppConfigWidget), controller(controller) {
    ui->setupUi(this);

    configModel = new UiTableModel(this);
    ui->tableView->setModel(configModel);

    loadConfigToForm();

    configModel->setColumnReadOnly(0); // clave no editable
    connect(ui->SaveButton, &QPushButton::clicked, this, &AppConfigWidget::onSaveClicked);
}

AppConfigWidget::~AppConfigWidget() {
    delete ui;
}

void AppConfigWidget::loadConfigToForm() {
    QHash<QString, QVariant> config = controller->getPoseCaptureConfig();

    QList<QList<QVariant>> rows;
    for (auto it = config.begin(); it != config.end(); ++it) {
        rows.append({ it.key(), it.value() });
    }

    configModel->setDataSet(rows, { "Key", "Value" });
}


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

void AppConfigWidget::onSaveClicked() {
    QHash<QString, QVariant> config = collectConfigData();
    qDebug() << "Nueva configuración:";
    for (auto it = config.begin(); it != config.end(); ++it) {
        qDebug() << it.key() << "=" << it.value();
    }
    controller->setPoseCaptureConfig(config);
}
