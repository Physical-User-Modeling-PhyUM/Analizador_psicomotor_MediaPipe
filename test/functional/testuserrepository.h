/**
 * @file testuserrepository.h
 * @brief Declaración de pruebas funcionales y estructurales para UserRepository.
 */
#ifndef TESTUSERREPOSITORY_H
#define TESTUSERREPOSITORY_H

#include <QObject>
#include "repositories/userrepository.h"
#include "fakedbmanager.h"

/**
 * @class TestUserRepository
 * @brief Clase de prueba para UserRepository. Incluye:
 * - Pruebas de caja negra (validación funcional externa).
 * - Pruebas de caja blanca (cubrimiento de lógica interna).
 * - Pruebas de condiciones especiales y bordes.
 */
class TestUserRepository : public QObject {
    Q_OBJECT

private:
    FakeDBManager* fakeDB;
    UserRepository* repo;

private slots:
    /** @brief Inicializa los objetos necesarios para las pruebas. */
    void initTestCase();

    /** @brief Libera recursos al finalizar las pruebas. */
    void cleanupTestCase();

    // --- Caja negra ---

    /** @test Verifica obtención correcta de usuario existente. */
    void test_GetUserById_valido();

    /** @test Verifica comportamiento ante ID negativo. */
    void test_GetUserById_idInvalido();

    /** @test Verifica comportamiento ante usuario inexistente. */
    void test_GetUserById_NoExiste();

    /** @test Verifica guardado correcto de usuario Client. */
    void test_SaveUser_val();

    /** @test Verifica que no se guarda usuario con tipo desconocido. */
    void test_SaveUser_desconocido();

    /** @test Verifica eliminación de usuario existente. */
    void test_DeleteUser_existente();

    /** @test Verifica eliminación segura de usuario no existente. */
    void test_DeleteUser_inexistente();

    /** @test Verifica carga completa del perfil de un cliente. */
    void test_GetClientProfile();

    // --- Caja blanca ---

    /** @test Ruta interna para usuario tipo Trainer. */
    void test_GetUserById_Trainer();

    /** @test Ruta interna para usuario tipo Admin. */
    void test_GetUserById_Admin();



    /** @test Verifica guardado ruta Trainer. */
    void testSave_Trainer();

    /** @test Verifica guardado ruta Admin. */
    void testSave_Admin();

    /** @test Verifica eliminación iterada sobre todas las tablas. */
    void test_DeleteUser_bucleTablas();
};

#endif // TESTUSERREPOSITORY_H
