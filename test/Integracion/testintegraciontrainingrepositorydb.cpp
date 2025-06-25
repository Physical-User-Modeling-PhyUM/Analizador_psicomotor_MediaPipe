/**
 * @file testintegraciontrainingrepositorydb.cpp
 * @brief Implementación de pruebas de integración para TrainingRepository con base de datos SQLite real.
 */

#include "testintegraciontrainingrepositorydb.h"
#include "workouts/exerciseespec.h"
#include "workouts/trainingsesion.h"
#include "pose/state.h"
#include "pose/condition.h"
#include "pose/sesionreport.h"
#include <QTest>
#include <QDebug>
#include <QFile>

void TestIntegracionTrainingRepositoryDB::initTestCase() {
    dbPath = "testdb.sqlite";
    QFile::remove(dbPath);
    db = new DBManager(dbPath);
    QVERIFY(db->openDatabase(dbPath));
    QVERIFY(db->initializeSchema());

    qDebug() << "[Tablas inicializadas para TrainingRepository]";
    db->get(TypeBDEnum::TrainingWorkout).print();
    db->get(TypeBDEnum::ExerciseEspec).print();
    db->get(TypeBDEnum::WorkoutExercises).print();
    db->get(TypeBDEnum::State).print();
    db->get(TypeBDEnum::AngleConstraint).print();
    db->get(TypeBDEnum::Transition).print();
    db->get(TypeBDEnum::ExerciseSummary).print();

    repo = new TrainingRepository(db);
}

void TestIntegracionTrainingRepositoryDB::cleanupTestCase() {
    delete repo;
    db->closeDatabase();
    delete db;
    QFile::remove(dbPath);
}

