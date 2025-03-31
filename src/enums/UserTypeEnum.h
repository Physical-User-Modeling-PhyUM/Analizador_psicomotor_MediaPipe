#ifndef USERTYPE_H
#define USERTYPE_H

#include <QString>

enum class UserType {
    Admin,
    Client,
    Trainer,
    Unknown
};

inline QString UserTypeToString(UserType field) {
    switch (field) {
    case UserType::Admin: return "Admin";
    case UserType::Client: return "Client";
    case UserType::Trainer: return "Trainer";
    default: return "Unknown";
    }
}

inline UserType UserTypeFromString(const QString& str) {
    if (str == "Admin") return UserType::Admin;
    if (str == "Client") return UserType::Client;
    if (str == "Trainer") return UserType::Trainer;
    return UserType::Unknown;
}

#endif // USERTYPE_H
