#ifndef GENDER_H
#define GENDER_H

#include <QString>

enum class Gender {
    Male,
    Female,
    Unknown
};

inline QString GenderToString(Gender field) {
    switch (field) {
    case Gender::Male: return "Male";
    case Gender::Female: return "Female";
    default: return "Unknown";
    }
}

inline Gender GenderFromString(const QString& str) {
    if (str == "Male") return Gender::Male;
    if (str == "Female") return Gender::Female;
    return Gender::Unknown;
}

#endif // GENDER_H
