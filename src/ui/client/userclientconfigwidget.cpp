/// @brief Implementación de la clase UserClientConfigWidget.
#include "userclientconfigwidget.h"
#include <QComboBox>
#include <QDebug>
#include <nlohmann/json.hpp>

/// @brief Constructor. Configura la interfaz y conecta el botón de guardado.
UserClientConfigWidget::UserClientConfigWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent), ui(new Ui_UserClientConfigWidget), controller(controller)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &UserClientConfigWidget::saveUserPreferences);
    //loadConfig();
}
/// @brief Destructor.
UserClientConfigWidget::~UserClientConfigWidget()
{
    delete ui;
}
/// @brief Carga las preferencias del usuario desde el sistema y las muestra en el formulario.
void UserClientConfigWidget::loadConfig()
{
\
    auto currentUser = controller->getUserManager()->getCurrentUser();
    if (currentUser.isNull()) {
        qWarning() << "[UserClientConfigWidget] currentUser is null. Cannot load preferences.";
        return;
    }

    int userId = currentUser->getId();
    UserPreferences prefs = controller->getUserManager()->getUserPreferences(userId);



    // Cargar preferencias
    ui->dualModeCheck->setChecked(prefs.getDualMode());
    ui->infoSoundCheck->setChecked(prefs.getInfoSound());
    ui->alertSoundCheck->setChecked(prefs.getAlertSound());
    ui->muteCheck->setChecked(prefs.getMute());

    // Cargar conexiones
    QHash<QPair<int, int>, QString> conns = prefs.getConnections();
    if (conns.isEmpty())conns =controller->getConnections();

    QHash<int, QString> keypoints = controller->getKeypoints();

    ui->connectionsTable->setRowCount(0);

    for (auto it = conns.begin(); it != conns.end(); ++it) {
        int row = ui->connectionsTable->rowCount();
        ui->connectionsTable->insertRow(row);

        QComboBox* comboA = new QComboBox;
        QComboBox* comboB = new QComboBox;

        for (auto kIt = keypoints.begin(); kIt != keypoints.end(); ++kIt) {
            comboA->addItem(kIt.value(), kIt.key());
            comboB->addItem(kIt.value(), kIt.key());
        }
        int a=comboA->findData(it.key().first);
        int b=comboB->findData(it.key().second);
        comboA->setCurrentIndex(a);
        comboB->setCurrentIndex(b);

        ui->connectionsTable->setCellWidget(row, 0, comboA);
        ui->connectionsTable->setCellWidget(row, 1, comboB);
    }


    int lastRow = ui->connectionsTable->rowCount();
    ui->connectionsTable->insertRow(lastRow);

    QComboBox* newComboA = new QComboBox;
    QComboBox* newComboB = new QComboBox;

    for (auto k_It = keypoints.begin(); k_It != keypoints.end(); ++k_It) {
        newComboA->addItem(k_It.value(), k_It.key());
        newComboB->addItem(k_It.value(), k_It.key());
    }
    ui->connectionsTable->setCellWidget(lastRow, 0, newComboA);
    ui->connectionsTable->setCellWidget(lastRow, 1, newComboB);
}


/// @brief Guarda las preferencias del usuario, incluyendo opciones y conexiones entre keypoints.
/// Almacena las conexiones en formato JSON y actualiza los datos en el sistema.
void UserClientConfigWidget::saveUserPreferences()
{
    UserPreferences prefs;
    int userId = controller->getUserManager()->getCurrentUser()->getId();
    prefs.setIdUser(userId);
    prefs.setDualMode(ui->dualModeCheck->isChecked());
    prefs.setInfoSound(ui->infoSoundCheck->isChecked());
    prefs.setAlertSound(ui->alertSoundCheck->isChecked());
    prefs.setMute(ui->muteCheck->isChecked());

    nlohmann::json jsonConnections = nlohmann::json::array();

    for (int i = 0; i < ui->connectionsTable->rowCount(); ++i) {
        auto* comboA = qobject_cast<QComboBox*>(ui->connectionsTable->cellWidget(i, 0));
        auto* comboB = qobject_cast<QComboBox*>(ui->connectionsTable->cellWidget(i, 1));
        if (!comboA || !comboB) continue;

        int idA = comboA->currentData().toInt();
        int idB = comboB->currentData().toInt();

        if (idA == idB || idA == 0 || idB == 0) continue;

        jsonConnections.push_back({idA, idB});
    }

    prefs.setConnectionsJson(QString::fromStdString(jsonConnections.dump()));

    controller->getUserManager()->updateUserPreferences(prefs);
    controller->setPoseCaptureConfig(controller->getPoseCaptureConfig());
}


// void UserClientConfigWidget::populateConnectionsTable()
// {
//     ui->connectionsTable->setRowCount(0);
//     auto keypoints = controller->getKeypoints();

//     for (auto it = controller->getConnections().begin(); it != controller->getConnections().end(); ++it) {
//         int row = ui->connectionsTable->rowCount();
//         ui->connectionsTable->insertRow(row);

//         QComboBox* comboA = new QComboBox;
//         QComboBox* comboB = new QComboBox;

//         for (auto kIt = keypoints.begin(); kIt != keypoints.end(); ++kIt) {
//             comboA->addItem(kIt.value(), kIt.key());
//             comboB->addItem(kIt.value(), kIt.key());
//         }

//         comboA->setCurrentIndex(comboA->findData(it.key().first));
//         comboB->setCurrentIndex(comboB->findData(it.key().second));

//         ui->connectionsTable->setCellWidget(row, 0, comboA);
//         ui->connectionsTable->setCellWidget(row, 1, comboB);
//     }

//     // Añadir una fila vacía para permitir nuevas conexiones
//     int lastRow = ui->connectionsTable->rowCount();
//     ui->connectionsTable->insertRow(lastRow);
//     QComboBox* newComboA = new QComboBox;
//     QComboBox* newComboB = new QComboBox;
//     for (auto kIt = keypoints.begin(); kIt != keypoints.end(); ++kIt) {
//         newComboA->addItem(kIt.value(), kIt.key());
//         newComboB->addItem(kIt.value(), kIt.key());
//     }
//     ui->connectionsTable->setCellWidget(lastRow, 0, newComboA);
//     ui->connectionsTable->setCellWidget(lastRow, 1, newComboB);
// }
