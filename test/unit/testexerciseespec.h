#ifndef TESTEXERCISEESPEC_H
#define TESTEXERCISEESPEC_H

#include <QObject>

/**
 * @file testexerciseespec.h
 * @brief Declaración de la clase de pruebas unitarias para ExerciseEspec.
 *
 * Esta clase contiene pruebas estructuradas de tipo caja negra (clases de equivalencia y análisis de valores límite)
 * y caja blanca (cobertura lógica y pruebas de bucles), orientadas a verificar el correcto comportamiento de la clase
 * ExerciseEspec.
 * Está diseñada para ejecutarse con el framework QTest de Qt.
 *
 */

class TestExerciseEspec : public QObject {
    Q_OBJECT

private slots:

    /**
     * @brief Inicialización antes de las pruebas (se ejecuta una sola vez).
     */
    void initTestCase();

    /**
     * @brief Limpieza posterior a las pruebas (se ejecuta una sola vez).
     */
    void cleanupTestCase();

    /**
     * @test Caja negra: prueba con valores válidos (partición de clase de equivalencia).
     */
    void testValoresValidos_equivalencia();

    /**
     * @test Caja negra: prueba con valores inválidos (clase inválida).
     */
    void testValoresInvalidos_equivalencia();

    /**
     * @test Caja negra: prueba de análisis de valores límite (0, 1, etc.).
     */
    void testValoresLimite();

    /**
     * @test Caja blanca: prueba de inserción de estados (bucle) y transición (cobertura lógica).
     */
    void testTransiciones_yEstados_coberturaYBucles();
};

#endif // TESTEXERCISEESPEC_H