void TestIntegracionTrainingRepositoryDB::Test_SaveAndGetExerciseEspec(){

    //Grabar un Espec creado con QHash
    qDebug()<<"Grabar un Espec0 creado con QHash, no States, no Transitions";
    QHash<ExEspecField, QVariant> data;
    data[ExEspecField::Name] = "Qhash";
    data[ExEspecField::Description] = "Ejercicio básico";
    data[ExEspecField::ExersiseType] = "Fuerza";
    data[ExEspecField::TargetMuscle] = "Piernas";
    data[ExEspecField::Series] = 3;
    data[ExEspecField::Repetitions] = 10;
    data[ExEspecField::Duration] = 60;
    data[ExEspecField::Weight] = 70.0f;
    data[ExEspecField::RestTime] = 30;
    QSharedPointer<ExerciseEspec> espec0 = QSharedPointer<ExerciseEspec>::create(data);



    //Grabar un Espec creado con QHash
    qDebug()<<"Grabar un Espec1 creado con QHash, con States, no Transitions";
    QHash<ExEspecField, QVariant> data1;
    data1[ExEspecField::Name] = "Qhash con states";
    data1[ExEspecField::Description] = "Ejercicio básico";
    data1[ExEspecField::ExersiseType] = "Fuerza";
    data1[ExEspecField::TargetMuscle] = "Piernas";
    data1[ExEspecField::Series] = 3;
    data1[ExEspecField::Repetitions] = 10;
    data1[ExEspecField::Duration] = 60;
    data1[ExEspecField::Weight] = 70.0f;
    data1[ExEspecField::RestTime] = 30;
    QSharedPointer<ExerciseEspec> espec1 = QSharedPointer<ExerciseEspec>::create(data1);

    State s1_1(-1,-1, 111, 111);
    State s1_2(-1,-1, 111, 111);

    s1_1.addAngleConstraint("Constraint1_Ex1,S0",AngleConstraint());
    s1_1.addAngleConstraint("Constraint2_Ex1,S0",AngleConstraint());
    s1_2.addAngleConstraint("Constraint1_Ex1,S1",AngleConstraint());
    s1_2.addAngleConstraint("Constraint2_Ex1,S1",AngleConstraint());

    espec1->addState(s1_1);
    espec1->addState(s1_2);

    //Grabar un Espec creado con QHash
    qDebug()<<"Grabar un Espec2 creado con QHash, con States, con Transitions";
    QHash<ExEspecField, QVariant> data2;
    data2[ExEspecField::Name] = "Qhash con states";
    data2[ExEspecField::Description] = "Ejercicio básico";
    data2[ExEspecField::ExersiseType] = "Fuerza";
    data2[ExEspecField::TargetMuscle] = "Piernas";
    data2[ExEspecField::Series] = 3;
    data2[ExEspecField::Repetitions] = 10;
    data2[ExEspecField::Duration] = 60;
    data2[ExEspecField::Weight] = 70.0f;
    data2[ExEspecField::RestTime] = 30;
    QSharedPointer<ExerciseEspec> espec2 = QSharedPointer<ExerciseEspec>::create(data2);

    State s2_1(-1,-1, 222, 222);
    State s2_2(-1,-1, 222, 222);

    s2_1.addAngleConstraint("Constraint1_Ex1,S0",AngleConstraint());
    s2_1.addAngleConstraint("Constraint2_Ex1,S0",AngleConstraint());
    s2_2.addAngleConstraint("Constraint1_Ex1,S1",AngleConstraint());
    s2_2.addAngleConstraint("Constraint2_Ex1,S1",AngleConstraint());

    espec2->addState(s2_1);
    espec2->addState(s2_2);
    espec2->addTransition(qMakePair(0, 1), QSet<Condition>{ Condition(ConditionType::EndOfExercise, "Transicion Ex1 0->1") });

    QSharedPointer<ExerciseEspec> espec3(new ExerciseEspec({}));

    espec3->setIdEx(-1);
    espec3->setName("Ex1");
    espec3->setSeries(3);
    espec3->setRepetitions(10);

    State s1(-1,-1, 100, 400);
    State s2(-1,-1, 200, 600);
    s1.addAngleConstraint("Constraint1_Ex1,S0",AngleConstraint());
    s1.addAngleConstraint("Constraint2_Ex1,S0",AngleConstraint());
    s2.addAngleConstraint("Constraint1_Ex1,S1",AngleConstraint());
    s2.addAngleConstraint("Constraint2_Ex1,S1",AngleConstraint());

    espec3->addState(s1);
    espec3->addState(s2);
    espec3->addTransition(qMakePair(0, 1), QSet<Condition>{ Condition(ConditionType::EndOfExercise, "Transicion Ex1 0->1") });

    QVERIFY(repo->saveEspec(espec3));
    QVERIFY(repo->saveEspec(espec2));
    QVERIFY(repo->saveEspec(espec1));
    QVERIFY(repo->saveEspec(espec0));
    //visualizamos las tablas
    qDebug()<<"///visualizamos las tablas tras grabar la Especificacion de ejercicio///";
    db->get(TypeBDEnum::TrainingWorkout).print();
    db->get(TypeBDEnum::ExerciseEspec).print();
    db->get(TypeBDEnum::WorkoutExercises).print();
    db->get(TypeBDEnum::State).print();
    db->get(TypeBDEnum::AngleConstraint).print();
    db->get(TypeBDEnum::Transition).print();
    db->get(TypeBDEnum::ExerciseSummary).print();

    QSharedPointer<ExerciseEspec> result=repo->getEspec(espec3->getIdEx());

    QVERIFY(result != nullptr);
    QCOMPARE(result->getName(), QString("Ex1"));
    QCOMPARE(result->getStatesList().size(), 2);

    State s=result->getState(1);
    QCOMPARE(s.getMaxTime(),600);
    QHash<QString,AngleConstraint> h=s.getConstraints();
    AngleConstraint a=h.value("Constraint1_Ex1,S1");
    QCOMPARE(a.getEvolution(),Direction::Not_matter);

}



