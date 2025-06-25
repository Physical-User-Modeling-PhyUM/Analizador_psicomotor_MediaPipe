-- Tabla de usuarios
CREATE TABLE IF NOT EXISTS users (
    idUser INTEGER PRIMARY KEY,
    userName TEXT NOT NULL,
    email TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    userType TEXT NOT NULL,
    join_up_date TEXT,
    last_login TEXT,
    profile_picture BLOB
);

-- Clientes
CREATE TABLE IF NOT EXISTS clients (
    idClient INTEGER PRIMARY KEY,
    idUser INTEGER NOT NULL,
    expLevel INTEGER,
    plan TEXT,
    FOREIGN KEY (idUser) REFERENCES users(idUser)
);

-- Entrenadores
CREATE TABLE IF NOT EXISTS fitnesstrainers (
    idTrainer INTEGER PRIMARY KEY,
    idUser INTEGER NOT NULL,
    Qualification TEXT,
    specialtyArea TEXT,
      resume TEXT,
     experience_years INTEGER,
    FOREIGN KEY (idUser) REFERENCES users(idUser)
);

-- Administradores
CREATE TABLE IF NOT EXISTS admins (
    idAdmin INTEGER PRIMARY KEY,
    idUser INTEGER NOT NULL,
    roleDescription TEXT,
    canManageUsers BOOLEAN,
    canModifyPlans BOOLEAN,
    permissionLevel TEXT,
    FOREIGN KEY (idUser) REFERENCES users(idUser)
);

-- Preferencias de usuario
CREATE TABLE IF NOT EXISTS userpreferences (
    idPref INTEGER PRIMARY KEY,
    idUser INTEGER,
    dualMode TEXT,
    infoSound TEXT,
    alertSound TEXT,
    mute TEXT,
    connectionsJson TEXT
);


-- Registros generales de cliente
CREATE TABLE IF NOT EXISTS general_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    Age INTEGER,
    Gender TEXT,
    Weight REAL,
    Height REAL,
    Plan TEXT,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);


-- Registros funcionales
CREATE TABLE IF NOT EXISTS functional_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    StaticPosture TEXT,
    HipMobility TEXT,
    AnkleMobility TEXT,
    ShoulderMobilityLevel TEXT,
    SquatStrength TEXT,
    BalanceTest TEXT,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Registros morfologicos
CREATE TABLE IF NOT EXISTS morfological_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    FatPercentage REAL,
    MuscleMass REAL,
    BMI REAL,
    VO2Max REAL,
    HeartRate INTEGER,
    TorsoLength REAL,
    LegLength REAL,
    ArmLength REAL,
    ForearmLength REAL,
    ThighCircumference REAL,
    WaistCircumference REAL,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Registros Rangos de Movimientos
CREATE TABLE IF NOT EXISTS rom_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    NeckFlexion REAL,
    NeckExtension REAL,
    NeckRotation REAL,
    NeckLateralFlexion REAL,
    ShoulderFlexion REAL,
    ShoulderExtension REAL,
    ShoulderAbduction REAL,
    ShoulderAdduction REAL,
    ShoulderInternalRotation REAL,
    ShoulderExternalRotation REAL,
    ElbowFlexion REAL,
    ElbowExtension REAL,
    ForearmSupination REAL,
    ForearmPronation REAL,
    WristFlexion REAL,
    WristExtension REAL,
    WristRadialDeviation REAL,
    WristUlnarDeviation REAL,
    FingerFlexion REAL,
    ThumbOpposition REAL,
    ThoracicRotation REAL,
    ThoracicLateralFlexion REAL,
    ThoracicExtension REAL,
    LumbarFlexion REAL,
    LumbarExtension REAL,
    LumbarLateralFlexion REAL,
    LumbarRotation REAL,
    HipFlexion REAL,
    HipExtension REAL,
    HipAbduction REAL,
    HipAdduction REAL,
    HipInternalRotation REAL,
    HipExternalRotation REAL,
    KneeFlexion REAL,
    KneeExtension REAL,
    AnkleDorsiflexion REAL,
    AnklePlantarflexion REAL,
    FootInversion REAL,
    FootEversion REAL,
    ToeFlexion REAL,
    ToeExtension REAL,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);


