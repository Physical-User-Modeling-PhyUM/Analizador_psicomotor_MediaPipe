#ifndef MORFOLOGICALFIELD_H
#define MORFOLOGICALFIELD_H

#include <QString>

enum class MorfologicalFields {

    FatPercentage,
    MuscleMass,
    BMI,
    VO2Max,
    HeartRate,
    TorsoLength,
    LegLength,
    ArmLength,
    ForearmLength,
    ThighCircumference,
    WaistCircumference,
    Unknown
};

inline QString MorfologicalFieldsToString(MorfologicalFields field) {
    switch (field) {
    case MorfologicalFields::FatPercentage: return "FatPercentage";
    case MorfologicalFields::MuscleMass: return "MuscleMass";
    case MorfologicalFields::BMI: return "BMI";
    case MorfologicalFields::VO2Max: return "VO2Max";
    case MorfologicalFields::HeartRate: return "HeartRate";
    case MorfologicalFields::TorsoLength: return "TorsoLength";
    case MorfologicalFields::LegLength: return "LegLength";
    case MorfologicalFields::ArmLength: return "ArmLength";
    case MorfologicalFields::ForearmLength: return "ForearmLength";
    case MorfologicalFields::ThighCircumference: return "ThighCircumference";
    case MorfologicalFields::WaistCircumference: return "WaistCircumference";
    default: return "Unknown";
    }
}

inline MorfologicalFields MorfologicalFieldsFromString(const QString& str) {


    if (str == "FatPercentage") return MorfologicalFields::FatPercentage;
    if (str == "MuscleMass") return MorfologicalFields::MuscleMass;
    if (str == "BMI") return MorfologicalFields::BMI;
    if (str == "VO2Max") return MorfologicalFields::VO2Max;
    if (str == "HeartRate") return MorfologicalFields::HeartRate;
    if (str == "TorsoLength") return MorfologicalFields::TorsoLength;
    if (str == "LegLength") return MorfologicalFields::LegLength;
    if (str == "ArmLength") return MorfologicalFields::ArmLength;
    if (str == "ForearmLength") return MorfologicalFields::ForearmLength;
    if (str == "ThighCircumference") return MorfologicalFields::ThighCircumference;
    if (str == "WaistCircumference") return MorfologicalFields::WaistCircumference;
    return MorfologicalFields::Unknown;
}


#endif // MORFOLOGICALFIELD_H
