#include <QApplication>
#include <QTimer>
#include <iostream>
#include "../../src/pose/posemanager.h"

class TestPoseManager : public QObject {


public:
    TestPoseManager(QObject *parent = nullptr) : QObject(parent) {}

    void runTests() {
        manager = new PoseManager(nullptr, false);
        qDebug() << "Iniciando test de PoseManager...";
        manager->init();


        // Detener la prueba después de 10 segundos
       // QTimer::singleShot(10000, this, &TestPoseManager::stopTest);
    }

private slots:
    void startTest() {

    }

    void stopTest() {


    }


private:
    PoseManager *manager;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TestPoseManager test;
    test.runTests();

    return app.exec();
}
