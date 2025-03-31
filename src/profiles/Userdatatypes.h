#ifndef USERDATATYPES_H
#define USERDATATYPES_H

#include <QString>

enum class UserType{
    Admin,
    Client,
    Trainer,
    Unknown
};

inline QString UserTypeToString(UserType tipo) {
    switch (tipo) {
    case UserType::Admin:      return "Admin";
    case UserType::Client:    return "Client";
    case UserType::Trainer:   return "Trainer";

    }
    return "Unknown";
}

inline UserType  UserTypeFromString(const QString& tipo) {
    if (tipo == "Admin") return UserType::Admin;
    if (tipo == "Client") return UserType::Client;
    if (tipo == "Trainer") return UserType::Trainer;

    return UserType::Unknown;
}

enum class DataType {
    IdUser,
    IdClient,
    IdTrainer,
    IdAdmin,
    Email,
    Name,
    UserName,
    FamilyName,
    Pass,
    Qualification,
    Level,
    Languaje,
    Notifications,
    Units,
    Age,
    Gender,
    Weight,
    Height,
    Plan,

    Unknown

};

inline QString bodyRecordToString(DataType data) {
    switch (data) {
    case DataType::IdUser:      return "IdUser";
    case DataType::IdClient:    return "IdClient";
    case DataType::IdTrainer:   return "IdTrainer";
    case DataType::IdAdmin:     return "IdAdmin";
    case DataType::Email:       return "Email";
    case DataType::Name:       return "Name";
    case DataType::UserName:       return "UserName";
    case DataType::FamilyName:       return "FamilyName";
    case DataType::Pass:       return "Pass";
    case DataType::Qualification:       return "Qualification";
    case DataType::Level:       return "Level";
    case DataType::Languaje:       return "Languaje";
    case DataType::Notifications:       return "Notifications";
    case DataType::Units:       return "Units";
    case DataType::Age:       return "Age";
    case DataType::Gender:       return "Gender";
    case DataType::Weight:       return "Weight";
    case DataType::Height:       return "Height";
    case DataType::Plan:       return "Plan";
    }
    return "Unknown";
}


inline DataType dataTypeFromString(const QString& name) {
    if (name == "idUser") return DataType::IdUser;
    if (name == "IdClient") return DataType::IdClient;
    if (name == "IdTrainer") return DataType::IdTrainer;
    if (name == "IdAdmin") return DataType::IdAdmin;
    if (name == "Email") return DataType::Email;


    if (name == "Name") return DataType::Name;
    if (name == "userName") return DataType::UserName;
    if (name == "FamilyName") return DataType::FamilyName;
    if (name == "pass") return DataType::Pass;
    if (name == "level") return DataType::Level;
    if (name == "Qualification") return DataType::Qualification;
    if (name == "Languaje") return DataType::Languaje;
    if (name == "Notifications") return DataType::Notifications;
    if (name == "Units") return DataType::Units;
    if (name == "Age") return DataType::Age;

    if (name == "Gender") return DataType::Gender;
    if (name == "Weight") return DataType::Weight;
    if (name == "Height") return DataType::Height;
    if (name == "Plan") return DataType::Plan;

    return DataType::Unknown;
}




#endif // USERDATATYPES_H
