/**
 * @file testintegraciontrainingrepositorydb.h
 * @brief Declaración de la clase TestIntegracionTrainingRepositoryDB para pruebas de integración con base de datos real y TrainingRepository.
 */

#ifndef TESTINTEGRACIONTRAININGREPOSITORYDB_H
#define TESTINTEGRACIONTRAININGREPOSITORYDB_H

#include <QObject>
#include "repositories/trainingrepository.h"
#include "core/dbmanager.h"

/**
 * @class TestIntegracionTrainingRepositoryDB
 * @brief Clase de pruebas de integración para TrainingRepository con base de datos real (SQLite).
 *
 * Esta clase prueba la funcionalidad de guardado, recuperación y eliminación
 * de workouts, especificaciones de ejercicios y sesiones de entrenamiento,
 * utilizando una base de datos real en lugar de una simulada.
 */
class TestIntegracionTrainingRepositoryDB : public QObject {
    Q_OBJECT

public:
    explicit TestIntegracionTrainingRepositoryDB(QObject* parent = nullptr) {}

private:
    DBManager* db;
    TrainingRepository* repo;
    QString dbPath = "test_training.sqlite";

private slots:
    /** @brief Inicializa la base de datos y el repositorio */
    void initTestCase();

    /** @brief Libera recursos y elimina la base de datos */
    void cleanupTestCase();

    /** @test Verifica que se guarda y recupera un especificacion de ejercicio correctamente */
    void Test_SaveAndGetExerciseEspec();

    /** @test Verifica que se guarda y recupera un workout correctamente */
    void test_SaveAndGetWorkout_Valido();

    /** @test Verifica eliminación completa de un workout */
    void test_DeleteWorkoutFull();

    /** @test Verifica que se guarda una sesión de entrenamiento válida */
    //void test_SaveTrainingSesion();
    void test_SaveAndGet_TrainingSesion();

};

#endif // TESTINTEGRACIONTRAININGREPOSITORYDB_H
