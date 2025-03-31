#ifndef MEDICALRECORDSENUM_H
#define MEDICALRECORDSENUM_H


#include <QString>
enum class MedicalRecordsFields {
    PreviousInjuries,
    MedicalLimitations,
    PainZones,
    Unknown
};

inline QString MedicalRecordsFieldsToString(MedicalRecordsFields field) {
    switch (field) {
    case MedicalRecordsFields::PreviousInjuries: return "PreviousInjuries";
    case MedicalRecordsFields::MedicalLimitations: return "MedicalLimitations";
    case MedicalRecordsFields::PainZones: return "PainZones";
    default: return "Unknown";
    }
}

inline MedicalRecordsFields MedicalRecordsFieldsFromString(const QString& str) {
    if (str == "PreviousInjuries") return MedicalRecordsFields::PreviousInjuries;
    if (str == "MedicalLimitations") return MedicalRecordsFields::MedicalLimitations;
    if (str == "PainZones") return MedicalRecordsFields::PainZones;
    return MedicalRecordsFields::Unknown;
}



#endif // MEDICALRECORDSENUM_H
