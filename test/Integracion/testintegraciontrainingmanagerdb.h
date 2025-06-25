/**
 * @file testintegraciontrainingmanagerdb.h
 * @brief Declaración de la clase de prueba de integración TestIntegracionTrainingManagerDB.
 *
 * Esta clase realiza pruebas de integración sobre la clase TrainingManager utilizando DBManager real.
 * Incluye pruebas funcionales para la creación, recuperación y eliminación de workouts, ejercicios y sesiones.
 */

#ifndef TESTINTEGRACIONTRAININGMANAGERDB_H
#define TESTINTEGRACIONTRAININGMANAGERDB_H

#include <QObject>
#include <QtTest>
#include "core/dbmanager.h"
#include "core/trainingmanager.h"
#include "repositories/trainingrepository.h"
#include "enums/WorkoutFieldEnum.h"
#include "enums/ExEspecFieldEnum.h"

/**
 * @class TestIntegracionTrainingManagerDB
 * @brief Clase que realiza pruebas de integración de TrainingManager con una base de datos real.
 *
 * Esta clase comprueba el correcto funcionamiento del flujo de datos entre TrainingManager y la base de datos real
 * gestionada por DBManager. Las pruebas incluyen operaciones de guardado, recuperación y cierre de sesión de entrenamiento.
 */
class TestIntegracionTrainingManagerDB : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor por defecto.
     */
    TestIntegracionTrainingManagerDB();

    /**
     * @brief Destructor.
     */
    ~TestIntegracionTrainingManagerDB();

private slots:
    /**
     * @brief Configura la base de datos antes de correr las pruebas.
     */
    void initTestCase();

    /**
     * @brief Limpia los recursos tras ejecutar todas las pruebas.
     */
    void cleanupTestCase();

    /**
     * @brief Verifica que se pueda guardar y recuperar correctamente un workout.
     */
    void testGuardarYRecuperarWorkout();

    /**
     * @brief Verifica que se pueda guardar y recuperar correctamente un ejercicio.
     */
    void testGuardarYRecuperarExercise();

    /**
     * @brief Verifica que se pueda iniciar y finalizar una sesión correctamente.
     */
    void testStartYEndSesion();

private:
    DBManager* db;  ///< Manejador real de base de datos.
    QSharedPointer<TrainingManager> manager;  ///< Manejador de lógica de entrenamiento.
    QString testDbPath = ":memory:";  ///< Ruta de base de datos en memoria para pruebas.
};

#endif // TESTINTEGRACIONTRAININGMANAGERDB_H
