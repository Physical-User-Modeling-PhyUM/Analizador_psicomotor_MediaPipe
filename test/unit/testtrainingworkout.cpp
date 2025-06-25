#include "testtrainingworkout.h"
#include "workouts/trainingworkout.h"
#include "workouts/exerciseespec.h"
#include "enums/WorkoutFieldEnum.h"
#include <QtTest>

void TestTrainingWorkout::init_Test() {
    qDebug() << "== TestTrainingWorkout empieza ==";
}

void TestTrainingWorkout::finishTest() {
    qDebug() << "== TestTrainingWorkout termina ==";
}

/**
 * @brief Constructor con datos válidos. Debería inicializar correctamente los campos.
 */
void TestTrainingWorkout::testConstructorValido() {

    QHash<WorkoutField, QVariant> data;
    data[WorkoutField::IdWorkout] = 11;
    data[WorkoutField::Name] = "Full Body";
    data[WorkoutField::Description] = "entreno de todo";
    data[WorkoutField::duration] = 50;

    TrainingWorkout tw(data);

    int id =tw.getIdWorkout();
    QString name=tw.getName();
    QString des=tw.getDescription();
    int est=tw.getEstimatedDuration();

    QCOMPARE(id, 11);
    QCOMPARE(name, QString("Full Body"));
    QCOMPARE(des, QString("entreno de todo"));
    QCOMPARE(est, 50);
}


/**
 * @brief Se añade un ejercicio sin orden específico. Se espera que quede al final.
 */
void TestTrainingWorkout::testAddExercise_porDefecto() {
    QHash<WorkoutField, QVariant> vacio;
    TrainingWorkout tw(vacio);
    QHash<ExEspecField, QVariant> h1=QHash<ExEspecField, QVariant>{{ExEspecField::IdEx, 1}};
    QSharedPointer<ExerciseEspec> ex = QSharedPointer<ExerciseEspec>::create(h1);
    tw.addExercise(ex);

    int size=tw.getExercises().size();
    int id=tw.getExercises().first()->getIdEx();

    QCOMPARE(size, 1);
    QCOMPARE(id, 1);
}

/**
 * @brief Se añade un ejercicio con un orden explícito. Se inserta en posición concreta.
 */
void TestTrainingWorkout::testAddExercise_conOrden() {

    QHash<WorkoutField, QVariant> vacio;

    TrainingWorkout tw(vacio);

    QHash<ExEspecField, QVariant> h2=QHash<ExEspecField, QVariant>{{ExEspecField::IdEx, 2}};
    QHash<ExEspecField, QVariant> h3=QHash<ExEspecField, QVariant>{{ExEspecField::IdEx, 3}};

    QSharedPointer<ExerciseEspec> ex1 = QSharedPointer<ExerciseEspec>::create(h2);
    QSharedPointer<ExerciseEspec> ex2 = QSharedPointer<ExerciseEspec>::create(h3);

    tw.addExercise(ex1);
    tw.addExercise(ex2, 0);  // lo inserta al principio

    int size=tw.getExercises().size();
    int id3=tw.getExercises().first()->getIdEx();

    QCOMPARE(size, 2);
    QCOMPARE(id3, 3);
}

/**
 * @brief Se recupera un ejercicio que sí está en la lista.
 */
void TestTrainingWorkout::testGetExercise_existente() {

    QHash<WorkoutField, QVariant> vacio;
    TrainingWorkout tw(vacio);
    QHash<ExEspecField, QVariant> h5=QHash<ExEspecField, QVariant>{{ExEspecField::IdEx, 5}};

    QSharedPointer<ExerciseEspec> ex = QSharedPointer<ExerciseEspec>::create(h5);
    tw.addExercise(ex);

    QSharedPointer<ExerciseEspec> result = tw.getExercise(5);
    int id=result->getIdEx();

    QVERIFY(result != nullptr);
    QCOMPARE(id, 5);
}

/**
 * @brief Intenta obtener un ejercicio que no existe. El resultado debería ser null.
 */
void TestTrainingWorkout::testGetExercise_noExistente() {

    QHash<WorkoutField, QVariant> vacio;
    TrainingWorkout tw(vacio);

    QSharedPointer<ExerciseEspec>  result = tw.getExercise(42);

    QCOMPARE(result, nullptr);
}

/**
 * @brief Se prueba que el resumen generado tiene los datos correctos.
 * No se comparan todos los campos internos, solo se verifica el id y cantidad de ejercicios.
 */
void TestTrainingWorkout::testToSummary_Correcto() {

    QHash<WorkoutField, QVariant> data;

    data[WorkoutField::IdWorkout] = 88;
    data[WorkoutField::Name] = "Rutina A";
    data[WorkoutField::Description] = "Desc breve";

    TrainingWorkout tw(data);
    QHash<ExEspecField, QVariant> h=QHash<ExEspecField, QVariant>{
        {ExEspecField::IdEx, 1},
        {ExEspecField::Name, "Bíceps"},
        {ExEspecField::TargetMuscle, "bíceps"}
    };

    QSharedPointer<ExerciseEspec> ex = QSharedPointer<ExerciseEspec>::create(h);

    tw.addExercise(ex);

    WorkoutSummary resumen = tw.toSummary();

    QCOMPARE(resumen.getIdWorkout(), 88);
    QCOMPARE(resumen.getTotalExercises(), 1);
}

/**
 * @brief Caja blanca: se prueba el recorrido completo del bucle en getExercise().
 * Se añaden 3 elementos y se busca el último.
 */
void TestTrainingWorkout::testGetExercise_bucleEjecutado() {

    QHash<WorkoutField, QVariant> vacio;
    TrainingWorkout tw(vacio);

    for (int i = 0; i < 3; ++i) {
        QHash<ExEspecField, QVariant>h=QHash<ExEspecField, QVariant>{{ExEspecField::IdEx, i}};
        auto ex = QSharedPointer<ExerciseEspec>::create(h);
        tw.addExercise(ex);
    }
    QSharedPointer<ExerciseEspec> result = tw.getExercise(2);

    QVERIFY(result != nullptr);

    int id =result->getIdEx();

    QCOMPARE(id, 2);
}

/**
 * @brief Caja blanca: se prueba que el bucle de getExercise() no se ejecuta si la lista está vacía.
 */
void TestTrainingWorkout::testGetExercise_bucleVacio() {
    QHash<WorkoutField, QVariant> vacio;
    TrainingWorkout tw(vacio);

    QSharedPointer<ExerciseEspec> res = tw.getExercise(99);
    QVERIFY(res == nullptr);
}
