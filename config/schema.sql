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
    age INTEGER,
    sex TEXT,
    weight REAL,
    height REAL,
    plan TEXT,
    FOREIGN KEY (idUser) REFERENCES users(idUser)
);

-- Entrenadores
CREATE TABLE IF NOT EXISTS fitnesstrainers (
    idTrainer INTEGER PRIMARY KEY,
    idUser INTEGER NOT NULL,
    qualifications TEXT,
    specialtyArea TEXT,
    experience_years INTEGER,
    resumme TEXT,
    FOREIGN KEY (idUser) REFERENCES users(idUser)
);

-- Administradores
CREATE TABLE IF NOT EXISTS admins (
    idAdmin INTEGER PRIMARY KEY,
    idUser INTEGER NOT NULL,
    roleDescription TEXT,
    canManageUsers BOOLEAN,
    canModifyPlans BOOLEAN,
    FOREIGN KEY (idUser) REFERENCES users(idUser)
);

-- Preferencias de usuario
CREATE TABLE IF NOT EXISTS userpreferences (
    idPref INTEGER PRIMARY KEY,
    idUser INTEGER NOT NULL,
    language TEXT,
    notifications BOOLEAN,
    units TEXT,
    FOREIGN KEY (idUser) REFERENCES users(idUser)
);

-- Registros corporales
CREATE TABLE IF NOT EXISTS bodyrecords (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    metric TEXT,
    value TEXT,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Registros de estilo de vida
CREATE TABLE IF NOT EXISTS lifestylerecords (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    type TEXT,
    value TEXT,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Condiciones médicas
CREATE TABLE IF NOT EXISTS medicalconditions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    condition TEXT,
    value TEXT,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Adicional
CREATE TABLE IF NOT EXISTS medicalconditions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    idClient INTEGER NOT NULL,
    FOREIGN KEY (idClient) REFERENCES clients(idClient)
);

-- Especificaciones de ejercicios
CREATE TABLE IF NOT EXISTS exerciseespecs (
    idEx INTEGER PRIMARY KEY,
    name TEXT,
    description TEXT,
    exerciseType TEXT,
    targetMuscle TEXT,
    equipment TEXT,
    series INTEGER,
    repetitions INTEGER,
    duration INTEGER,
    weight INTEGER,
    restTime INTEGER
);

-- Workouts de entrenamiento
CREATE TABLE IF NOT EXISTS trainingworkouts (
    idWorkout INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    description TEXT,
    estimatedDuration INTEGER
);

-- Asociación workout - ejercicios
CREATE TABLE IF NOT EXISTS workout_exerciseespec (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    workoutId INTEGER NOT NULL,
    exerciseId INTEGER NOT NULL,
    "order" INTEGER DEFAULT 0,
    FOREIGN KEY (workoutId) REFERENCES trainingworkouts(idWorkout) ON DELETE CASCADE,
    FOREIGN KEY (exerciseId) REFERENCES exerciseespecs(idEx) ON DELETE CASCADE
);
