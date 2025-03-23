#include "testpose.h"
#include "Sqlite3_Test.h"

int main(int argc, char *argv[]) {


    QCoreApplication app(argc, argv);
    //TestPose test;
    //test.runTests();

    Sqlite3_Test testDB=Sqlite3_Test ();

    testDB.createDB();


    return 0;
}
