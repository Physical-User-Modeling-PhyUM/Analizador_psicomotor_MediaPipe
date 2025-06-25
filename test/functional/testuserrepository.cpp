/**
 * @file testuserrepository.cpp
 * @brief Implementación de pruebas funcionales para UserRepository con comentarios Doxygen.
 */

#include "testuserrepository.h"
#include "profiles/user.h"
#include "profiles/client.h"
#include <QDebug>
#include <QTest>

/**
 * @brief Inicializa el entorno de pruebas creando instancias de FakeDBManager y UserRepository.
 */
void TestUserRepository::initTestCase() {
    fakeDB = new FakeDBManager();
    repo = new UserRepository(fakeDB);

    //generamos las tablas que vamos a usar

    DBTable userTable("users");
    userTable.setColumns({"idUser", "userName", "email", "password", "userType", "join_up_date", "last_login"});
    userTable.addRow({1, "juan", "j@l.com", "1234", "Client", "2024-03-14", "2024-03-15"});
    userTable.addRow({2, "Fran", "t@l.com", "abc", "Trainer", "2024-03-14", "2024-03-15"});
    userTable.addRow({3, "Cisco", "a@l.com", "admin", "Admin", "2024-03-14", "2024-03-15"});
    userTable.addRow({20, "extra", "e@l.com", "pass", "Client", "2024-03-14", "2024-03-15"});

    fakeDB->setTestTable(TypeBDEnum::User, userTable);

    DBTable clientTable("clients");
    clientTable.setColumns({"idUser","idClient","expLevel","plan"});
    clientTable.addRow({1,1,2,"3 días"});
    clientTable.addRow({20,2,2,"2 Dias"});
    fakeDB->setTestTable(TypeBDEnum::Client, clientTable);

    DBTable trainerTable("trainers");
    trainerTable.setColumns({"idUser","idTrainer","qualifications","specialtyArea","experience_years","resumme"});
    trainerTable.addRow({2,1,"High","crossfit",2,"resume"});
    fakeDB->setTestTable(TypeBDEnum::Trainer, trainerTable);

    DBTable adminTable("admins");
    adminTable.setColumns({"idUser","idAdmin","canManageUsers","canModifyPlans","roleDescription","permissionLevel" });
    adminTable.addRow({3,3,true,true,"admin",3});
    fakeDB->setTestTable(TypeBDEnum::Admin, adminTable);

    DBTable general("general_records");
    general.setColumns({"idClient", "age"});
    general.addRow({1, 45});
     general.addRow({20, 45});
    fakeDB->setTestTable(TypeBDEnum::ProfileGeneral, general);

    DBTable functional("functional_records");
    functional.setColumns({"idClient", "hipmobility"});
    functional.addRow({1, 8});
    functional.addRow({20, 8});
    fakeDB->setTestTable(TypeBDEnum::ProfileFunctional, functional);

    DBTable life("lifestyle_records");
    life.setColumns({"idClient", "Trainingfrequency"});
    life.addRow({1, "diaria"});
       life.addRow({20, "diaria"});
    fakeDB->setTestTable(TypeBDEnum::ProfileLifestyle, life);

    DBTable med("medical_records");
    med.setColumns({"idClient", "previousinjuries"});
    med.addRow({1, "none"});
    med.addRow({20, "none"});
    fakeDB->setTestTable(TypeBDEnum::ProfileMedical, med);

    DBTable mor("morfological_records");
    mor.setColumns({"idClient", "fatpercentage"});
    mor.addRow({1, 10.0});
      mor.addRow({20, 10.0});
    fakeDB->setTestTable(TypeBDEnum::ProfileMorphology, mor);

    DBTable rom("rom_records");
    rom.setColumns({"idClient", "neckflexion"});
    rom.addRow({1, 60});
      rom.addRow({20, 60});
    fakeDB->setTestTable(TypeBDEnum::ProfileROM, rom);

    DBTable calendar("client_workout_calendar");
    calendar.setColumns({"idClient", "date", "idWorkout"});
    calendar.addRow({1, "2024-03-15T10:00:00", 101});
      calendar.addRow({20, "2024-03-15T10:00:00", 102});
    fakeDB->setTestTable(TypeBDEnum::ClientWorkoutCalendar, calendar);

    // DBTable e("users");
    // e.setColumns({"idUser"});
    // e.addRow({20});
    // fakeDB->setTestTable(TypeBDEnum::User, e);

    fakeDB->get(TypeBDEnum::User).print();
     fakeDB->get(TypeBDEnum::Client).print();
    fakeDB->get(TypeBDEnum::ProfileGeneral).print();
    fakeDB->get(TypeBDEnum::ProfileFunctional).print();
  fakeDB->get(TypeBDEnum::ProfileLifestyle).print();
   fakeDB->get(TypeBDEnum::ProfileMedical).print();
     fakeDB->get(TypeBDEnum::ProfileMorphology).print();
   fakeDB->get(TypeBDEnum::ProfileROM).print();
     fakeDB->get(TypeBDEnum::ClientWorkoutCalendar).print();
      fakeDB->get(TypeBDEnum::Admin).print();
     fakeDB->get(TypeBDEnum::Trainer).print();



}

