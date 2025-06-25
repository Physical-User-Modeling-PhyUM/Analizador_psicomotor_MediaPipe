#include "testuserpreferences.h"
#include "profiles/userpreferences.h"
#include <QtTest>

/**
 * @brief Inicializa el entorno de pruebas para TestUserPreferences.
 */
void TestUserPreferences::init_Test() {
    qDebug() << "== TestUserPreferences empieza ==";
}

/**
 * @brief Finaliza el entorno de pruebas para TestUserPreferences.
 */
void TestUserPreferences::finish_Test() {
    qDebug() << "== TestUserPreferences termina ==";
}

/**
 * @brief Prueba que al no proporcionar datos, se asignen los valores por defecto esperados.
 */
void TestUserPreferences::testValoresPorDefecto() {
    QMap<QString, QString> vacio;
    UserPreferences prefs(vacio);

    int idUser = prefs.getIdUser();
    QCOMPARE(idUser, 0);
}

/**
 * @brief Prueba la correcta interpretación de datos completos al construir UserPreferences.
 */
void TestUserPreferences::testInicializacionCompleta() {
    QMap<QString, QString> datos;
    datos["idPref"] = "100";
    datos["idUser"] = "200";
    datos["language"] = "Español";
    datos["notifications"] = "true";
    datos["units"] = "Imperial";

    UserPreferences prefs(datos);

    int userId = prefs.getIdUser();
    QCOMPARE(userId, 200);
}
