#include "testadmin.h"
#include "profiles/admin.h"
#include "enums/enums.h"
#include <QtTest>

/**
 * @brief Inicializa el entorno de pruebas para TestAdmin.
 */
void TestAdmin::init_Test() {
    qDebug() << "== TestAdmin empieza ==";
}

/**
 * @brief Finaliza el entorno de pruebas para TestAdmin.
 */
void TestAdmin::finish_Test() {
    qDebug() << "== TestAdmin termina ==";
}

/**
 * @brief Prueba el constructor Admin con todos los campos necesarios.
 *
 * Se asegura que los valores heredados y propios se almacenen correctamente.
 */
void TestAdmin::testConstructorValido() {

    QHash<UserField, QVariant> datos;
    datos[UserField::IdUser] = 10;
    datos[UserField::UserName] = "admin_user";
    datos[UserField::Email] = "admin@mail.com";
    datos[UserField::Pass] = "adminpass";
    datos[UserField::permissionLevel] = 3;
    datos[UserField::roleDescription] = "Coordinador";
    datos[UserField::canManageUsers] = true;
    datos[UserField::canModifyPlans] = false;

    Admin admin(datos);

    int id = admin.getId();
    QString name = admin.getUserName();
    QString email = admin.getEmail();
    int level = admin.getPermissionLevel();
    QString role = admin.getRoleDescription();
    bool manage = admin.canManage();
    bool modify = admin.canModifyTrainingPlans();

    QCOMPARE(id, 10);
    QCOMPARE(name, QString("admin_user"));
    QCOMPARE(email, QString("admin@mail.com"));
    QCOMPARE(level, 3);
    QCOMPARE(role, QString("Coordinador"));
    QCOMPARE(manage, true);
    QCOMPARE(modify, false);
}


/**
 * @brief Prueba la construcción inválida del objeto Client con distintos errores en los datos.
 *
 * Se verifican casos como ausencia de campos obligatorios, ID negativo y ID no convertible.
 */
void TestAdmin::test_ConstructorInvalido() {


    QHash<UserField, QVariant> datos;
    datos[UserField::UserName] = "no_id";
    datos[UserField::Email] = "email";
    datos[UserField::Pass] = "123";

     Admin admin1(datos);


    datos[UserField::IdUser] = -20;
    datos[UserField::UserName] = "Id negativo";
    datos[UserField::Email] = "email";
    datos[UserField::Pass] = "123";

     Admin admin2(datos);


    QHash<UserField, QVariant> datos2;
    datos2[UserField::IdUser] = 1;
    datos2[UserField::Pass] = "123";

     Admin admin3(datos2);


    QHash<UserField, QVariant> datos3;
    datos[UserField::IdUser] = "id";
    datos3[UserField::UserName] = "ID String";
    datos3[UserField::Email] = "email";
    datos3[UserField::Pass] = "123";

     Admin admin4(datos3);



    int id = admin1.getId();
    int id2 = admin2.getId();
    int id3 = admin3.getId();
    int id4=admin4.getId();


    QVERIFY(id ==-1);
    QVERIFY(id2 ==-1 );
    QVERIFY(id3 ==-1 );
    QVERIFY(id4 ==-1 );
}
