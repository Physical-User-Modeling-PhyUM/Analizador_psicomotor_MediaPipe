#include <QTest>
#include "testadmin.h"
# include "testclient.h"
#include "testclientprofile.h"
#include "testdb.h"
#include "testfitnesstrainer.h"
#include "testpose.h"
#include "Sqlite3_Test.h"
#include "testexerciseespec.h"
#include "testdbtable.h"
# include "testtrainingworkout.h"
#include "testuser.h"
#include "testuserpreferences.h"
#include "testworkoutsummary.h"

int main(int argc, char *argv[]) {

    int status = 0;
    QCoreApplication app(argc, argv);
    //TestPose test;
    //test.runTests();
    //Sqlite3_Test testDB=Sqlite3_Test ();
    //testDB.createDB();

    //Test de BD
    TestDB testDB;
    status |= QTest::qExec(&testDB, argc, argv);

    //Test de las clases de DB
    TestDBTable testDBTable;
    status |= QTest::qExec(&testDBTable, argc, argv);


    //Test de las clases de Workout
    TestExerciseEspec testExerciseEspec;
    status |= QTest::qExec(&testExerciseEspec, argc, argv);
    TestTrainingWorkout testTrainingWorkout;
      status |= QTest::qExec(&testTrainingWorkout, argc, argv);
    TestWorkoutSummary testWorkoutSummary;
     status |= QTest::qExec(&testWorkoutSummary, argc, argv);
     // TrainingSesion y ExserciseSummary no se testea todos sus métodos son getters o setters sencillos.

    //Test de las clases de Profile
    TestUser testUser;
     status |= QTest::qExec(&testUser, argc, argv);
    TestClient testClient;
    status |= QTest::qExec(&testClient, argc, argv);
    TestFitnessTrainer testFitnessTrainer;
    status |= QTest::qExec(&testFitnessTrainer, argc, argv);
    TestAdmin testAdmin;
    status |= QTest::qExec(&testAdmin, argc, argv);
    TestClientProfile testClientProfile;
    status |= QTest::qExec(&testClientProfile, argc, argv);
    TestUserPreferences testUserPreferences;
    status |= QTest::qExec(&testUserPreferences, argc, argv);

     //Tests de las clases de pose
    //TestPose testPose;
    //status |= QTest::qExec(&testPose, argc, argv);

    return status;

    return 0;
}