void TestIntegracionTrainingRepositoryDB::test_SaveAndGetWorkout_Valido() {
    // Crear especificación de ejercicio
    QSharedPointer<ExerciseEspec> espec(new ExerciseEspec({}));
    espec->setIdEx(-1);
    espec->setName("Ex2");
    espec->setSeries(3);
    espec->setRepetitions(10);

    State s1(-1,-1, 100, 400);
    State s2(-1,-1, 200, 600);
    s1.addAngleConstraint("Constraint1_Ex2,S0", AngleConstraint());
    s1.addAngleConstraint("Constraint2_Ex2,S0", AngleConstraint());
    s2.addAngleConstraint("Constraint1_Ex2,S1", AngleConstraint());
    s2.addAngleConstraint("Constraint2_Ex2,S1", AngleConstraint());

    espec->addState(s1);
    espec->addState(s2);
    espec->addTransition(qMakePair(0, 1), QSet<Condition>{ Condition(ConditionType::EndOfExercise, "Transicion Ex2 0->1") });

    // Crear workout y añadir ejercicio
     QHash<WorkoutField, QVariant> data;
    QSharedPointer<TrainingWorkout> workout(new TrainingWorkout(data));
    workout->setIdWorkout(-1);
    workout->setName("W1");
    QVERIFY(workout->addExercise(espec));

    // Guardar workout
    QVERIFY(repo->saveWorkout(workout));

    // Visualizar tablas tras el guardado
    qDebug() << "///visualizamos las tablas después de guardar el Workout///";
    db->get(TypeBDEnum::TrainingWorkout).print();
    db->get(TypeBDEnum::ExerciseEspec).print();
    db->get(TypeBDEnum::WorkoutExercises).print();
    db->get(TypeBDEnum::State).print();
    db->get(TypeBDEnum::AngleConstraint).print();
    db->get(TypeBDEnum::Transition).print();
    db->get(TypeBDEnum::ExerciseSummary).print();

    // Obtener el workout guardado
    QSharedPointer<TrainingWorkout> result = repo->getWorkout(workout->getIdWorkout());
    QVERIFY(result != nullptr);
    QCOMPARE(result->getName(), QString("W1"));

    const QList<QSharedPointer<ExerciseEspec>> ejercicios = result->getExercises();
    QCOMPARE(ejercicios.size(), 1);

    QSharedPointer<ExerciseEspec> ex = ejercicios.first();
    QVERIFY(ex != nullptr);

    const QList<State> estados = ex->getStatesList();
    QCOMPARE(estados.size(), 2);

    State s = estados[1];
    QCOMPARE(s.getMaxTime(), 600);


    QHash<QString,AngleConstraint> h=s.getConstraints();
    AngleConstraint a=h.value("Constraint2_Ex2,S1");
    QCOMPARE(a.getEvolution(),Direction::Not_matter);

}


