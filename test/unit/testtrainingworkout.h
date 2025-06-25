#ifndef TESTTRAININGWORKOUT_H
#define TESTTRAININGWORKOUT_H

#include <QObject>

/**
 * @file testtrainingworkout.h
 * @brief Pruebas unitarias para la clase TrainingWorkout.
 *
 * Contiene pruebas de caja negra (validaciones básicas del constructor y getters/setters)
 * y algunas pruebas de caja blanca limitadas (bucle de búsqueda de ejercicios).
 *
 * Algunas pruebas como validación de setters no son necesarias porque la lógica es trivial,
 * aunque se testean los efectos por si acaso algo se rompiera por refactor a futuro.
 */
class TestTrainingWorkout : public QObject {
    Q_OBJECT

private slots:

    void init_Test();
    void finishTest();

    // Pruebas funcionales / caja negra
    void testConstructorValido();

    void testAddExercise_porDefecto();
    void testAddExercise_conOrden();
    void testGetExercise_existente();
    void testGetExercise_noExistente();
    void testToSummary_Correcto();

    // Caja blanca mínima
    void testGetExercise_bucleEjecutado();
    void testGetExercise_bucleVacio();
};

#endif // TESTTRAININGWORKOUT_H
