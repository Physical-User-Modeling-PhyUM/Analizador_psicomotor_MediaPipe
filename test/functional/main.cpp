#include <QTest>
#include "testdb.h"
#include "testuserrepository.h"
#include "testtrainingrepository.h"

int main(int argc, char *argv[]) {

    int status = 0;
    QCoreApplication app(argc, argv);
    //Functional_Test_Pose test;
    //test.runTests();

    //Test Funcional de la base de datos
    TestDB testDB;
    status |= QTest::qExec(&testDB, argc, argv);

    //Test Funcional de UserRepository
    TestUserRepository tuser;
    status |= QTest::qExec(&tuser, argc, argv);

    //Test Funcional de TrainingRepository
    TestTrainingRepository ttser;
    status |= QTest::qExec(&ttser, argc, argv);

    //TestPose testPose;
    //status |= QTest::qExec(&testPose, argc, argv);

    return status;

    return 0;
}
