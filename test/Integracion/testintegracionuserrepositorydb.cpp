/**
 * @file test_integracion_userrepository_db.cpp
 * @brief Implementación de pruebas de integración para UserRepository usando base de datos SQLite real.
 */

#include "testintegracionuserrepositorydb.h"
#include "profiles/client.h"
#include "profiles/fitnesstrainer.h"
#include "profiles/admin.h"
#include <QDebug>
#include <QTest>

/**
 * @brief Inicializa la base de datos y el repositorio de usuarios.
 */
void TestIntegracionUserRepositoryDB::initTestCase() {

    dbPath = "testdb.sqlite";
    QFile::remove(dbPath);
    db = new DBManager(dbPath);
    QVERIFY(db->openDatabase(dbPath));
    QVERIFY(db->initializeSchema());

    qDebug()<<"Tablas de la Base de datos para UserRepoaitory";
    db->get(TypeBDEnum::User).print();
    db->get(TypeBDEnum::Client).print();
    db->get(TypeBDEnum::Admin).print();
    db->get(TypeBDEnum::Trainer).print();
    db->get(TypeBDEnum::ProfileFunctional).print();
    db->get(TypeBDEnum::ProfileGeneral).print();
    db->get(TypeBDEnum::ProfileLifestyle).print();
    db->get(TypeBDEnum::ProfileMedical).print();
    db->get(TypeBDEnum::ProfileMorphology).print();
    db->get(TypeBDEnum::ProfileROM).print();

    repo = new UserRepository(db);
}

/**
 * @brief Limpia los recursos utilizados tras las pruebas.
 */
void TestIntegracionUserRepositoryDB::cleanupTestCase() {
    delete repo;
    db->closeDatabase();
    delete db;
    QFile::remove(dbPath);
}
/**
 * @test Verifica que se puede guardar un usuario tipo Client y recuperarlo correctamente desde la base de datos.
 */
