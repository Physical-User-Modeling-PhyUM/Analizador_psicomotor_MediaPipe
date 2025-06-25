/**
 * @file testtrainingrepository.h
 * @brief Declaración de pruebas funcionales para TrainingRepository.
 */
#ifndef TESTTRAININGREPOSITORY_H
#define TESTTRAININGREPOSITORY_H

#include <QObject>
#include <Qtest.h>
#include "repositories/trainingrepository.h"
#include "fakedbmanager.h"

/**
 * @class TestTrainingRepository
 * @brief Clase de prueba para TrainingRepository. Incluye:
 * - Pruebas de caja negra (valores límite, clases de equivalencia).
 * - Pruebas de caja blanca (cubrimiento lógico, bucles).
 * - Validaciones adicionales según criterio profesional.
 */
class TestTrainingRepository : public QObject {
    Q_OBJECT

private:
    FakeDBManager* fakeDB;
    TrainingRepository* repo;

private slots:
    /** @brief Inicializa el entorno de pruebas */
    void initTestCase();

    /** @brief Limpia recursos tras las pruebas */
    void cleanupTestCase();

    // --- Caja negra ---

    /** @test Verifica guardado y recuperación correcta de un Workout con un ejercicio */
    void test_SaveAndGetWorkout_Valido();

    /** @test Verifica que no se guarda un workout nulo */
    void test_SaveWorkout_Nulo();

    /** @test Verifica eliminación completa de un workout existente */
    void test_DeleteWorkoutFull_Valido();

    /** @test Verifica guardado de sesión de entrenamiento válida */
    void test_SaveTrainingSesion_Valida();

    // --- Caja blanca ---

    /** @test Verifica cobertura de bucle en guardado de estados de ExerciseEspec */
    void test_SaveEspec_MultiplesEstados();

    /** @test Verifica que se cubren transiciones múltiples entre estados */
    void test_SaveEspec_TransicionesMultiples();

    /** @test Verifica que no se guarda una especificación sin datos */
    void test_SaveEspec_Invalida();

    // --- Validaciones ---

    /** @test Verifica que getWorkout devuelve nullptr si no existe el ID */
    void test_GetWorkout_NoExiste();

    /** @test Verifica que deleteEspec borra completamente todos los elementos asociados */
    void test_DeleteEspec_ValidacionCompleta();
};

#endif // TESTTRAININGREPOSITORY_H
