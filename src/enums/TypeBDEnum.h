#ifndef TYPEBDENUM_H
#define TYPEBDENUM_H

#include <QString>

enum class TypeBDEnum {
    User,
    Client,
    Trainer,
    Admin,
    ProfileFunctional,
    ProfileMorphology,
    ProfileROM,
    ProfileLifestyle,
    ProfileMedical,
    ProfileGeneral,
    ProfileAditional,
    Preferences,
    ExerciseEspec,
    TrainingWorkout,
    WorkoutExercises,
    Unknown,
};

inline QString TypeBDEnumToString(TypeBDEnum tipo) {
    switch (tipo) {
    case TypeBDEnum::Client: return "Client";
    case TypeBDEnum::Trainer: return "Trainer";
    case TypeBDEnum::Admin: return "Admin";
    case TypeBDEnum::ProfileFunctional: return "ProfileFunctional";
    case TypeBDEnum::ProfileMorphology: return "ProfileMorphology";
    case TypeBDEnum::ProfileROM: return "ProfileROM";
    case TypeBDEnum::ProfileLifestyle: return "ProfileLifestyle";
    case TypeBDEnum::ProfileMedical: return "ProfileMedical";
    case TypeBDEnum::ProfileGeneral: return "ProfileGeneral";
    case TypeBDEnum::Preferences: return "Preferences";
    case TypeBDEnum::ExerciseEspec: return "ExerciseEspec";
    case TypeBDEnum::TrainingWorkout: return "TrainingWorkout";
    case TypeBDEnum::WorkoutExercises: return "WorkoutExercises";
    default: return "Unknown";
    }
}

inline TypeBDEnum TypeBDEnumFromString(const QString& str) {
    if (str == "Admin") return TypeBDEnum::Admin;
    if (str == "Client") return TypeBDEnum::Client;
    if (str == "Trainer") return TypeBDEnum::Trainer;
    return TypeBDEnum::Unknown;
}
enum class DBFilter{

    IdUser,
    IdClient,
    IdTrainer,
    IdAdmin,
    Email,
    Name,
    UserName,
    FamilyName,
    none

};

inline QString BDFilterToString(DBFilter filter) {
    switch (filter) {
    case DBFilter::IdUser: return "IdUser";
    case DBFilter::IdClient: return "IdClient";
    case DBFilter::IdTrainer: return "IdTrainer";
    case DBFilter::Email: return "Email";
    case DBFilter::Name: return "Name";
    case DBFilter::UserName: return "UserName";
    case DBFilter::FamilyName: return "FamilyName";
    default: return "none";
    }
}

#endif // TYPEBDENUM_H