void TestIntegracionUserRepositoryDB::test_SaveClient() {
    //VAmos a crear un cliente con datos incompletos
    qDebug() << "Creamos cliente incompleto";
    QHash<UserField, QVariant> data0;
    data0[UserField::IdUser] = -1;
    data0[UserField::UserName] = "C_incompleto";
    data0[UserField::Pass] = "Pass_incompleto";
    QSharedPointer<Client> client0(new Client(data0));
    client0->setUserType(UserType::Client);


    //VAmos a crear un cliente con datos incompletos
    qDebug() << "Creamos cliente sin tipo";
    QHash<UserField, QVariant> data1;
    data1[UserField::IdUser] = -1;
    data1[UserField::UserName] = "C_no_tipo";
    data1[UserField::Pass] = "Pass_no_tipo";
    QSharedPointer<Client> client1(new Client(data1));
    //client0->setUserType(UserType::Client);

    //VAmos a crear un cliente con datos justos
    qDebug() << "Creamos cliente completo";
    QHash<UserField, QVariant> data2;
    data2[UserField::IdUser] = -1;
    data2[UserField::UserName] = "C_completo";
    data2[UserField::Email] = "completo@c.com";
    data2[UserField::Pass] = "Pass_completo";
    QSharedPointer<Client> client2(new Client(data2));


    //VAmos a crear un cliente con datos completos sin profile
    qDebug() << "Creamos cliente con  todo completo sin profile";
    QHash<UserField, QVariant> data3;
    data3[UserField::IdUser] = -1;
    data3[UserField::UserName] = "C_todo";
     data3[UserField::Email] = "todo@c.com";
    data3[UserField::Pass] = "Pass_todo_completo";
    QSharedPointer<Client> client3(new Client(data3));
    client3->setUserType(UserType::Client);
    client3->setJoin_up_date(QDateTime::currentDateTime());
    client3->setLast_login(QDateTime::currentDateTime());
    client3->setWorkout(QDateTime::currentDateTime(), 999);


    //VAmos a crear un cliente con datos completos con profile
    qDebug() << "Creamos cliente con  todo completo";
    QHash<UserField, QVariant> data4;
    data4[UserField::IdUser] = -1;
    data4[UserField::UserName] = "C_todo con profile";
    data4[UserField::Email] = " con_profile@c.com";
    data4[UserField::Pass] = "Pass_todo_ con profile";

    QSharedPointer<Client> client4(new Client(data4));

    client4->setUserType(UserType::Client);
    client4->setJoin_up_date(QDateTime::currentDateTime());
    client4->setLast_login(QDateTime::currentDateTime());
    client4->setWorkout(QDateTime::currentDateTime(), 555);

    QSharedPointer<ClientProfile> profile(new ClientProfile);
    profile->setIdClient(-1);
    profile->setRecord(GeneralDataField::Age, 30);
    profile->setRecord(MorfologicalFields::FatPercentage, 18.5);
    client4->setProfile(profile);

    //VAmos a crear un cliente con datos completos con id
    qDebug() << "Creamos cliente con id100";
    QHash<UserField, QVariant> data5;
    data5[UserField::IdUser] = 100;
    data5[UserField::UserName] = "C_id=100";
    data5[UserField::Email] = " 100@c.com";
    data5[UserField::Pass] = "Pass_id=100";

    QSharedPointer<Client> client5(new Client(data5));

    client5->setUserType(UserType::Client);
    client5->setJoin_up_date(QDateTime::currentDateTime());
    client5->setLast_login(QDateTime::currentDateTime());
    client5->setWorkout(QDateTime::currentDateTime(), 555);

    QSharedPointer<ClientProfile> profile2(new ClientProfile);
    profile2->setIdClient(-1);
    profile2->setRecord(GeneralDataField::Height, 180);
    profile2->setRecord(MorfologicalFields::BMI, 20);
    client5->setProfile(profile2);

    QVERIFY(repo->saveUser(client5));
    QVERIFY(repo->saveUser(client4));
    QVERIFY(repo->saveUser(client3));
    QVERIFY(repo->saveUser(client2));
    QVERIFY(!repo->saveUser(client1));
    QVERIFY(!repo->saveUser(client0));

     qDebug() << "Tabla USERS tras guardar";
     db->get(TypeBDEnum::User).print();
     qDebug() << "Tabla CLIENTS tras guardar";
     db->get(TypeBDEnum::Client).print();
     qDebug() << "Tabla ProfileGeneral tras guardar";
     db->get(TypeBDEnum::ProfileGeneral).print();
     qDebug() << "Tabla ProfileMorphology tras guardar";
     db->get(TypeBDEnum::ProfileMorphology).print();




}
/**
 * @test Verifica que se puede guardar un usuario tipo Client y recuperarlo correctamente desde la base de datos.
 */
void TestIntegracionUserRepositoryDB::test_SaveUserAndGetUserById() {

    QHash<UserField, QVariant> data;
    data[UserField::IdUser] = -1;
    data[UserField::UserName] = "ana";
    data[UserField::Email] = "ana@email.com";
    data[UserField::Pass] = "abc";

    QSharedPointer<Client> client(new Client(data));
    client->setUserType(UserType::Client);
    client->setJoin_up_date(QDateTime::currentDateTime());
    client->setLast_login(QDateTime::currentDateTime());

    QSharedPointer<ClientProfile> profile(new ClientProfile);
    profile->setIdClient(-1);
    profile->setRecord(GeneralDataField::Age, 30);
    profile->setRecord(MorfologicalFields::FatPercentage, 18.5);
    client->setProfile(profile);
    client->setWorkout(QDateTime::currentDateTime(), 999);

    qDebug()<<"Grabando cliente con id:"<<client->getId()<<",Name:"<<client->getUserName();

    QVERIFY(repo->saveUser(client));

    qDebug() << "Tabla USERS tras guardar";
    db->get(TypeBDEnum::User).print();
    qDebug() << "Tabla CLIENTS tras guardar";
    db->get(TypeBDEnum::Client).print();
    qDebug() << "Tabla ProfileGeneral tras guardar";
    db->get(TypeBDEnum::ProfileGeneral).print();
    qDebug() << "Tabla ProfileMorphology tras guardar";
    db->get(TypeBDEnum::ProfileMorphology).print();

    QSharedPointer<User> u = repo->getUserById(4);
    QVERIFY(u != nullptr);
    QCOMPARE(u->getUserName(), QString("ana"));
}

