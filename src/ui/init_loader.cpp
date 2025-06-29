#include "init_loader.h"
#include "enums/ExEspecFieldEnum.h"
#include "profiles/client.h"
#include "profiles/fitnesstrainer.h"
#include "profiles/admin.h"
#include <utility>
#include "repositories/trainingrepository.h"
#include "repositories/userrepository.h"
#include "core/dbmanager.h"
#include "core/loginmanager.h"

class init_LoaderData : public QSharedData
{
public:
};

Init_Loader::Init_Loader()
{}

Init_Loader::~Init_Loader() {}


void Init_Loader::load(QSharedPointer<DBManager> db)
{
       // db->initializeSchema();
        loadExercises(db);
       loadWorkouts(db);
        loadClients(db);
        loadTrainerAndAdmin(db);
        loadTrainingSessions(db);


}

void Init_Loader::loadExercises(QSharedPointer<DBManager> db)
{

     TrainingRepository*  repo = new TrainingRepository(&*db);

    qDebug()<<"Grabar un Espec2 creado con QHash, con States, con Transitions";
    QHash<ExEspecField, QVariant> data2;
     data2[ExEspecField::IdEx] = 100;
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

    espec3->setIdEx(101);
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

    QSharedPointer<ExerciseEspec> espec4(new ExerciseEspec({}));

    espec4->setIdEx(102);
    espec4->setName("Ex1");
    espec4->setSeries(3);
    espec4->setRepetitions(10);

    State s3(-1,-1, 400, 444);
    State s4(-1,-1, 500, 544);
    s3.addAngleConstraint("Constraint1_Ex3,S0",AngleConstraint());
    s3.addAngleConstraint("Constraint2_Ex3,S0",AngleConstraint());
    s4.addAngleConstraint("Constraint1_Ex3,S1",AngleConstraint());
    s4.addAngleConstraint("Constraint2_Ex3,S1",AngleConstraint());

    espec4->addState(s3);
    espec4->addState(s4);
    espec4->addTransition(qMakePair(0, 1), QSet<Condition>{ Condition(ConditionType::EndOfExercise, "Transicion Ex1 0->1") });

    QSharedPointer<ExerciseEspec> espec5(new ExerciseEspec({}));

    espec5->setIdEx(103);
    espec5->setName("Ex1");
    espec5->setSeries(3);
    espec5->setRepetitions(10);

    State s5(-1,-1, 400, 455);
    State s6(-1,-1, 600, 644);
    s5.addAngleConstraint("Constraint1_Ex4,S0",AngleConstraint());
    s5.addAngleConstraint("Constraint2_Ex4,S0",AngleConstraint());
    s6.addAngleConstraint("Constraint1_Ex4,S1",AngleConstraint());
    s6.addAngleConstraint("Constraint2_Ex4,S1",AngleConstraint());

    espec5->addState(s5);
    espec5->addState(s6);
    espec5->addTransition(qMakePair(0, 1), QSet<Condition>{ Condition(ConditionType::EndOfExercise, "Transicion Ex1 0->1") });



    repo->saveEspec(espec3);
    repo->saveEspec(espec2);
     repo->saveEspec(espec4);

 repo->saveEspec(espec5);

    // // Obtener estructura de las tablas desde la base de datos
    // DBTable tableExercises = db->get(TypeBDEnum::ExerciseEspec);
    // DBTable tableStates = db->get(TypeBDEnum::State);
    // DBTable tableConstraints = db->get(TypeBDEnum::AngleConstraint);
    // DBTable tableTransitions = db->get(TypeBDEnum::Transition);

    // // Preparar para escribir datos
    // DBTable exercises(tableExercises.getTableName());
    // exercises.setColumns(tableExercises.getColumnsNames());

    // DBTable states(tableStates.getTableName());
    // states.setColumns(tableStates.getColumnsNames());

    // DBTable constraints(tableConstraints.getTableName());
    // constraints.setColumns(tableConstraints.getColumnsNames());

    // DBTable transitions(tableTransitions.getTableName());
    // transitions.setColumns(tableTransitions.getColumnsNames());

    // int idConstraint = 2001;
    // int idState = 1001;
    // int idEx = 101;

    // auto addExercise = [&](QString nombre, QString desc, QString tipo, QString musculo, QString equipo,
    //                        ROMJoint joint, double minA, double maxA, int series, int reps, int duracion, float peso, int rest) {
    //     // ExerciseEspec
    //     exercises.addRow({idEx, nombre, desc, tipo, musculo, equipo, series, reps, duracion, peso, rest});

    //     QString jointStr = ROMJointToString(joint);

    //     // Estado 1
    //     int state1Id = idState++;
    //     states.addRow({state1Id, idEx, 1000, 4000,1});
    //     constraints.addRow({idConstraint++,  state1Id, idEx, jointStr,minA, minA + 5, minA + 10, minA - 5, 2000, 5000, -1, "Steady", 5.0});

    //     // Estado 2
    //     int state2Id = idState++;
    //     states.addRow({state2Id, idEx, 1000, 4000,2});
    //     constraints.addRow({idConstraint++, state2Id,idEx, jointStr, maxA - 10, maxA, maxA + 10, maxA - 10, 1500, 4000, -1, "Increase", 5.0});

    //     // Transiciones
    //     transitions.addRow({state1Id, state2Id, "Increase", jointStr});
    //     transitions.addRow({state1Id, state2Id, "OptimalForm", jointStr});

    //     ++idEx;
    // };

    // addExercise("Sentadilla", "Ejercicio de tren inferior", "Fuerza", "Cuádriceps", "Peso corporal", ROMJoint::KneeFlexion, 80, 130, 3, 12, 45, 0, 60);
    // addExercise("Curl Bíceps", "Flexión de codo con mancuernas", "Fuerza", "Bíceps", "Mancuernas", ROMJoint::ElbowFlexion, 30, 120, 3, 10, 40, 60, 45);
    // addExercise("Elevación lateral", "Activación del deltoides", "Activación", "Deltoides", "Mancuernas", ROMJoint::ShoulderAbduction, 20, 100, 2, 15, 30, 30, 30);
    // addExercise("Puente de glúteo", "Estabilidad y extensión de cadera", "Estabilidad", "Glúteo mayor", "Colchoneta", ROMJoint::HipExtension, 0, 30, 2, 12, 60, 0, 40);

    // // Guardar
    // db->save(TypeBDEnum::ExerciseEspec, exercises);
    // db->save(TypeBDEnum::State, states);
    // db->save(TypeBDEnum::AngleConstraint, constraints);
    // db->save(TypeBDEnum::Transition, transitions);
}
void Init_Loader::loadWorkouts(QSharedPointer<DBManager> db)
{
    // Obtener estructuras de las tablas reales
    DBTable structureWorkout = db->get(TypeBDEnum::TrainingWorkout);
    DBTable structureWorkoutExercises = db->get(TypeBDEnum::WorkoutExercises);

    DBTable workouts(structureWorkout.getTableName());
    workouts.setColumns(structureWorkout.getColumnsNames());

    DBTable workoutExercises(structureWorkoutExercises.getTableName());
    workoutExercises.setColumns(structureWorkoutExercises.getColumnsNames());

    // Workout 201: tren inferior
    workouts.addRow({201, "Fuerza Tren Inferior", "Entrenamiento con énfasis en piernas y glúteos", 20});
    workoutExercises.addRow({201, 100, 1});
    workoutExercises.addRow({201, 101, 2});

    // Workout 202: tren superior
    workouts.addRow({202, "Fuerza Tren Superior", "Entrenamiento con énfasis en brazos y hombros", 25});
    workoutExercises.addRow({202, 102, 1}); // Curl Bíceps
    workoutExercises.addRow({202, 103, 2}); // Elevación lateral

    // Guardar
    db->save(TypeBDEnum::TrainingWorkout, workouts);
    db->save(TypeBDEnum::WorkoutExercises, workoutExercises);
}
void Init_Loader::loadClients(QSharedPointer<DBManager> db)
{

    UserRepository *repo = new UserRepository(&*db);

    QImage image("config/blank-profile.jpg");
    if (image.isNull()) {
        qWarning() << "No se pudo cargar la imagen del perfil.";
    }

    LoginManager login(db, nullptr);
    QString hashedPass = login.hashPassword("1234");

    qDebug() << "Creamos cliente con todo completo";

    QHash<UserField, QVariant> data1;
    data1[UserField::IdUser] = -1;
    data1[UserField::UserName] = "client1";
    data1[UserField::Email] = "client1@c.com";
    data1[UserField::Pass] = hashedPass;
    data1[UserField::Picture] = image;

    QSharedPointer<Client> client1(new Client(data1));
    client1->setUserType(UserType::Client);
    client1->setJoin_up_date(QDateTime::currentDateTime());
    client1->setLast_login(QDateTime::currentDateTime());
    client1->setWorkout(QDateTime::currentDateTime(), 201);
    client1->setPlan("weekly plan");

    QSharedPointer<ClientProfile> profile1(new ClientProfile);
    profile1->setIdClient(-1);
    profile1->setRecord(GeneralDataField::Age, 30);
    profile1->setRecord(MorfologicalFields::FatPercentage, 18.5);
    client1->setProfile(profile1);

    QHash<UserField, QVariant> data2;
    data2[UserField::IdUser] = -1;
    data2[UserField::UserName] = "client2";
    data2[UserField::Email] = "client2@c.com";
    data2[UserField::Pass] = hashedPass;
    data2[UserField::Picture] = image;

    QSharedPointer<Client> client2(new Client(data2));
    client2->setUserType(UserType::Client);
    client2->setJoin_up_date(QDateTime::currentDateTime());
    client2->setLast_login(QDateTime::currentDateTime());
    client2->setWorkout(QDateTime::currentDateTime(), 202);
    client2->setPlan("weekly plan");

    QSharedPointer<ClientProfile> profile2(new ClientProfile);
    profile2->setIdClient(-1);
    profile2->setRecord(GeneralDataField::Age, 30);
    profile2->setRecord(MorfologicalFields::FatPercentage, 18.5);
    client2->setProfile(profile2);

    repo->saveUser(client1);
    repo->saveClientProfile(client1->getId(), client1->getProfile());

    repo->saveUser(client2);
    repo->saveClientProfile(client2->getId(), client2->getProfile());
}


