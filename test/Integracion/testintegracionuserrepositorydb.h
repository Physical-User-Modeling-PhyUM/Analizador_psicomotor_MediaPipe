/**
 * @file test_integracion_userrepository_db.h
 * @brief Declaración de la clase TestIntegracionUserRepositoryDB para pruebas de integración con base de datos real.
 */

#ifndef TEST_INTEGRACION_USERREPOSITORY_DB_H
#define TEST_INTEGRACION_USERREPOSITORY_DB_H

#include <QObject>
#include "repositories/userrepository.h"
#include "core/dbmanager.h"

/**
 * @class TestIntegracionUserRepositoryDB
 * @brief Clase de pruebas de integración para verificar el funcionamiento de UserRepository con una base de datos SQLite real.
 *
 * Esta clase realiza pruebas de guardado, recuperación y eliminación de usuarios,
 * así como comprobación de los perfiles de cliente. Usa la base de datos real en SQLite
 * y muestra en consola el estado de las tablas tras cada operación.
 */
class TestIntegracionUserRepositoryDB : public QObject {
    Q_OBJECT

public:
    explicit TestIntegracionUserRepositoryDB(QObject* parent = nullptr) {}

private:
    DBManager* db;                ///< Gestor de base de datos real.
    UserRepository* repo;        ///< Repositorio de usuarios a probar.
    QString dbPath = "test_integration.sqlite"; ///< Ruta del archivo de base de datos para pruebas.

private slots:
    /**
     * @brief Inicializa la base de datos, eliminando cualquier archivo previo y creando el esquema.
     */
    void initTestCase();

    /**
     * @brief Libera recursos y elimina el archivo de base de datos.
     */
    void cleanupTestCase();

    /**
     * @test Verifica que se puede guardar usuarios tipo Client correctamente.
     */
    void test_SaveClient();

    /**
     * @test Verifica que se puede guardar y recuperar un usuario tipo Client correctamente.
     */
    void test_SaveUserAndGetUserById();

    /**
     * @test Verifica que se puede eliminar correctamente un usuario de la base de datos.
     */
    void test_DeleteUser();

    /**
     * @test Verifica el guardado de un usuario tipo Admin y muestra el estado de la base de datos.
     */
    void test_SaveAdmin();

    /**
     * @test Verifica que se puede recuperar el perfil completo de un cliente.
     */
    void test_GetClientProfile();
};

#endif // TEST_INTEGRACION_USERREPOSITORY_DB_H
