#include "testworkoutsummary.h"
#include "workouts/workoutsummary.h"
#include "workouts/exercisesummary.h"
#include <QtTest>

/**
 * @brief Inicialización antes de todas las pruebas.
 * En este caso solo muestra un mensaje, pero podría cargar recursos comunes.
 */
void TestWorkoutSummary::initTest() {
    qDebug() << "== Inicio TestWorkoutSummary ==";
}

/**
 * @brief Limpieza final tras ejecutar todas las pruebas.
 * Aquí no se liberan recursos porque no hay externos.
 */
void TestWorkoutSummary::finishTest() {
    qDebug() << "== Fin TestWorkoutSummary ==";
}

/**
 * @brief Prueba simple con valores válidos para todos los campos del constructor.
 * Se espera que todos los getters devuelvan lo mismo que se pasó al constructor.
 */
void TestWorkoutSummary::test_Constructor_ValoresValidos() {
    WorkoutSummary w(1, "Pecho", "Rutina de fuerza", "Entrenador");

    int id=w.getIdWorkout();
    QString t =w.getTitle();
     QString d =w. getDescription();
     QString a =w. getAuthor();
     int tt=w.getTotalExercises();

    QCOMPARE(id , 1);
    QCOMPARE(t, QString("Pecho"));
    QCOMPARE(d, QString("Rutina de fuerza"));
    QCOMPARE(a, QString("Entrenador"));
    QCOMPARE(tt, 0);
}

/**
 * @brief Intenta construir un objeto con datos incompletos o inválidos.
 * Se espera que el sistema no falle, pero asigne -1 como id y deje campos vacíos.
 */
void TestWorkoutSummary::test_Constructor_ValoresInvalidos() {

    WorkoutSummary w(-2, "", "desc", "");

      int id=w.getIdWorkout();
     QString t =w.getTitle();


    QCOMPARE(id, -1);  // id inválido se conserva como -1
    QCOMPARE(t, QString(""));

}

/**
 * @brief Valores límite: id 0, cadenas con espacios antes/después.
 * Se espera que las cadenas sean limpiadas correctamente.
 */

void TestWorkoutSummary::test_ValoresLimite() {

    WorkoutSummary w(0, "  Básico  ", "  Desc  ", "  A  ");

    int id=w.getIdWorkout();
    QString t =w.getTitle();
    QString d =w. getDescription();
    QString a =w. getAuthor();

    QCOMPARE(id, 0);
    QCOMPARE(t, QString("Básico"));
     QCOMPARE(d, QString("Desc"));
    QCOMPARE(a, QString("A"));
}

/**
 * @brief Añade dos  ejercicios y comprueba que el contador interno se actualiza.
 */
void TestWorkoutSummary::test_AddExerciseSummary() {

    WorkoutSummary w(2, "Piernas", "", "Coach");
    ExerciseSummary e1(101, "Sentadilla", "desc", "fuerza", "cuádriceps");
    ExerciseSummary e2(102, "Sentadilla", "desc", "fuerza", "cuádriceps");

    w.addExerciseSummary(e1);
     w.addExerciseSummary(e2);

    int total= w.getTotalExercises();

    QCOMPARE(total, 2);
}

/**
 * @brief Añade un ejercicio dos veces y comprueba que el contador interno no se actualiza.
 */
void TestWorkoutSummary::test_AddExerciseDuplicado() {

    WorkoutSummary w(2, "Piernas", "", "Coach");
    ExerciseSummary e1(101, "Sentadilla", "desc", "fuerza", "cuádriceps");
    ExerciseSummary e2(101, "Sentadilla", "desc", "fuerza", "cuádriceps");

    w.addExerciseSummary(e1);
    w.addExerciseSummary(e2);

    int total= w.getTotalExercises();

    QCOMPARE(total, 1);
}


/**
 * @brief Añade un ejercicio y luego lo consulta por ID.
 * La comparación se hace por ID (no usamos operadores == para simplificar).
 */
void TestWorkoutSummary::test_GetExercise_Existe() {

    WorkoutSummary w(3, "Hombro", "", "");

    ExerciseSummary e(201, "Press militar ", "desc", "fuerza", "deltoides");

    w.addExerciseSummary(e);

    ExerciseSummary res = w.getExercise(201);
    int id=res.getIdEx();
    QString n=res.getName();

    QCOMPARE(id, 201);
    QCOMPARE(n, QString("Press militar "));
}

/**
 * @brief Intenta buscar un ID que no existe. Se espera un objeto vacío (id = -1).
 */

void TestWorkoutSummary::testGetExercise_NoExiste() {

    WorkoutSummary w(3, "Hombro", "", "");
    ExerciseSummary res = w.getExercise(999 );

    int id=res.getIdEx();

    QCOMPARE(id, -1);
}

/**
 * @brief Verifica el comportamiento de getExercise() cuando no hay ejercicios.
 * Debe recorrer 0 elementos y devolver el objeto vacío.
 */
void TestWorkoutSummary::testGetExercise_BucleVacio() {

    WorkoutSummary w(3, "Hombro", "", "");

    ExerciseSummary res =   w.getExercise(1);
     int id=res.getIdEx();


    QCOMPARE(id, -1);
}

/**
 * @brief Se añade un solo ejercicio y se busca por su Id.
 * El bucle tiene una única vuelta.
 */

void TestWorkoutSummary::testGetExercise_BucleUnico() {

    WorkoutSummary w(3, "Hombro", "", "");

    w.addExerciseSummary(ExerciseSummary(10, "Press militar ", "desc", "fuerza", "deltoides"));

    ExerciseSummary res =  w.getExercise( 10);
    int id=res.getIdEx();
    QCOMPARE(id, 10);
}

/**
 * @brief Se añaden varios ejercicios, y se busca uno que no esté ni al principio ni al final.
 * Se comprueba que el bucle recorra más de un elemento y aún así devuelva el correcto.
 */
void TestWorkoutSummary::testGetExercise_BucleMultiple() {

    WorkoutSummary w(3, "Hombro", "", "");

    for (int i = 0; i < 5; ++i)   {
        w.addExerciseSummary(ExerciseSummary(100 + i, "Ej" + QString::number(i), "", "", ""));
    }
    ExerciseSummary res = w.getExercise(103);;
     int id=res.getIdEx();

    QCOMPARE(id, 103);
}
