#include "testuser.h"
#include "profiles/user.h"
#include <QtTest>
#include <QDateTime>

/**
 * @brief Inicializa el entorno de pruebas para TestUser.
 */
void TestUser::init_Test() {
    qDebug() << "== TestUser empieza ==";
}

/**
 * @brief Finaliza el entorno de pruebas para TestUser.
 */
void TestUser::finish_Test() {
    qDebug() << "== TestUser termina ==";
}

/**
 * @brief Prueba que el constructor de User inicializa correctamente todos los campos.
 *
 * Crea una instancia de User usando el constructor con parámetros y verifica
 * que los atributos internos coinciden con los valores pasados.
 */
void TestUser::testConstructorValido() {
    User user(1, "john", "john@mail.com", "secret");

    int id = user.getId();
    QString name = user.getUserName();
    QString email = user.getEmail();
    QString pass = user.getPassword();

    QCOMPARE(id, 1);
    QCOMPARE(name, QString("john"));
    QCOMPARE(email, QString("john@mail.com"));
    QCOMPARE(pass, QString("secret"));
}

/**
 * @brief Prueba los métodos de modificación y consulta de nombre y email.
 *
 * Se asignan nuevos valores usando los métodos setUserName() y setEmail(),
 * y luego se validan con sus respectivos getters.
 */
void TestUser::testCamposEspecificos() {

    User user(2, "jane", "jane@mail.com", "clave");

    user.setUserName("jane_doe");
    user.setEmail("new@mail.com");

    QString name = user.getUserName();
    QString email = user.getEmail();

    QCOMPARE(name, QString("jane_doe"));
    QCOMPARE(email, QString("new@mail.com"));
}
