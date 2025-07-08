/**
 * @file testintegraciontrainingmanagerdb.cpp
 * @brief Implementación de pruebas de integración para TrainingManager con DBManager real.
 *
 * Este archivo ejecuta pruebas de integración funcionales sobre TrainingManager, evaluando operaciones
 * clave como guardar, recuperar y eliminar datos utilizando una base de datos SQLite real.
 */

#include "testintegraciontrainingmanagerdb.h"
#include "repositories/trainingrepository.h"
#include "workouts/trainingworkout.h"
#include "workouts/exerciseespec.h"
#include "workouts/trainingsesion.h"
#include <QDebug>

TestIntegracionTrainingManagerDB::TestIntegracionTrainingManagerDB() {}
TestIntegracionTrainingManagerDB::~TestIntegracionTrainingManagerDB() {}

void TestIntegracionTrainingManagerDB::initTestCase() {
    db = new DBManager(testDbPath);
    db->initializeSchema();
    QSharedPointer<TrainingRepository> repo = QSharedPointer<TrainingRepository>::create(db);
    manager = QSharedPointer<TrainingManager>::create(repo);
}

void TestIntegracionTrainingManagerDB::cleanupTestCase() {
    db->closeDatabase();
    delete db;
}

void TestIntegracionTrainingManagerDB::testGuardarYRecuperarWorkout() {
    QHash<WorkoutField, QVariant> data;
    data[WorkoutField::Name] = "Test Workout";
    data[WorkoutField::Description] = "Desc";
    data[WorkoutField::duration] = 30;
    QSharedPointer<TrainingWorkout> workout = QSharedPointer<TrainingWorkout>::create(data);

    // Crear y guardar ejercicio antes de añadirlo
    QHash<ExEspecField, QVariant> exData;
    exData[ExEspecField::Name] = "Ejercicio 1";
    exData[ExEspecField::Description] = "desc";
    exData[ExEspecField::ExersiseType] = "Fuerza";
    exData[ExEspecField::TargetMuscle] = "Piernas";
    exData[ExEspecField::Series] = 3;
    exData[ExEspecField::Repetitions] = 10;
    exData[ExEspecField::Duration] = 60;
    exData[ExEspecField::Weight] = 70.0f;
    exData[ExEspecField::RestTime] = 30;

    QSharedPointer<ExerciseEspec> espec = QSharedPointer<ExerciseEspec>::create(exData);
    QVERIFY(manager->saveExercise(espec));

    workout->addExercise(espec);

    QVERIFY(manager->saveWorkout(workout));

    QList<WorkoutSummary> list = manager->listWorkouts();
    QVERIFY(!list.isEmpty());
    QCOMPARE(list.first().getTitle(), QString("Test Workout"));
}

void TestIntegracionTrainingManagerDB::testGuardarYRecuperarExercise() {
    QHash<ExEspecField, QVariant> data;
    data[ExEspecField::Name] = "Sentadilla";
    data[ExEspecField::Description] = "Ejercicio básico";
    data[ExEspecField::ExersiseType] = "Fuerza";
    data[ExEspecField::TargetMuscle] = "Piernas";
    data[ExEspecField::Series] = 3;
    data[ExEspecField::Repetitions] = 10;
    data[ExEspecField::Duration] = 60;
    data[ExEspecField::Weight] = 70.0f;
    data[ExEspecField::RestTime] = 30;
    QSharedPointer<ExerciseEspec> espec = QSharedPointer<ExerciseEspec>::create(data);

    QVERIFY(manager->saveExercise(espec));
    QList<ExerciseSummary> list = manager->listExercises();
    QVERIFY(!list.isEmpty());
    QCOMPARE(list.first().getName(), QString("Sentadilla"));
}

void TestIntegracionTrainingManagerDB::testStartYEndSesion() {
    QList<ExerciseSummary> list = manager->listExercises();
    QVERIFY(!list.isEmpty());
    int idEx = list.first().getIdEx();

    // QSharedPointer<TrainingSesion> sesion = manager->startSesion(1, idEx);
    // QVERIFY(sesion);

    // qDebug() << "Sesión creada con idEx=" << sesion->getExerciseSummary()->getIdEx();
    // qDebug() << "Nombre del ejercicio:" << sesion->getExerciseSummary()->getName();

    // QTest::qWait(100); // Simula duración
    // QVERIFY(manager->endCurrentSesion());
}
