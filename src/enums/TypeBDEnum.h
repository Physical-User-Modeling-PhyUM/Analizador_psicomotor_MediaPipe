#ifndef TYPEBDENUM_H
#define TYPEBDENUM_H

#include <QString>
/**
 * @file TypeBDEnum.h
 * @brief Define enumeraciones para identificar tipos de datos de la base de datos y filtros aplicables.
 *
 * Este archivo contiene dos enumeraciones principales:
 * - `TypeBDEnum`: Identifica las distintas entidades que se almacenan o consultan en la base de datos del sistema.
 * - `DBFilter`: Define los filtros disponibles para realizar consultas específicas sobre esas entidades.
 *
 * También se proporcionan funciones auxiliares para convertir estas enumeraciones a cadenas de texto y viceversa.
 */

/// Enum que representa los diferentes tipos de datos almacenables en la base de datos del sistema.
enum class TypeBDEnum {
    User,                    ///< Usuario genérico (no usado directamente en toString).
    Client,                  ///< Usuario tipo cliente.
    Trainer,                 ///< Usuario tipo preparador físico.
    Admin,                  ///< Usuario tipo administrador.

    ProfileFunctional,       ///< Perfil funcional del cliente (movilidad, equilibrio, etc.).
    ProfileMorphology,       ///< Perfil morfológico (composición corporal, dimensiones).
    ProfileROM,              ///< Perfil de rango de movimiento articular.
    ProfileLifestyle,        ///< Perfil de estilo de vida (hábitos, frecuencia de entrenamiento).
    ProfileMedical,          ///< Historial médico del cliente.
    ProfileGeneral,          ///< Datos generales del perfil del cliente.
    ProfileAditional,        ///< Información adicional no categorizada.

    ClientWorkoutCalendar,   ///< Calendario de planificación de entrenamientos del cliente.
    Preferences,             ///< Preferencias del usuario.

    ExerciseEspec,           ///< Especificación detallada de un ejercicio.
    TrainingWorkout,         ///< Plan de entrenamiento completo.
    WorkoutExercises,        ///< Relación ejercicios-workouts.
    State,                   ///< Estado individual dentro de un ejercicio.
    AngleConstraint,         ///< Restricciones angulares definidas por el entrenador.
    Transition,              ///< Transiciones entre estados en el modelo de movimiento.
    TrainingSesion,          ///< Sesión de entrenamiento realizada.
    ExerciseSummary,         ///< Resumen de ejecución para un ejercicio específico.
    SesionReport,            ///< Informe completo de una sesión.
    Unknown                  ///< Valor no reconocido o sin clasificar.
};

/**
 * @brief Convierte un valor de TypeBDEnum en una cadena legible.
 * @param tipo Valor de enumeración a convertir.
 * @return Cadena representativa del valor.
 */
inline QString TypeBDEnumToString(TypeBDEnum tipo) {
    switch (tipo) {
    //case TypeBDEnum::User: return "User";
    case TypeBDEnum::Client: return "Client";
    case TypeBDEnum::Trainer: return "Trainer";
    case TypeBDEnum::Admin: return "Admin";
    case TypeBDEnum::ProfileFunctional: return "ProfileFunctional";
    case TypeBDEnum::ProfileMorphology: return "ProfileMorphology";
    case TypeBDEnum::ProfileROM: return "ProfileROM";
    case TypeBDEnum::ProfileLifestyle: return "ProfileLifestyle";
    case TypeBDEnum::ProfileMedical: return "ProfileMedical";
    case TypeBDEnum::ProfileGeneral: return "ProfileGeneral";
    case TypeBDEnum::ClientWorkoutCalendar: return "ClientWorkoutCalendar";
    case TypeBDEnum::Preferences: return "Preferences";
    case TypeBDEnum::ExerciseEspec: return "ExerciseEspec";
    case TypeBDEnum::TrainingWorkout: return "TrainingWorkout";
    case TypeBDEnum::WorkoutExercises: return "WorkoutExercises";
    case TypeBDEnum::State: return "State";
    case TypeBDEnum::AngleConstraint: return "AngleConstraint";
    case TypeBDEnum::Transition: return "Transition";
    case TypeBDEnum::TrainingSesion: return "TrainingSesion";
    case TypeBDEnum::ExerciseSummary: return "ExerciseSummary";
    case TypeBDEnum::SesionReport: return "SesionReport";


    default: return "Unknown";
    }
}
/**
 * @brief Convierte una cadena de texto en un valor del enumerado TypeBDEnum.
 * @param str Cadena con el nombre del tipo.
 * @return Valor correspondiente de TypeBDEnum.
 */