-- Registros de estilo de vida
CREATE TABLE IF NOT EXISTS lifestyle_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    TrainingFrequency TEXT,
    TypeOfTraining TEXT,
    PreviousSports TEXT,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Condiciones médicas
CREATE TABLE IF NOT EXISTS medical_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    PreviousInjuries TEXT,
    MedicalLimitations TEXT,
    PainZones TEXT,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Adicional
CREATE TABLE IF NOT EXISTS aditional_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);




CREATE TABLE client_workout_calendar (
    idClient INTEGER,
    date TEXT,
    idWorkout INTEGER,
    PRIMARY KEY (idClient, date)
);

-- Tabla principal de workouts
CREATE TABLE training_workout (
    idWorkout INTEGER PRIMARY KEY,
    name TEXT,
    description TEXT,
    estimatedDuration INTEGER
);

-- Tabla de ejercicios
CREATE TABLE exercise_espec (
   idEx INTEGER PRIMARY KEY,
    Name TEXT,
   Description TEXT,
   ExerciseType TEXT,
   TargetMuscle TEXT,
    Equipment TEXT,
    Series INTEGER,
    Repetitions INTEGER,
    Duration INTEGER,
   Weight INTEGER,
   RestTime INTEGER
);

-- Relación Workout → ExerciseEspec con orden
CREATE TABLE workout_exercise_espec (
    idWorkout INTEGER,
    idEx INTEGER,
    exerciseOrder INTEGER,
    PRIMARY KEY (idWorkout, idEx),
    FOREIGN KEY (idWorkout) REFERENCES training_workout(idWorkout),
    FOREIGN KEY (idEx) REFERENCES exercise_espec(idEx)
);

-- Estados de un ejercicio
CREATE TABLE state (
    idState INTEGER,
    idEx INTEGER,
    minTime INTEGER,
    maxTime INTEGER,
    orderInList INTEGER,
    name TEXT,
    PRIMARY KEY (idEx, idState),
    FOREIGN KEY (idEx) REFERENCES exercise_espec(idEx)
);

-- Restricciones angulares por estado
CREATE TABLE angle_constraint (
   idConstraint INTEGER,
   idState INTEGER,
   idEx INTEGER,
   line TEXT,
   minAngle REAL,
   maxAngle REAL,
   maxSafeAngle REAL,
   minSafeAngle REAL,
   fastThreshold INTEGER,
   slowThreshold INTEGER,
   symetricalAngle REAL,
   evolution TEXT,
   toler REAL,
   view TEXT,
   PRIMARY KEY (idEx, idState,idConstraint),
   FOREIGN KEY (idState) REFERENCES state(idState)
);

-- Transiciones con condiciones
CREATE TABLE transition (
   idEx INTEGER,
   fromState INTEGER,
   toState INTEGER,
   conditionType TEXT,
   keypointLine TEXT,
   view TEXT,
   PRIMARY KEY (idEx, fromState, toState, conditionType, keypointLine),
   FOREIGN KEY (idEx) REFERENCES exercise_espec(idEx)
);

-- Sesiones de entrenamiento
CREATE TABLE TrainingSesion (
    idSesion INTEGER PRIMARY KEY,
    idClient INTEGER NOT NULL,
    idWorkout INTEGER,
    idEx INTEGER ,
    date TEXT ,
    duration INTEGER ,
    FOREIGN KEY (idClient) REFERENCES Users(idUser),
    FOREIGN KEY (idExercise) REFERENCES ExerciseSummary(idExercise)
);

--Resumenes de los ejercicions de las sesiones
CREATE TABLE ExerciseSummary (
    idEx INTEGER PRIMARY KEY,
    idWorkout INTEGER,
    name TEXT,
    description TEXT,
    type TEXT,
    targetMuscle TEXT,
    series INTEGER,
    repetitions INTEGER,
    duration INTEGER,
    weightPercentage REAL,
    restTime INTEGER
);

--Reportes de las sesiones
CREATE TABLE SesionReport (
    idSesion INTEGER PRIMARY KEY,
    seriesDataJson TEXT,
    globalAnglesJson TEXT,
    globalOverloadsJson TEXT,
    log TEXT
);