void Init_Loader::loadTrainerAndAdmin(QSharedPointer<DBManager> db)
{


    DBTable usersS = db->get(TypeBDEnum::User);
    DBTable trainersS = db->get(TypeBDEnum::Trainer);
    DBTable adminsS = db->get(TypeBDEnum::Admin);

    DBTable users(usersS.getTableName()); users.setColumns(usersS.getColumnsNames());
    DBTable trainers(trainersS.getTableName()); trainers.setColumns(trainersS.getColumnsNames());
    DBTable admins(adminsS.getTableName()); admins.setColumns(adminsS.getColumnsNames());

    QDateTime now = QDateTime::currentDateTime();

    // contraseñas
    LoginManager login(db, nullptr);
    QString hashedAdminPass = login.hashPassword("admin123");
    QString hashedTrainerPass = login.hashPassword("trainer123");

    // ADMIN (ID 1)
    users.addRow({1, "admin1", "admin@test.com", hashedAdminPass, "Admin", now.addYears(-1), now, QVariant()});
    admins.addRow({1, 1, "Administrador general", true, true, "10"});

    // TRAINER (ID 4)
    users.addRow({4, "trainer1", "trainer@test.com", hashedTrainerPass, "Trainer", now.addYears(-2), now, QVariant()});
    trainers.addRow({4, 4, "Lic. Ciencias del Deporte", "Fuerza y Rehabilitación", 8, "Experiencia en readaptación y entrenamiento funcional."});

    db->save(TypeBDEnum::User, users);
    db->save(TypeBDEnum::Trainer, trainers);
    db->save(TypeBDEnum::Admin, admins);
}
void Init_Loader::loadTrainingSessions(QSharedPointer<DBManager> db)
{

    // Obtener estructura de tablas
    DBTable sesionS = db->get(TypeBDEnum::TrainingSesion);
    DBTable reportS = db->get(TypeBDEnum::SesionReport);

    DBTable sesiones(sesionS.getTableName()); sesiones.setColumns(sesionS.getColumnsNames());
    DBTable reportes(reportS.getTableName()); reportes.setColumns(reportS.getColumnsNames());

    QDateTime now = QDateTime::currentDateTime();
    QDateTime mesPasado = now.addMonths(-1);
    int idSesion = 3001;

    auto addSesion = [&](int idClient, int idEx, QDateTime fecha, const QString& nombreEjercicio) {
        // Entrenamiento básico de 1 minuto
        sesiones.addRow({idSesion, idClient, idEx, fecha.toString(Qt::ISODate), 60000, nombreEjercicio});

        // Simular condiciones en reporte
        reportes.addRow({idSesion, 1, "ShoulderFlexion", "OptimalForm", 1});
        reportes.addRow({idSesion, 1, "ShoulderFlexion", "Increase", 1});
        reportes.addRow({idSesion, 1, "ShoulderFlexion", "FastMovement", 1});

        ++idSesion;
    };

    for (int idClient : {2, 3}) {
        // Workout 201
        addSesion(idClient, 101, mesPasado.addDays(2), "Sentadilla");
        addSesion(idClient, 104, mesPasado.addDays(4), "Puente de glúteo");
        // Workout 202
        addSesion(idClient, 102, now.addDays(-5), "Curl Bíceps");
        addSesion(idClient, 103, now.addDays(-2), "Elevación lateral");
    }

    // Guardar
    db->save(TypeBDEnum::TrainingSesion, sesiones);
    db->save(TypeBDEnum::SesionReport, reportes);
}

