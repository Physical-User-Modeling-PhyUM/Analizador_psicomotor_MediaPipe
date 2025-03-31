#ifndef GENERALDATAFIELD_H
#define GENERALDATAFIELD_H

#include <QString>

enum class GeneralDataField {

    Qualification,
    Level,
    Language,
    Notifications,
    Units,
    Age,
    Gender,
    Weight,
    Height,
    Plan,
    Unknown
};

inline QString GeneralDataFieldToString(GeneralDataField field) {
    switch (field) {


        case GeneralDataField::Qualification: return "Qualification";
        case GeneralDataField::Level: return "Level";
        case GeneralDataField::Language: return "Language";
        case GeneralDataField::Notifications: return "Notifications";
        case GeneralDataField::Units: return "Units";
        case GeneralDataField::Age: return "Age";
        case GeneralDataField::Gender: return "Gender";
        case GeneralDataField::Weight: return "Weight";
        case GeneralDataField::Height: return "Height";
        case GeneralDataField::Plan: return "Plan";

    default: return "Unknown";
    }
}

inline GeneralDataField GeneralDataFieldFromString(const QString& str) {


    if (str == "Qualification") return GeneralDataField::Qualification;
    if (str == "Level") return GeneralDataField::Level;
    if (str == "Language") return GeneralDataField::Language;
    if (str == "Notifications")   return GeneralDataField::Notifications;
    if (str == "Units") return GeneralDataField::Units;
    if (str == "Age") return GeneralDataField::Age;
    if (str == "Gender") return GeneralDataField::Gender;
    if (str == "Weight") return GeneralDataField::Weight;
    if (str == "Height") return GeneralDataField::Height;
    if (str == "Plan") return GeneralDataField::Plan;
    return GeneralDataField::Unknown;
}

#endif // GENERALDATAFIELD_H
