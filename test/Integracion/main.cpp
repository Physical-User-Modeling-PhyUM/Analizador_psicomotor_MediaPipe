#include <QTest>
#include "testintegracionuserrepositorydb.h"
#include "testintegraciontrainingrepositorydb.h"
#include "testintegracionusermanagerdb.h"
#include "testintegraciontrainingmanagerdb.h"
#include "testintegraciondummyposemanager.h"

int main(int argc, char *argv[]) {

    int status = 0;
    QCoreApplication app(argc, argv);
    //Functional_Test_Pose test;
    //test.runTests();

    // //Test Integracion de la base de datos con UserRepository
     TestIntegracionUserRepositoryDB testU_DB;
     status |= QTest::qExec(&testU_DB, argc, argv);

    // //Test Integracion de la base de datos con TrainingRepository
     TestIntegracionTrainingRepositoryDB testT_DB;
     status |= QTest::qExec(&testT_DB, argc, argv);

    // //Test Integracion de userMAnager
     TestIntegracionUserManagerDB test_U;
     status |= QTest::qExec(&test_U, argc, argv);

    // //Test Integracion de TrainingMAnager
     TestIntegracionTrainingManagerDB test_T;
     status |= QTest::qExec(&test_T, argc, argv);

    //TestdeDummyPostManager
    TestIntegracionDummyPoseManager testDummy;
    testDummy.run();

    return status;

    return 0;
}
