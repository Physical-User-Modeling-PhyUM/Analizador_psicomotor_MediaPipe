#include "testintegraciondummyposemanager.h"
#include<QCoreApplication>
#include "core/dbmanager.h"
#include "dummyposemanager.h"
#include "core/trainingmanager.h"
#include "repositories/trainingrepository.h"
#include <fstream>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(TestDummyPoseLog, "Controller")

TestIntegracionDummyPoseManager::TestIntegracionDummyPoseManager() {}

void TestIntegracionDummyPoseManager::run(){


    loadConfig();

    QString dbPath = QCoreApplication::applicationDirPath() + "/data.db";

    bool dbExists = QFile::exists(dbPath);
   auto  dbManager = QSharedPointer<DBManager>::create(dbPath);

    auto trainingRepo = QSharedPointer<TrainingRepository>::create(dbManager.data());
   auto  trainingManager = QSharedPointer<TrainingManager>::create(trainingRepo);





   QSharedPointer<ExerciseEspec> espec =trainingManager->getExercise(101);
   QSharedPointer<TrainingSesion> sesion = QSharedPointer<TrainingSesion>::create(
       QSharedPointer<ExerciseSummary>::create(espec->toSummary()),
       QDateTime::currentDateTime(),
       1
       );
   DummyPoseManager dummy;
   // QVector<QPair<int, int>> connList;
   // for (const auto& key : connections) {
   //     connList.append(key);
   // }
   dummy.setConnections(connections);
    dummy.setInputFolder("/Users/MZT/pfg/test/images/test");



    dummy.init(sesion, espec);


}
void TestIntegracionDummyPoseManager::loadConfig() {


    QString configPath = QCoreApplication::applicationDirPath() + "/config/poseConfig.json";
    qDebug(TestDummyPoseLog) << "Buscando config.json en:" << configPath;

    std::ifstream file(configPath.toStdString());
    if (!file.is_open()) {
        qCritical(TestDummyPoseLog) << "No se pudo abrir config.json";
        return;
    }

    nlohmann:: json config;
    file >> config;
    file.close();

    QStringList requiredKeys = {"WIDTH", "HEIGHT", "JSON_SIZE", "FLAG_SIZE", "PYTHON_SCRIPT", "CAM1", "CAM2", "SEM_SHM1", "SEM_SHM2"};

    for (const QString& key : requiredKeys) {
        if (!config.contains(key)) {
            qCritical(TestDummyPoseLog) << "Falta la clave en poseConfig.json:" << key;
            exit(1);
        }

    }


    poseCaptureConfig.insert("WIDTH", config["WIDTH"].get<int>());
    poseCaptureConfig.insert("HEIGHT", config["HEIGHT"].get<int>());
    poseCaptureConfig.insert("JSON_SIZE", config["JSON_SIZE"].get<int>());
    poseCaptureConfig.insert("FLAG_SIZE", config["FLAG_SIZE"].get<int>());
    poseCaptureConfig.insert("PYTHON_SCRIPT", QString::fromStdString(config["PYTHON_SCRIPT"]));
    poseCaptureConfig.insert("CAM1", QString::fromStdString(config["CAM1"]));
    poseCaptureConfig.insert("CAM2", QString::fromStdString(config["CAM2"]));
    poseCaptureConfig.insert("SEM_SHM1", QString::fromStdString(config["SEM_SHM1"]));
    poseCaptureConfig.insert("SEM_SHM2", QString::fromStdString(config["SEM_SHM2"]));

    if (config.contains("DUALMODE")) {
        // QString mode= QString::fromStdString(config["DUALMODE"]).toLower();
        // if (mode=="true") dualmode=true;
        // else dualmode=false;
         poseCaptureConfig.insert("DUALMODE", config["DUALMODE"].get<bool>());
    }
    if (config.contains("BUFFER_SIZE")) {maxBufferSize=config["BUFFER_SIZE"].get<int>();}


    if (config.contains("KEYPOINTS")) {
        nlohmann:: json keys = config["KEYPOINTS"];
        for (auto it = keys.begin(); it != keys.end(); ++it) {
            int key = std::stoi(it.key());
            QString name = QString::fromStdString(it.value());
            keypoints.insert(key, name);
        }
    }


    if (config.contains("CONNECTIONS")) {
        nlohmann:: json connArray = config["CONNECTIONS"];
        for (const auto& conn : connArray) {
            int from = conn[0];
            int to = conn[1];
            QString fromName = keypoints.value(from, QString::number(from));
            QString toName = keypoints.value(to, QString::number(to));
            QString name = fromName + "<->" + toName;
            connections.insert(qMakePair(from, to), name);
        }
    }

    // Mostrar valores cargados
    qDebug(TestDummyPoseLog) << "PYTHON_SCRIPT:" << poseCaptureConfig["PYTHON_SCRIPT"].toString();
    qDebug(TestDummyPoseLog) << "CAM_1:" << poseCaptureConfig["CAM1"].toString();
    qDebug(TestDummyPoseLog) << "CAM_2:" << poseCaptureConfig["CAM2"].toString();
    qDebug(TestDummyPoseLog) << "WIDTH:" << poseCaptureConfig["WIDTH"].toInt();
    qDebug(TestDummyPoseLog) << "HEIGHT:" << poseCaptureConfig["HEIGHT"].toInt();
    qDebug(TestDummyPoseLog) << "JSON_SIZE:" << poseCaptureConfig["JSON_SIZE"].toInt();
    qDebug(TestDummyPoseLog) << "FLAG_SIZE:" << poseCaptureConfig["FLAG_SIZE"].toInt();
    qDebug(TestDummyPoseLog) << "SEM_SHM1:" << poseCaptureConfig["SEM_SHM1"].toString();
    qDebug(TestDummyPoseLog) << "SEM_SHM2:" << poseCaptureConfig["SEM_SHM2"].toString();

    qDebug(TestDummyPoseLog) << "CONNECTIONS:";
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        qDebug(TestDummyPoseLog) << "(" << it.key().first << "," << it.key().second << ") ->" << it.value();
    }
    qDebug(TestDummyPoseLog) << "==============================";

}