/**
 * @brief Libera los recursos usados tras la ejecución de las pruebas.
 */
void TestUserRepository::cleanupTestCase() {
    delete repo;
    delete fakeDB;
}

/**
 * @test Verifica que se puede obtener un usuario válido (tipo Client) correctamente.
 */
void TestUserRepository::test_GetUserById_valido() {


    QSharedPointer<User> user = repo->getUserById(1);
    qDebug()<<"Tabla usuario inicial";
    fakeDB->get(TypeBDEnum::User).print();

    QVERIFY(user != nullptr);
    QCOMPARE(user->getUserName(), QString("juan"));


}

/**
 * @test Verifica que se devuelve nullptr si se pasa un ID negativo.
 */
void TestUserRepository::test_GetUserById_idInvalido() {
    qDebug()<<"Tabla usuario inicial";
    fakeDB->get(TypeBDEnum::User).print();
    QSharedPointer<User> user = repo->getUserById(-5);
    QVERIFY(user == nullptr);
}

/**
 * @test Verifica que se devuelve nullptr si el usuario no existe en la base de datos.
 */
void TestUserRepository::test_GetUserById_NoExiste() {
    qDebug()<<"Tabla usuario inicial";
    fakeDB->get(TypeBDEnum::User).print();

    QSharedPointer<User> user1 = repo->getUserById(999);
    DBTable savedU = fakeDB->get(TypeBDEnum::User);
    savedU.print();
    QVERIFY(user1 == nullptr);
}

/**
 * @test Verifica que se guarda correctamente un usuario válido de tipo Client.
 */
void TestUserRepository::test_SaveUser_val() {
    qDebug()<<"Tabla usuario inicial";
    DBTable savedU = fakeDB->get(TypeBDEnum::User);
    savedU.print();

    QHash<UserField, QVariant> data;
    data[UserField::IdUser] = 10;
    data[UserField::UserName] = "pepe";
    data[UserField::Email] = "p@l.com";
    data[UserField::Pass] = "xyz";

    QSharedPointer<Client> client(new Client(data));

    client->setUserType(UserType::Client);
    client->setJoin_up_date(QDateTime::currentDateTime());
    client->setLast_login(QDateTime::currentDateTime());

    QSharedPointer<ClientProfile> profile(new ClientProfile);
    client->setProfile(profile);
    client->setWorkout(QDateTime::currentDateTime(), 101);

    repo->saveUser(client);

    savedU = fakeDB->get(TypeBDEnum::User);
    DBTable savedT = fakeDB->get(TypeBDEnum::Client);
    savedU.print();
    savedT.print();
    QVERIFY(savedT.rowCount() == 3);

}


/**
 * @test Verifica que no se guarda un usuario con tipo desconocido.
 */
void TestUserRepository::test_SaveUser_desconocido() {
    QSharedPointer<User> desc(new User(5, "Lara", "l@l.com", "pass", nullptr));
    desc->setUserType(UserType::Unknown);

    repo->saveUser(desc);
    qDebug()<<"Tabla usuarios resultante";
    DBTable result = fakeDB->get(TypeBDEnum::User);
    result.print();
    QVERIFY(result.rowCount() == 5);
}

/**
 * @test Verifica que se elimina correctamente un usuario existente.
 */
void TestUserRepository::test_DeleteUser_existente() {
    DBTable check1 = fakeDB->get(TypeBDEnum::User);
    DBTable check2 = fakeDB->get(TypeBDEnum::Client);

    bool borrado=repo->deleteUser(20);
    DBTable check3 = fakeDB->get(TypeBDEnum::User);
    DBTable check4 = fakeDB->get(TypeBDEnum::Client);
    DBTable check5 =  fakeDB->get(TypeBDEnum::ProfileGeneral);
    DBTable check6 = fakeDB->get(TypeBDEnum::User,DBFilter::IdUser,"20");

    qDebug()<<"Antes de borrar al Usuario";
     check1.print();
     check2.print();
     qDebug()<<"Despues de borrar al Usuario";
     check3.print();
     check4.print();
    check5.print();

     QVERIFY(!borrado);
    QVERIFY(check6.rowCount() == 0);
}

