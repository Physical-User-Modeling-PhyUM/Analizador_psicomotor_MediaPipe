#include "testintegracionusermanagerdb.h"
#include "profiles/client.h"

TestIntegracionUserManagerDB::TestIntegracionUserManagerDB()
    : db(new DBManager( QCoreApplication::applicationDirPath() + "test_usermanager.db")),
    repo(new UserRepository(db)),
    userManager(new UserManager(QSharedPointer<UserRepository>(repo))),
    testClientId(-1)
{}

void TestIntegracionUserManagerDB::initTestCase()
{
    QVERIFY(db->initializeSchema());
    //creamos un usuario nuevo
    QHash<UserField, QVariant> data;
    data[UserField::IdUser] = -1;
    data[UserField::UserName] = "c200";
    data[UserField::Email] = "c200@t.com";
    data[UserField::Pass] = "pass200";

    QSharedPointer<ClientProfile> profile(new ClientProfile());


    QVERIFY(userManager->createClient(data, profile));


    qDebug()<<"Tablas de la Base de datos para Usermanager";

    db->get(TypeBDEnum::User).print();
    db->get(TypeBDEnum::Client).print();
    //db->get(TypeBDEnum::Admin).print();
    //db->get(TypeBDEnum::Trainer).print();
    //db->get(TypeBDEnum::ProfileFunctional).print();
    //db->get(TypeBDEnum::ProfileGeneral).print();
    //db->get(TypeBDEnum::ProfileLifestyle).print();
    //db->get(TypeBDEnum::ProfileMedical).print();
    //db->get(TypeBDEnum::ProfileMorphology).print();
    //db->get(TypeBDEnum::ProfileROM).print();

    testClientId = 1;
}

void TestIntegracionUserManagerDB::testCreateClient()
{
    QSharedPointer<User> c = userManager->getUser(testClientId);
    QVERIFY(c!=nullptr);
    QCOMPARE(c->getUserName(), "c200");
    QCOMPARE(c->getEmail(), "c200@t.com");
}

void TestIntegracionUserManagerDB::testAssignAndGetWorkout()
{

    userManager->loging_As(testClientId);

    QDateTime today = QDateTime::currentDateTime();
    QVERIFY(userManager->assignWorkoutToClient(today, 99));
    QCOMPARE(userManager->getUserWorkout(today), 99);

    QVERIFY(userManager->unassignWorkoutFromClient(today));
    QCOMPARE(userManager->getUserWorkout(today), -1);
    userManager->logout();

}

void TestIntegracionUserManagerDB::testUpdateAndGetClientProfile()
{
    QVERIFY(userManager->loging_As(testClientId));

    QSharedPointer<ClientProfile> profile = userManager->getClientProfile(testClientId);

    QVERIFY(profile);
    QVERIFY(profile->setRecord(GeneralDataField::Age, 35));
    userManager->updateClientProfile(profile);

    QSharedPointer<ClientProfile> updatedProfile = userManager->getClientProfile(testClientId);
    QCOMPARE(updatedProfile->getRecord(GeneralDataField::Age).toInt(), 35);
    userManager->logout();
}

void TestIntegracionUserManagerDB::cleanupTestCase()
{
    // QVERIFY(repo->deleteUser(testClientId));
    // delete userManager;
    // delete repo;
    // delete db;
}
