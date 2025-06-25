#include "testclient.h"
#include "profiles/client.h"
#include "profiles/clientprofile.h"
#include "enums/enums.h"
#include <QtTest>
#include <QDateTime>

/**
 * @brief Inicializa el entorno de pruebas para TestClient.
 */
void TestClient::init_Test() {
    qDebug() << "== TestClient empieza ==";
}

/**
 * @brief Finaliza el entorno de pruebas para TestClient.
 */
void TestClient::finish_Test() {
    qDebug() << "== TestClient termina ==";
}

/**
 * @brief Prueba la inicialización correcta del objeto Client desde un QHash.
 */
void TestClient::test_ConstructorValido() {

    QHash<UserField, QVariant> datos;

    datos[UserField::IdUser] = 20;
    datos[UserField::UserName] = "cliente_test";
    datos[UserField::Email] = "cliente@mail.com";
    datos[UserField::Pass] = "1234";

    Client client(datos);

    int id = client.getId();
    QString name = client.getUserName();

    QCOMPARE(id, 20);
    QCOMPARE(name, QString("cliente_test"));
}
/**
 * @brief Prueba la construcción inválida del objeto Client con distintos errores en los datos.
 *
 * Se verifican casos como ausencia de campos obligatorios, ID negativo y ID no convertible.
 */
void TestClient::test_ConstructorInvalido() {


    QHash<UserField, QVariant> datos;
    datos[UserField::UserName] = "no_id";
    datos[UserField::Email] = "email";
    datos[UserField::Pass] = "123";

     Client client1(datos);

    datos[UserField::IdUser] = -20;
    datos[UserField::UserName] = "Id negativo";
    datos[UserField::Email] = "email";
    datos[UserField::Pass] = "123";

    Client client2(datos);

    QHash<UserField, QVariant> datos2;
    datos2[UserField::IdUser] = 1;
    datos2[UserField::Pass] = "123";

    Client client3(datos2);

    QHash<UserField, QVariant> datos3;
    datos[UserField::IdUser] = "id";
    datos3[UserField::UserName] = "ID String";
    datos3[UserField::Email] = "email";
    datos3[UserField::Pass] = "123";

      Client client4(datos3);


    int id = client1.getId();
     int id2 = client2.getId();
      int id3 = client3.getId();
     int id4=client4.getId();


    QVERIFY(id ==-1);
    QVERIFY(id2 ==-1 );
     QVERIFY(id3 ==-1 );
     QVERIFY(id4 ==-1 );
}

/**
 * @brief Prueba la asociación y recuperación de un perfil de cliente.
 */
void TestClient::test_AsociacionPerfil() {

    QHash<UserField, QVariant> datos;

    datos[UserField::IdUser] = 20;
    datos[UserField::UserName] = "cliente_test";
    datos[UserField::Email] = "cliente@mail.com";
    datos[UserField::Pass] = "1234";
    Client client(datos);

    QSharedPointer<ClientProfile> perfil = QSharedPointer<ClientProfile>::create();

    perfil->setIdClient(20);
    client.setProfile(perfil);

    int perfilId = client.getProfile()->getIdClient();

     QCOMPARE(perfilId, 20);
}

/**
 * @brief Prueba el almacenamiento, consulta y eliminación de entrenamientos por fecha.
 */
void TestClient::test_CalendarioEntrenamientos() {

    QHash<UserField, QVariant> datos;

    datos[UserField::IdUser] = 20;
    datos[UserField::UserName] = "cliente_test";
    datos[UserField::Email] = "cliente@mail.com";
    datos[UserField::Pass] = "1234";

    Client client(datos);

    QDateTime fecha1 = QDateTime::currentDateTime();
    QDateTime fecha2 = fecha1.addDays(1);

    client.setWorkout(fecha1, 101);
    client.setWorkout(fecha2, 202);

    bool existe1 = client.isDateAvailable(fecha1);
    bool existe2 = client.isDateAvailable(fecha2);
    int id1 = client.getWorkoutId(fecha1);

    client.delWorkout(fecha1);
    bool borrado = client.isDateAvailable(fecha1);

    QCOMPARE(existe1, true);
    QCOMPARE(existe2, true);
    QCOMPARE(id1, 101);
    QCOMPARE(borrado, false);
}
