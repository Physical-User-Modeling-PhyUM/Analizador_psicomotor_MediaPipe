#ifndef TESTWORKOUTSUMMARY_H
#define TESTWORKOUTSUMMARY_H

#include <QObject>

/**
 * @file testworkoutsummary.h
 * @brief Declaración de la clase de test unitario para WorkoutSummary.
 *
 * Esta clase ejecuta pruebas unitarias sobre la clase WorkoutSummary, validando
 * tanto la construcción de objetos como su comportamiento frente a inserción y
 * consulta de resúmenes de ejercicios.
 *
 * El diseño de pruebas sigue un enfoque de caja negra (valores válidos, inválidos,
 * límites) y caja blanca (cobertura de bucles y lógica interna del método getExercise()).
 *
 * Nota: algunos casos no se prueban explícitamente porque no hay lógica asociada (setters, etc.)
 */
class TestWorkoutSummary : public QObject {
    Q_OBJECT

private slots:

    /// Se ejecuta antes de todas las pruebas. Aquí podríamos preparar logs u otros recursos.
    void initTest();

    /// Se ejecuta después de todas las pruebas. Se usa para liberar recursos si fuera necesario.
    void finishTest();

    /**
     * @brief Prueba con parámetros válidos.
     * Verifica que el constructor almacena correctamente todos los valores.
     */
    void test_Constructor_ValoresValidos();

    /**
     * @brief Prueba valores inválidos pero aceptados por diseño (id negativo, título vacío).
     * El sistema no lanza errores, solo marca como "-1" el id y trimmea el texto.
     */
    void test_Constructor_ValoresInvalidos();

    /**
     * @brief Prueba valores en el borde de lo válido (id = 0, cadenas con espacios).
     * Se espera que los espacios sen eliminados.
     */
    void test_ValoresLimite();;
    /**
     * @brief Verifica que al añadir un ejercicio se actualiza correctamente el contador interno.
     */
    void test_AddExerciseSummary();


    /**
     * @brief Verifica que al añadir un ejercicio duplicado, no se agrega y no se actualiza el contador interno.
     */
    void test_AddExerciseDuplicado();


    /**
     * @brief Verifica que getExercise() retorna el ejercicio correcto si existe.
     */
    void test_GetExercise_Existe();

    /**
     * @brief Verifica que getExercise() retorna un objeto vacío si no encuentra coincidencia.
     * En este sistema, un objeto vacío tiene id = -1.
     *
     */
    void testGetExercise_NoExiste();

    /**
     * @brief Caja blanca: llama a getExercise() cuando no hay ejercicios (bucle vacío).
     * bucle no ejecutado
     * N=0
     */
    void testGetExercise_BucleVacio();

    /**
     * @brief Caja blanca: llama a getExercise() con un único ejercicio en la lista.
     * bucle ejecutado 1 vez
     */
    void testGetExercise_BucleUnico();

    /**
     * @brief Caja blanca: recorre varios ejercicios hasta encontrar el deseado.
     * bucle ejecutado multipleas veces
     * Esto asegura que el bucle funcione incluso con múltiples iteraciones.
     */
    void testGetExercise_BucleMultiple();
};

#endif // TESTWORKOUTSUMMARY_H
