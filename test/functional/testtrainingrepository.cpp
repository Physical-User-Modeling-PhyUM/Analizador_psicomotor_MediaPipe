/**
 * @file testtrainingrepository.cpp
 * @brief Implementación de pruebas funcionales para TrainingRepository.
 */
#include "testtrainingrepository.h"
#include "workouts/trainingworkout.h"
#include "workouts/trainingsesion.h"
#include "workouts/exercisesummary.h"
#include "pose/sesionreport.h"
#include "pose/condition.h"

void TestTrainingRepository::initTestCase() {
    fakeDB = new FakeDBManager();
    repo = new TrainingRepository(fakeDB);

    // Agragamos las tablas vacias que vamosa utilizar
    DBTable w=DBTable("training_workout");
    w.setColumns({"idWorkout","name","description","estimatedDuration"});
    fakeDB->setTestTable(TypeBDEnum::TrainingWorkout,w);

    DBTable rel("WorkoutExercises");
    rel.setColumns({"idWorkout", "idEx", "exerciseOrder"});
     fakeDB->setTestTable(TypeBDEnum::WorkoutExercises,rel);

    DBTable e=DBTable("ExerciseEspec");
    e.setColumns({
        "idEx", "name", "description", "type", "targetMuscle", "equipment","series", "repetitions", "duration", "weightPercentage", "restTime"
    });
    fakeDB->setTestTable(TypeBDEnum::ExerciseEspec,e);

    DBTable states=DBTable ("States");
    states.setColumns({"idEx", "minTime", "maxTime", "orderInList"});
     fakeDB->setTestTable(TypeBDEnum::State,states);

    DBTable acTable("AngleConstraint");
    acTable.setColumns({
            "idState", "line", "minAngle", "maxAngle",
            "maxSafeAngle", "minSafeAngle", "fastThreshold", "slowThreshold",
            "symetricalAngle", "evolution", "toler"
        });
     fakeDB->setTestTable(TypeBDEnum::AngleConstraint,acTable);

    DBTable trTable("Transition");
    trTable.setColumns({"idEx", "fromState", "toState", "conditionType", "keypointLine"});
    fakeDB->setTestTable(TypeBDEnum::Transition,trTable);

    DBTable reportTable=DBTable("SesionReport");
    reportTable.setColumns({"idSesion", "repetition", "keypointLine", "conditionType", "count"});
     fakeDB->setTestTable(TypeBDEnum::SesionReport,reportTable);

    DBTable sum=DBTable("ExerciseSummary");
    sum.setColumns({"idExercise", "name", "descricion","type", "targetMuscle"});
    fakeDB->setTestTable(TypeBDEnum::ExerciseSummary,sum);

    DBTable sessionTable("TrainingSesion");
    sessionTable.setColumns({"idSesion", "idClient", "idExercise", "date", "duration"});
    fakeDB->setTestTable(TypeBDEnum::TrainingSesion,sessionTable);
}

void TestTrainingRepository::cleanupTestCase() {
    delete repo;
    delete fakeDB;
}

void TestTrainingRepository::test_SaveAndGetWorkout_Valido() {
    State e1=State(1,1,5);
    State e2=State(2,1,5);
    e1.addAngleConstraint("L_shoulder_L_Elbow",AngleConstraint());
    e2.addAngleConstraint("R_shoulder_R_Elbow",AngleConstraint());
    //ExerciseEspec e=new ExerciseEspec({});
    QSharedPointer<ExerciseEspec> espec(new ExerciseEspec({}));
    espec->setIdEx(-1);
    espec->setName("Sentadilla");
    espec->setSeries(3);
    espec->setRepetitions(10);
    espec->addState(e1);
    espec->addState(e2);
    espec->addTransition(QPair(1,2),QSet({Condition(ConditionType::EndOfExercise,"s-e")}));

    QHash<WorkoutField, QVariant> data;
    QSharedPointer<TrainingWorkout> workout(new TrainingWorkout(data));

    workout->setIdWorkout(-1);
    workout->setName("Rutina pierna");
    workout->addExercise(espec);

    bool result=repo->saveWorkout(workout);

    QVERIFY(result);

    QSharedPointer<TrainingWorkout> rec = repo->getWorkout(workout->getIdWorkout());

    QVERIFY(rec != nullptr);

    QCOMPARE(rec->getName(), QString("Rutina pierna"));
    QCOMPARE(rec->getExercises().size(), 1);
}

void TestTrainingRepository::test_SaveWorkout_Nulo() {

    QVERIFY(!repo->saveWorkout(nullptr));
}

void TestTrainingRepository::test_DeleteWorkoutFull_Valido() {

    QHash<WorkoutField, QVariant> data;
    QSharedPointer<TrainingWorkout> workout(new TrainingWorkout(data));

    workout->setIdWorkout(-1);
    workout->setName("Para borrar");

    repo->saveWorkout(workout);

    QVERIFY(repo->deleteWorkoutfull(workout->getIdWorkout()));
    QVERIFY(repo->getWorkout(workout->getIdWorkout()) == nullptr);
}

void TestTrainingRepository::test_SaveTrainingSesion_Valida() {
    //ExerciseSummary e= ExerciseSummary(1, "Press Banca", "Empuje horizontal", "Fuerza", "Pectoral");
    //QSharedPointer<ExerciseSummary> p(&e);
    QSharedPointer<ExerciseSummary> summary(new ExerciseSummary(1, "Press Banca", "Empuje horizontal", "Fuerza", "Pectoral"));
    QSharedPointer<TrainingSesion> sesion(new TrainingSesion(summary, QDateTime::currentDateTime(), 10));

    sesion->setDuration(45000);

    QVERIFY(repo->SaveTrainingSesion(sesion));
}

void TestTrainingRepository::test_SaveEspec_MultiplesEstados() {

    QSharedPointer<ExerciseEspec> espec(new ExerciseEspec({}));

    espec->setIdEx(-1);
    espec->setName("Flexi");
    espec->addState(State(1, 100, 500));
    espec->addState(State(2, 200, 700));
    QVERIFY(repo->saveEspec(espec));
}

void TestTrainingRepository::test_SaveEspec_TransicionesMultiples() {

    QSharedPointer<ExerciseEspec> espec(new ExerciseEspec({}));
    espec->setIdEx(-1);
    espec->setName("Con transiciones");
    QSet<Condition> transiciones;
    transiciones.insert(Condition(ConditionType::EndOfSet, "line1"));
    espec->addTransition(qMakePair(0, 1), transiciones);
    QVERIFY(repo->saveEspec(espec));
}

void TestTrainingRepository::test_SaveEspec_Invalida() {
    QSharedPointer<ExerciseEspec> espec;
    QVERIFY(!repo->saveEspec(espec));
}

void TestTrainingRepository::test_GetWorkout_NoExiste() {

    QSharedPointer<TrainingWorkout> rec = repo->getWorkout(999);
    QVERIFY(rec == nullptr);
}

void TestTrainingRepository::test_DeleteEspec_ValidacionCompleta() {

    QSharedPointer<ExerciseEspec> espec(new ExerciseEspec({}));
    espec->setIdEx(50);
    espec->setName("Burpee");
    QVERIFY(repo->saveEspec(espec));
    QVERIFY(repo->deleteEspec(espec->getIdEx()));
}