void TestIntegracionTrainingRepositoryDB::test_DeleteWorkoutFull() {
    QSharedPointer<ExerciseEspec> espec(new ExerciseEspec({}));

    espec->setIdEx(-1);
    espec->setName("Ex3 borrar");
    espec->setSeries(3);
    espec->setRepetitions(10);

    State s1(-1,-1, 100, 555);
    State s2(-1,-1, 200, 555);
    s1.addAngleConstraint("Constraint1_Ex3,S0 para_Borrar",AngleConstraint());
    s2.addAngleConstraint("Constraint1_Ex3,S1 Para_Borrar",AngleConstraint());
    espec->addState(s1);
    espec->addState( s2);
    espec->addTransition(qMakePair(0, 1), QSet<Condition>{ Condition(ConditionType::EndOfExercise, "Transicion Ex3 0->1 borrar") });
    QHash<WorkoutField, QVariant> data;

    QSharedPointer<TrainingWorkout> workout(new TrainingWorkout(data));
    workout->setIdWorkout(-1);
    workout->setName("W2 borrar");
    workout->addExercise(espec);

    QVERIFY(repo->saveWorkout(workout));

    //visualizamos las tablas
    qDebug()<<"///vsualizamos las tablas antes de borrar el Workout///";
    db->get(TypeBDEnum::TrainingWorkout).print();
    db->get(TypeBDEnum::ExerciseEspec).print();
    db->get(TypeBDEnum::WorkoutExercises).print();
    db->get(TypeBDEnum::State).print();
    db->get(TypeBDEnum::AngleConstraint).print();
    db->get(TypeBDEnum::Transition).print();
    db->get(TypeBDEnum::ExerciseSummary).print();

    QVERIFY(repo->deleteWorkoutfull(workout->getIdWorkout()));
    QVERIFY(repo->getWorkout(workout->getIdWorkout()) == nullptr);

    //visualizamos las tablas
    qDebug()<<"///////vsualizamos las tablas despues de borrar el Workout////";
    db->get(TypeBDEnum::TrainingWorkout).print();
    db->get(TypeBDEnum::ExerciseEspec).print();
    db->get(TypeBDEnum::WorkoutExercises).print();
    db->get(TypeBDEnum::State).print();
    db->get(TypeBDEnum::AngleConstraint).print();
    db->get(TypeBDEnum::Transition).print();
    db->get(TypeBDEnum::ExerciseSummary).print();

}
void TestIntegracionTrainingRepositoryDB::test_SaveAndGet_TrainingSesion()
{
    // Crear resumen del ejercicio
    QSharedPointer<ExerciseSummary> summary(new ExerciseSummary(5, "Sentadilla", "Piernas completas", "Fuerza", "Cuádriceps"));

    qDebug()<<"///vsualizamos las tablas antes de incluir la Sesión de entrenamiento///";
    db->get(TypeBDEnum::TrainingSesion).print();
    db->get(TypeBDEnum::SesionReport).print();
    // Crear sesión
    QSharedPointer<TrainingSesion> sesion(new TrainingSesion(summary, QDateTime::currentDateTime(), 42));
    sesion->setDuration(90000);

    // Crear SesionReport con múltiples datos
    SesionReport report;
    report.addCondition(0, 1,0, Condition(ConditionType::EndOfRepetition, "L_Knee-L_Hip"));
    report.addCondition(0, 1,0, Condition(ConditionType::EndOfRepetition, "L_Knee-L_Hip"));
    report.addCondition(0, 1, 0,Condition(ConditionType::EndOfSet,        "L_Knee-L_Hip"));
    report.addCondition(0, 2, 0,Condition(ConditionType::EndOfRepetition, "R_Ankle-R_Hip"));
    report.addCondition(0, 2, 0,Condition(ConditionType::EndOfRepetition, "R_Ankle-R_Hip"));
    report.addCondition(0, 2,0, Condition(ConditionType::EndOfRepetition, "R_Ankle-R_Hip"));
    report.addCondition(0, 2,0, Condition(ConditionType::EndOfExercise,   "R_Ankle-R_Hip"));

    sesion->setReport(report);

    QVERIFY(repo->SaveTrainingSesion(sesion));
    int idSesion = sesion->getIdSesion();

    QVERIFY(idSesion != -1);
    qDebug()<<"///vsualizamos las tablas despues de incluir la Sesión de entrenamiento///";
    db->get(TypeBDEnum::TrainingSesion).print();
    db->get(TypeBDEnum::SesionReport).print();

    QSharedPointer<TrainingSesion> recuperada = repo->getTrainingSesion(idSesion);
    QVERIFY(recuperada != nullptr);
    QCOMPARE(recuperada->getIdClient(), 42);
    QCOMPARE(recuperada->getExerciseSummary()->getName(), QString("Sentadilla"));
    QCOMPARE(recuperada->getDuration(), 90000);


    SesionReport r = recuperada->getReport();
    // QCOMPARE(r.getConditionCount(0, 1, "L_Knee-L_Hip", ConditionType::EndOfRepetition), 2);
    // QCOMPARE(r.getConditionCount(0, 1, "L_Knee-L_Hip", ConditionType::EndOfSet),        1);
    // QCOMPARE(r.getConditionCount(0, 2, "R_Ankle-R_Hip", ConditionType::EndOfRepetition), 3);
    // QCOMPARE(r.getConditionCount(0, 2, "R_Ankle-R_Hip", ConditionType::EndOfExercise),   1);

}