/**
 * @test Verifica que intentar eliminar un usuario inexistente no lanza errores.
 */
void TestUserRepository::test_DeleteUser_inexistente() {

    repo->deleteUser(404);
    QVERIFY(true);
}

/**
 * @test Verifica que se recupera correctamente el perfil completo de un cliente.
 */
void TestUserRepository::test_GetClientProfile() {

    QSharedPointer<ClientProfile> profile = repo->getClientProfile(20);

    QVERIFY(profile != nullptr);
    QCOMPARE(profile->getIdClient(), 20);


    QVariant result=profile->getRecord(GeneralDataField::Age);
    qDebug()<<"Datos obtenido:"<<result;

    QCOMPARE(result.toInt(),45);
}



/**
 * @test Verifica la ruta interna de creación de usuario tipo Trainer.
 */
void TestUserRepository::test_GetUserById_Trainer() {

    QSharedPointer<User> user = repo->getUserById(2);
    QVERIFY(user != nullptr);
    QCOMPARE(user->getUserName(), QString("Fran"));
}

/**
 * @test Verifica la ruta interna de creación de usuario tipo Admin.
 */
void TestUserRepository::test_GetUserById_Admin() {


    QSharedPointer<User> user = repo->getUserById(3);
    QVERIFY(user != nullptr);
    QCOMPARE(user->getUserName(), QString("Cisco"));
}


/**
 * @test Verifica que se guarda correctamente un usuario tipo Trainer.
 */
void TestUserRepository::testSave_Trainer() {
    QHash<UserField, QVariant> data;
    data[UserField::IdUser] = 33;
    data[UserField::UserName] = "coach";
    data[UserField::Email] = "coach@email.com";
    data[UserField::Pass] = "abc";

    QSharedPointer<FitnessTrainer> trainer(new FitnessTrainer(data));
    trainer->setUserType(UserType::Trainer);
    trainer->setJoin_up_date(QDateTime::currentDateTime());
    trainer->setLast_login(QDateTime::currentDateTime());

    repo->saveUser(trainer);
    DBTable savedU = fakeDB->get(TypeBDEnum::User);
    DBTable savedT = fakeDB->get(TypeBDEnum::Trainer);
    savedU.print();
    savedT.print();
    QVERIFY(savedT.rowCount() == 2);
}


/**
 * @test Verifica que se guarda correctamente un usuario tipo Admin.
 */
void TestUserRepository::testSave_Admin() {
    QHash<UserField, QVariant> data;
    data[UserField::IdUser] = 77;
    data[UserField::UserName] = "admin2";
    data[UserField::Email] = "admin2@email.com";
    data[UserField::Pass] = "pass";

    QSharedPointer<Admin> admin(new Admin(data));
    admin->setUserType(UserType::Admin);
    admin->setJoin_up_date(QDateTime::currentDateTime());
    admin->setLast_login(QDateTime::currentDateTime());

    repo->saveUser(admin);
    DBTable savedU = fakeDB->get(TypeBDEnum::User);
    DBTable savedT = fakeDB->get(TypeBDEnum::Admin);
    savedU.print();
    savedT.print();
    QVERIFY(savedT.rowCount() == 2);
}


/**
 * @test Verifica que el bucle de borrado masivo de datos funciona sobre todas las tablas.
 */
void TestUserRepository::test_DeleteUser_bucleTablas() {
    QList<TypeBDEnum>tablas{
        TypeBDEnum::User, TypeBDEnum::Client, TypeBDEnum::Trainer, TypeBDEnum::Admin,
            TypeBDEnum::ProfileGeneral, TypeBDEnum::ProfileFunctional, TypeBDEnum::ProfileMorphology,
            TypeBDEnum::ProfileROM, TypeBDEnum::ProfileLifestyle, TypeBDEnum::ProfileMedical
    };

    for (auto tipo :tablas) {
        fakeDB->get(tipo).print();

    }

    repo->deleteUser(99);
    int c=1;
    for (auto tipo :tablas) {
        fakeDB->get(tipo).print();
        if (tipo== TypeBDEnum::User) c=6;
        //else if (tipo==TypeBDEnum::Client) c=2;
        //else if (tipo==TypeBDEnum::Trainer||tipo==TypeBDEnum::Admin)c=2;

        else c=2;

        QVERIFY(fakeDB->get(tipo).rowCount() == c);
    }


}
