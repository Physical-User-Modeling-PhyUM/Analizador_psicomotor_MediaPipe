#ifndef USERFIELD_H
#define USERFIELD_H

#include <QString>

enum class UserField {
    IdUser,
    IdClient,
    IdTrainer,
    IdAdmin,
    Email,
    Name,
    UserName,
    FamilyName,
    Pass,

    Unknown
};

inline QString UserFieldToString(UserField field) {
    switch (field) {

        case UserField::IdUser: return "IdUser";
        case UserField::IdClient: return "IdClient";
        case UserField::IdTrainer: return "IdTrainer";
        case UserField::IdAdmin: return "IdAdmin";
        case UserField::Email: return "Email";
        case UserField::Name: return "Name";
        case UserField::UserName: return "UserName";
        case UserField::FamilyName: return "FamilyName";
        case UserField::Pass: return "Pass";



    default: return "Unknown";
    }
}

inline UserField UserFieldFromString(const QString& str) {

    if (str == "IdUser") return UserField::IdUser;
    if (str == "IdClient") return UserField::IdClient;
    if (str == "IdTrainer") return UserField::IdTrainer;
    if (str == "IdAdmin") return UserField::IdAdmin;
    if (str == "Email") return UserField::Email;
    if (str == "Name") return UserField::Name;
    if (str == "UserName") return UserField::UserName;
    if (str == "FamilyName") return UserField::FamilyName;
    if (str == "Pass") return UserField::Pass;

    return UserField::Unknown;
}

#endif // USERFIELD_H
