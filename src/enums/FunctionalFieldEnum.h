#ifndef FUNCTIONALFIELD_H
#define FUNCTIONALFIELD_H

#include <QString>

enum class FunctionalField {
    StaticPosture,  //Tipo de postura estática: neutra, cifótica, hiperlordótica...
    HipMobility,    //Nivel de movilidad de cadera
    AnkleMobility,  //Nivel de movilidad de tobillo
    ShoulderMobilityLevel, //Nivel de movilidad de hombros
    SquatStrength,      //Valoración funcional del patrón de sentadilla
    BalanceTest,    //Resultado del test de equilibrio
    Unknown
};

inline QString FunctionalFieldToString(FunctionalField field) {
    switch (field) {
    case FunctionalField::StaticPosture: return "StaticPosture";
    case FunctionalField::HipMobility: return "HipMobility";
    case FunctionalField::AnkleMobility: return "AnkleMobility";
    case FunctionalField::SquatStrength: return "SquatStrength";
    case FunctionalField::BalanceTest: return "BalanceTest";
    case FunctionalField::ShoulderMobilityLevel: return "ShoulderMobilityLevel";

    default: return "Unknown";
    }
}

inline FunctionalField FunctionalFieldFromString(const QString& str) {
    if (str == "StaticPosture") return FunctionalField::StaticPosture;
    if (str == "HipMobility") return FunctionalField::HipMobility;
    if (str == "AnkleMobility") return FunctionalField::AnkleMobility;
    if (str == "SquatStrength") return FunctionalField::SquatStrength;
    if (str == "BalanceTest") return FunctionalField::BalanceTest;
    if (str == "ShoulderMobilityLevel") return FunctionalField::ShoulderMobilityLevel;
    return FunctionalField::Unknown;
}

#endif // FUNCTIONALFIELD_H
