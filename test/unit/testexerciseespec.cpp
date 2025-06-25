#include "testexerciseespec.h"
#include "workouts/exerciseespec.h"
#include "enums/ExEspecFieldEnum.h"
#include "pose/state.h"
#include <QtTest>
#include <QDebug>

/**
 * @file testexerciseespec.cpp
 * @brief Implementación de pruebas unitarias para la clase ExerciseEspec.
 *
 * Este archivo contiene pruebas unitarias estructuradas según criterios de caja negra y caja blanca,
 * con el objetivo de validar la correcta funcionalidad del modelo `ExerciseEspec`.
 *
 * Se aplican:
 *  - Pruebas de clase de equivalencia (válidas e inválidas).
 *  - Análisis de valores límite.
 *  - Pruebas estructurales con cobertura lógica y de bucles.
 *
 * Esta clase de test puede declararse como `friend` en `ExerciseEspec` para permitir acceso interno si fuera necesario.
 */

/**
 * @brief Inicializa los recursos para los test.
 */
void TestExerciseEspec::initTestCase() {

    qDebug() << "==== INICIO TestExerciseEspec ====";
}

/**
 * @brief Limpieza de recursos tras la ejecución de pruebas.
 */
void TestExerciseEspec::cleanupTestCase() {
    qDebug() << "==== FIN TestExerciseEspec ====";
}

/**
 * @test Verifica que los valores válidos inicializan correctamente un ExerciseEspec.
 *
 * Aplica clase de equivalencia válida.
 */
void TestExerciseEspec::testValoresValidos_equivalencia() {

    QHash<ExEspecField, QVariant> data;
    data[ExEspecField::IdEx] = 10;
    data[ExEspecField::Name] = "Press de banca";
    data[ExEspecField::Series] = 3;
    data[ExEspecField::Repetitions] = 10;
    data[ExEspecField::Weight] = 70;

    ExerciseEspec espec(data);
    QCOMPARE(espec.getIdEx(), 10);
    QCOMPARE(espec.getName(), QString("Press de banca"));
    QCOMPARE(espec.getSeries(), 3);
    QCOMPARE(espec.getRepetitions(), 10);
    QCOMPARE(espec.getWeightPercentage(), 70.0f);
}

/**
 * @test Evalúa el comportamiento del constructor ante entradas inválidas (clase no válida).
 *
 * Se espera que los valores se almacenen tal cual, ya que la clase no realiza validación interna.
 */
void TestExerciseEspec::testValoresInvalidos_equivalencia() {
    QHash<ExEspecField, QVariant> data;
    data[ExEspecField::Series] = -5;
    data[ExEspecField::Repetitions] = -1;
    data[ExEspecField::Weight] = -30;

    ExerciseEspec espec(data);
    QVERIFY(espec.getSeries() ==0);
    QVERIFY(espec.getRepetitions() == 0);
    QVERIFY(espec.getWeightPercentage() == 0.0f);
}

/**
 * @test Prueba con valores límite en campos numéricos.
 *
 * Se validan límites inferiores como 0 y 1.
 */
void TestExerciseEspec::testValoresLimite() {
    QHash<ExEspecField, QVariant> data;

    data[ExEspecField::Series] = 0;
    data[ExEspecField::Repetitions] = 1;
    data[ExEspecField::Weight] = 0;

    ExerciseEspec espec(data);
    QCOMPARE(espec.getSeries(), 0);
    QCOMPARE(espec.getRepetitions(), 1);
    QCOMPARE(espec.getWeightPercentage(), 0.0f);
}
/**
 * @test Caja blanca: prueba de inserción de estados (bucle) y transiciones (cobertura lógica).
 *
 * - Se insertan múltiples estados mediante `addState` (prueba de bucles).
 * - Se inserta una transición válida con un `Condition`, usando `addTransition`.
 * - Se valida la recuperación del `QSet<Condition>` asociado (cobertura lógica del hash).
 */
void TestExerciseEspec::testTransiciones_yEstados_coberturaYBucles() {
    // Crear ExerciseEspec vacío
    QHash<ExEspecField, QVariant> vacio;
    ExerciseEspec espec(vacio);

    // Prueba de bucle: insertar múltiples estados con IDs diferentes
    for (int i = 0; i < 5; ++i) {
        State s(i);  // Constructor requiere ID
        espec.addState(s);
    }
    QCOMPARE(espec.getStatesList().size(), 5);

    // Transición entre estados con una condición válida
    QPair<int, int> trans(0, 1);
    QSet<Condition> condiciones;

    // Crear una condición real
    Condition c(ConditionType::OptimalForm, "shoulder_left-shoulder_right");
    condiciones.insert(c);

    espec.addTransition(trans, condiciones);

    // Verificación de inserción y acceso
    QVERIFY(espec.getTransitionTable().contains(trans));
    QVERIFY(espec.getTransitionTable().value(trans).contains(c));
}