inline TypeBDEnum TypeBDEnumFromString(const QString& str) {
    QString s = str.toLower();

    if (s == "admin") return TypeBDEnum::Admin;
    if (s == "client") return TypeBDEnum::Client;
    if (s == "trainer") return TypeBDEnum::Trainer;

    if (s == "profilefunctional") return TypeBDEnum::ProfileFunctional;
    if (s == "profilemorphology") return TypeBDEnum::ProfileMorphology;
    if (s == "profilerom") return TypeBDEnum::ProfileROM;
    if (s == "profilelifestyle") return TypeBDEnum::ProfileLifestyle;
    if (s == "profilemedical") return TypeBDEnum::ProfileMedical;
    if (s == "profilegeneral") return TypeBDEnum::ProfileGeneral;
    if (s == "profileaditional") return TypeBDEnum::ProfileAditional;
    if (s == "clientworkoutcalendar") return TypeBDEnum::ClientWorkoutCalendar;
    if (s == "preferences") return TypeBDEnum::Preferences;
    if (s == "exerciseespec") return TypeBDEnum::ExerciseEspec;
    if (s == "trainingworkout") return TypeBDEnum::TrainingWorkout;
    if (s == "workoutexercises") return TypeBDEnum::WorkoutExercises;
    if (s == "state") return TypeBDEnum::State;
    if (s == "angleconstraint") return TypeBDEnum::AngleConstraint;
    if (s == "transition") return TypeBDEnum::Transition;
    if (s == "trainingsesion") return TypeBDEnum::TrainingSesion;
    if (s == "exercisesummary") return TypeBDEnum::ExerciseSummary;
    if (s == "sesionreport") return TypeBDEnum::SesionReport;

    return TypeBDEnum::Unknown;
}


/// Enum que representa los filtros que pueden aplicarse al realizar consultas sobre la base de datos.
enum class DBFilter {
    IdUser,           ///< Filtro por ID de usuario.
    IdClient,         ///< Filtro por ID de cliente.
    IdTrainer,        ///< Filtro por ID de entrenador.
    IdAdmin,          ///< Filtro por ID de administrador.
    Email,            ///< Filtro por dirección de correo electrónico.
    Name,             ///< Filtro por nombre.
    UserName,         ///< Filtro por nombre de usuario.
    FamilyName,       ///< Filtro por apellido o nombre familiar.
    IdEx,             ///< Filtro por ID de ejercicio.
    IdWorkout,        ///< Filtro por ID de plan de entrenamiento.
    IdState,          ///< Filtro por ID de estado.
    idConstraint,     ///< Filtro por ID de restricción.
    IdSesion,         ///< Filtro por ID de sesión de entrenamiento.
    none              ///< Sin filtro aplicado (valor por defecto).
};

/**
 * @brief Convierte un valor de DBFilter a una cadena de texto.
 * @param filter Filtro a convertir.
 * @return Cadena que representa el filtro.
 */
inline QString BDFilterToString(DBFilter filter) {
    switch (filter) {
    case DBFilter::IdUser: return "IdUser";
    case DBFilter::IdClient: return "IdClient";
    case DBFilter::IdTrainer: return "IdTrainer";
    case DBFilter::Email: return "Email";
    case DBFilter::Name: return "Name";
    case DBFilter::UserName: return "UserName";
    case DBFilter::FamilyName: return "FamilyName";
    case DBFilter::IdEx: return "IdEx";
    case DBFilter::IdWorkout: return "IdWorkout";
    case DBFilter::IdState: return "IdState";
    case DBFilter::idConstraint: return "idConstraint";
    case DBFilter::IdSesion: return "IdSesion";
    default: return "none";
    }
}


#endif // TYPEBDENUM_H


