#ifndef LIFESTYLEFIELDS_H
#define LIFESTYLEFIELDS_H

#include <QString>
enum class LifeStyleFields {
    TrainingFrequency,
    TypeOfTraining,
    PreviousSports,
    Unknown
};

inline QString LifeStyleFieldsToString(LifeStyleFields field) {
    switch (field) {
    case LifeStyleFields::TrainingFrequency: return "TrainingFrequency";
    case LifeStyleFields::TypeOfTraining: return "TypeOfTraining";
    case LifeStyleFields::PreviousSports: return "PreviousSports";
    default: return "Unknown";
    }
}

inline LifeStyleFields LifeStyleFieldsFromString(const QString& str) {
    if (str == "TrainingFrequency") return LifeStyleFields::TrainingFrequency;
    if (str == "TypeOfTraining") return LifeStyleFields::TypeOfTraining;
    if (str == "PreviousSports") return LifeStyleFields::PreviousSports;
    return LifeStyleFields::Unknown;
}

#endif // LIFESTYLEFIELDS_H
