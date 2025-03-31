#ifndef CLIENTPROFILEDATATYPE_H
#define CLIENTPROFILEDATATYPE_H
#include <QString>

enum class ClientProfileData {
    General,
    Functional,
    Morfological,
    Medical,
    LifeStyle,
    Additional,
    All
};

inline QString ClientProfileDataToString(ClientProfileData field) {
    switch (field) {
    case ClientProfileData::General: return "General";
    case ClientProfileData::Functional: return "Functional";
    case ClientProfileData::Morfological: return "Morfological";
    case ClientProfileData::Medical: return "Medical";
    case ClientProfileData::LifeStyle: return "LifeStyle";
    case ClientProfileData::Additional: return "Additional";
    default: return "All";
    }
}

inline ClientProfileData ClientProfileDataFromString(const QString& str) {
    if (str == "General") return ClientProfileData::General;
    if (str == "Functional") return ClientProfileData::Functional;
    if (str == "Morfological") return ClientProfileData::Morfological;
    if (str == "Medical") return ClientProfileData::Medical;
    if (str == "LifeStyle") return ClientProfileData::LifeStyle;
    if (str == "Additional") return ClientProfileData::Additional;
    return ClientProfileData::All;
}
#endif // CLIENTPROFILEDATATYPE_H
