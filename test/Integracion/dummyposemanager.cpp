#include "dummyposemanager.h"
#include <nlohmann/json.hpp>
#include <QFile>
#include <QDebug>

DummyPoseManager::DummyPoseManager(QObject *parent)
    : QObject(parent)
{}

void DummyPoseManager::setInputFolder(const QString& path) {
    inputFolder = path;
}

void DummyPoseManager::init(QSharedPointer<TrainingSesion> sesion, QSharedPointer<ExerciseEspec> espec) {

    runningSesion = sesion;
    poseAnalyzer = QSharedPointer<StateMachine>::create(espec);

    QDir dir(inputFolder);
    QStringList files;
    files << "*.json";

    QStringList frames = dir.entryList(files, QDir::Files, QDir::Name);

    QTextStream in(stdin);
    for (const QString& jsonFile : frames) {
        QString base = QFileInfo(jsonFile).completeBaseName();
        qDebug() << "\nProcesando frame:" << base << " ( ENTER para continuar)";
        in.readLine();
        processFrame(base);

        if (poseAnalyzer->isComplete()) {
            runningSesion->setReport(poseAnalyzer->getReport());
            runningSesion->setComplete(true);
            emit exerciseCompleted();
            break;
        }
    }
}

void DummyPoseManager::processFrame(const QString& baseName) {

    QString jsonPath = QDir(inputFolder).filePath(baseName + ".json");
    QString imgPath = QDir(inputFolder).filePath(baseName + ".png");

    QFile jsonFile(jsonPath);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning() << "No se pudo abrir JSON:" << jsonPath;
        return;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    nlohmann::json poseData;
    try {
        //std::string json_str(reinterpret_cast<char*>(shm + FRAME_SIZE), JSON_SIZE);
        //json_str.erase(json_str.find('\0'));
         //json_data = nlohmann::json::parse(json_str);

        poseData = nlohmann::json::parse(jsonData.toStdString());
    } catch (...) {

        qWarning() << "Error al parsear JSON";
        return;
    }

    cv::Mat image = cv::imread(imgPath.toStdString());
    if (image.empty()) {
        qWarning() << "No se pudo cargar imagen:" << imgPath;
        return;
    }

    Pose* pose = new Pose(poseData, image, connections);
    auto angles = pose->getAngles();

    qDebug() << "Ángulos del frame " << baseName;
    for (auto it = angles.constBegin(); it != angles.constEnd(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }
    int timestamp= poseData["timestamp"];

    QHash<PoseView, QHash<QString, double>> anglesByView;
    anglesByView[PoseView::Front] = angles;
    FeedBack feedback = FeedBack(poseAnalyzer->run(anglesByView, timestamp));
    // qDebug() << "Feedback generado:";
    // for (ConditionType type : feedback.getEntriesList()) {
    //     QString message = feedback.getMessage(type);
    //     qDebug() << " -" << conditionTypeToString(type) << ":" << message;
    // }
    cv::Mat imgOut = pose->drawKeypoints();
    cv::imshow("DummyPoseManager - Frame " + baseName.toStdString(), imgOut);
    cv::waitKey(0);
    cv::destroyWindow("DummyPoseManager - Frame " + baseName.toStdString());
    //emit newImage1(pose->getImage_bgr());
    //emit feedbackGenerated(feedback);

    delete pose;
}

void DummyPoseManager::stopCapture(){}

void DummyPoseManager::runAnalysis()
{
    runningAnalysis=true;
}

void DummyPoseManager::pauseAnalysis()
{
    runningAnalysis=false;
}

void DummyPoseManager::setConnections(const QHash<QPair<int, int>, QString>& conn) {
    this->connections = conn;
}