/**
 * @test Verifica que se puede eliminar correctamente un usuario existente.
 */
void TestIntegracionUserRepositoryDB::test_DeleteUser() {
    bool eliminado = repo->deleteUser(1);
    QVERIFY(eliminado);

    qDebug() << "[Tabla USERS tras eliminar]";
    db->get(TypeBDEnum::User).print();
    qDebug() << "[Tabla CLIENTS tras eliminar]";
    db->get(TypeBDEnum::Client).print();
    qDebug() << "[Tabla ProfileGeneral tras eliminar]";
    db->get(TypeBDEnum::ProfileGeneral).print();

    QSharedPointer<User> u = repo->getUserById(1);

    QVERIFY(u == nullptr);
}

/**
 * @test Verifica que se puede guardar un usuario tipo Admin correctamente.
 */
void TestIntegracionUserRepositoryDB::test_SaveAdmin() {
    QHash<UserField, QVariant> data;
    data[UserField::IdUser] = 2;
    data[UserField::UserName] = "root";
    data[UserField::Email] = "root@email.com";
    data[UserField::Pass] = "secure";

    QSharedPointer<Admin> admin(new Admin(data));
    admin->setUserType(UserType::Admin);
    admin->setJoin_up_date(QDateTime::currentDateTime());
    admin->setLast_login(QDateTime::currentDateTime());
    admin->setRoleDescription("Admin master");
    admin->setPermissionLevel(5);
    admin->setCanManage(true);
    admin->setCanModifyTrainingPlans(true);

    QVERIFY(repo->saveUser(admin));

    qDebug() << "[Tabla USERS tras guardar admin]";
    db->get(TypeBDEnum::User).print();
    qDebug() << "[Tabla ADMINS tras guardar admin]";
    db->get(TypeBDEnum::Admin).print();
}

/**
 * @test Verifica que se puede obtener el perfil de cliente completo correctamente.
 */
void TestIntegracionUserRepositoryDB::test_GetClientProfile() {
    QHash<UserField, QVariant> data;
    data[UserField::IdUser] = 3;
    data[UserField::UserName] = "carlos";
    data[UserField::Email] = "c@correo.com";
    data[UserField::Pass] = "clave";

    QSharedPointer<Client> client(new Client(data));
    client->setUserType(UserType::Client);
    client->setJoin_up_date(QDateTime::currentDateTime());
    client->setLast_login(QDateTime::currentDateTime());

    QSharedPointer<ClientProfile> profile(new ClientProfile);
    profile->setIdClient(3);
    profile->setRecord(GeneralDataField::Age, 28);
    profile->setRecord(FunctionalField::HipMobility, 9);
    profile->setRecord(MorfologicalFields::FatPercentage, 12.3);
    profile->setRecord(ROMJoint::NeckFlexion, 55);
    profile->setRecord(LifeStyleFields::TrainingFrequency, "semanal");
    profile->setRecord(MedicalRecordsFields::PreviousInjuries, "esguince");

    client->setProfile(profile);
    client->setWorkout(QDateTime::currentDateTime(), 123);

    QVERIFY(repo->saveUser(client));

    QSharedPointer<ClientProfile> profileRes = repo->getClientProfile(3);
    QVERIFY(profileRes != nullptr);
    QCOMPARE(profileRes->getRecord(GeneralDataField::Age).toInt(), 28);
    QCOMPARE(profileRes->getRecord(MorfologicalFields::FatPercentage).toDouble(), 12.3);

    qDebug() << "[Tabla general_records tras insertar]";
    db->get(TypeBDEnum::ProfileGeneral).print();
}
