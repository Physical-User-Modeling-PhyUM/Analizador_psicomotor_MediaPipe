#ifndef TESTINTEGRACIONUSERMANAGERDB_H
#define TESTINTEGRACIONUSERMANAGERDB_H

#include <QObject>
#include <QtTest>
#include "core/usermanager.h"
#include "repositories/userrepository.h"
#include "core/dbmanager.h"
#include "profiles/clientprofile.h"

/**
 * @class TestIntegracionUserManagerDB
 * @brief Clase de pruebas de integración para la clase UserManager.
 *
 * Realiza pruebas de caja negra y caja blanca (si es necesario) sobre
 * los métodos del UserManager en conexión con la base de datos real.
 */
class TestIntegracionUserManagerDB : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor por defecto.
     */
    TestIntegracionUserManagerDB();

private slots:
    /// @brief Inicializa la base de datos antes de cada prueba.
    void initTestCase();

    /// @brief Prueba la creación de un nuevo cliente.
    void testCreateClient();

    /// @brief Prueba la asignación y recuperación de un plan de entrenamiento.
    void testAssignAndGetWorkout();

    /// @brief Prueba la actualización y recuperación del perfil del cliente.
    void testUpdateAndGetClientProfile();

    /// @brief Limpieza final después de todas las pruebas.
    void cleanupTestCase();

private:
    DBManager* db;
    UserRepository* repo;
    UserManager* userManager;
    int testClientId;
};

#endif // TESTINTEGRACIONUSERMANAGERDB_H
