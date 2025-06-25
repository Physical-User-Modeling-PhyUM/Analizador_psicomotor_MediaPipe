#ifndef INIT_LOADER_H
#define INIT_LOADER_H


// Perfiles de usuario
#include "profiles/client.h"
#include "profiles/admin.h"
#include "profiles/fitnesstrainer.h"
#include "profiles/clientprofile.h"

// Entrenamiento
#include "workouts/exerciseespec.h"
#include "workouts/trainingsesion.h"
#include "workouts/trainingworkout.h"
#include "workouts/exercisesummary.h"
#include "pose/sesionreport.h"

// Estados y restricciones
#include "pose/state.h"
#include "pose/condition.h"
#include "pose/angleconstraint.h"

// Enums de campos y tipos de base de datos
#include "enums/ExEspecFieldEnum.h"
#include "enums/UserFieldEnum.h"
#include "enums/WorkoutFieldEnum.h"
#include "enums/TypeBDEnum.h"
#include "enums/profileEnums.h"

// Qt
#include <QSharedPointer>
#include <QHash>
#include <QVariant>
#include <QDateTime>
#include <QPair>
#include <QSet>

#include <QSharedDataPointer>
#include "core/dbmanager.h"


class Init_Loader {

public:
    Init_Loader();
    void load(QSharedPointer<DBManager> db);
    ~Init_Loader();

private:
    void loadExercises(QSharedPointer<DBManager> db);
    void loadWorkouts(QSharedPointer<DBManager> db);
    void loadClients(QSharedPointer<DBManager> db);
    void loadTrainerAndAdmin(QSharedPointer<DBManager> db);
    void loadTrainingSessions(QSharedPointer<DBManager> db);
    void createEmptyTables(QHash<TypeBDEnum, DBTable>& tables);
};

#endif // INIT_LOADER_H
