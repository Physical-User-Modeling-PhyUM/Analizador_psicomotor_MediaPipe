#ifndef ROMFIELDS_H
#define ROMFIELDS_H


#include <QString>

enum class ROMJoint {
    NeckFlexion,
    NeckExtension,
    NeckRotation,
    NeckLateralFlexion,
    ShoulderFlexion,
    ShoulderExtension,
    ShoulderAbduction,
    ShoulderAdduction,
    ShoulderInternalRotation,
    ShoulderExternalRotation,
    ElbowFlexion,
    ElbowExtension,
    ForearmSupination,
    ForearmPronation,
    WristFlexion,
    WristExtension,
    WristRadialDeviation,
    WristUlnarDeviation,
    FingerFlexion,
    ThumbOpposition,
    ThoracicRotation,
    ThoracicLateralFlexion,
    ThoracicExtension,
    LumbarFlexion,
    LumbarExtension,
    LumbarLateralFlexion,
    LumbarRotation,
    HipFlexion,
    HipExtension,
    HipAbduction,
    HipAdduction,
    HipInternalRotation,
    HipExternalRotation,
    KneeFlexion,
    KneeExtension,
    AnkleDorsiflexion,
    AnklePlantarflexion,
    FootInversion,
    FootEversion,
    ToeFlexion,
    ToeExtension,
    Unknown
};

inline QString ROMJointToString(ROMJoint field) {
    switch (field) {
    case ROMJoint::NeckFlexion: return "NeckFlexion";
    case ROMJoint::NeckExtension: return "NeckExtension";
    case ROMJoint::NeckRotation: return "NeckRotation";
    case ROMJoint::NeckLateralFlexion: return "NeckLateralFlexion";
    case ROMJoint::ShoulderFlexion: return "ShoulderFlexion";
    case ROMJoint::ShoulderExtension: return "ShoulderExtension";
    case ROMJoint::ShoulderAbduction: return "ShoulderAbduction";
    case ROMJoint::ShoulderAdduction: return "ShoulderAdduction";
    case ROMJoint::ShoulderInternalRotation: return "ShoulderInternalRotation";
    case ROMJoint::ShoulderExternalRotation: return "ShoulderExternalRotation";
    case ROMJoint::ElbowFlexion: return "ElbowFlexion";
    case ROMJoint::ElbowExtension: return "ElbowExtension";
    case ROMJoint::ForearmSupination: return "ForearmSupination";
    case ROMJoint::ForearmPronation: return "ForearmPronation";
    case ROMJoint::WristFlexion: return "WristFlexion";
    case ROMJoint::WristExtension: return "WristExtension";
    case ROMJoint::WristRadialDeviation: return "WristRadialDeviation";
    case ROMJoint::WristUlnarDeviation: return "WristUlnarDeviation";
    case ROMJoint::FingerFlexion: return "FingerFlexion";
    case ROMJoint::ThumbOpposition: return "ThumbOpposition";
    case ROMJoint::ThoracicRotation: return "ThoracicRotation";
    case ROMJoint::ThoracicLateralFlexion: return "ThoracicLateralFlexion";
    case ROMJoint::ThoracicExtension: return "ThoracicExtension";
    case ROMJoint::LumbarFlexion: return "LumbarFlexion";
    case ROMJoint::LumbarExtension: return "LumbarExtension";
    case ROMJoint::LumbarLateralFlexion: return "LumbarLateralFlexion";
    case ROMJoint::LumbarRotation: return "LumbarRotation";
    case ROMJoint::HipFlexion: return "HipFlexion";
    case ROMJoint::HipExtension: return "HipExtension";
    case ROMJoint::HipAbduction: return "HipAbduction";
    case ROMJoint::HipAdduction: return "HipAdduction";
    case ROMJoint::HipInternalRotation: return "HipInternalRotation";
    case ROMJoint::HipExternalRotation: return "HipExternalRotation";
    case ROMJoint::KneeFlexion: return "KneeFlexion";
    case ROMJoint::KneeExtension: return "KneeExtension";
    case ROMJoint::AnkleDorsiflexion: return "AnkleDorsiflexion";
    case ROMJoint::AnklePlantarflexion: return "AnklePlantarflexion";
    case ROMJoint::FootInversion: return "FootInversion";
    case ROMJoint::FootEversion: return "FootEversion";
    case ROMJoint::ToeFlexion: return "ToeFlexion";
    case ROMJoint::ToeExtension: return "ToeExtension";
    default: return "Unknown";
    }
}

inline ROMJoint ROMJointFromString(const QString& str) {
    if (str == "NeckFlexion") return ROMJoint::NeckFlexion;
    if (str == "NeckExtension") return ROMJoint::NeckExtension;
    if (str == "NeckRotation") return ROMJoint::NeckRotation;
    if (str == "NeckLateralFlexion") return ROMJoint::NeckLateralFlexion;
    if (str == "ShoulderFlexion") return ROMJoint::ShoulderFlexion;
    if (str == "ShoulderExtension") return ROMJoint::ShoulderExtension;
    if (str == "ShoulderAbduction") return ROMJoint::ShoulderAbduction;
    if (str == "ShoulderAdduction") return ROMJoint::ShoulderAdduction;
    if (str == "ShoulderInternalRotation") return ROMJoint::ShoulderInternalRotation;
    if (str == "ShoulderExternalRotation") return ROMJoint::ShoulderExternalRotation;
    if (str == "ElbowFlexion") return ROMJoint::ElbowFlexion;
    if (str == "ElbowExtension") return ROMJoint::ElbowExtension;
    if (str == "ForearmSupination") return ROMJoint::ForearmSupination;
    if (str == "ForearmPronation") return ROMJoint::ForearmPronation;
    if (str == "WristFlexion") return ROMJoint::WristFlexion;
    if (str == "WristExtension") return ROMJoint::WristExtension;
    if (str == "WristRadialDeviation") return ROMJoint::WristRadialDeviation;
    if (str == "WristUlnarDeviation") return ROMJoint::WristUlnarDeviation;
    if (str == "FingerFlexion") return ROMJoint::FingerFlexion;
    if (str == "ThumbOpposition") return ROMJoint::ThumbOpposition;
    if (str == "ThoracicRotation") return ROMJoint::ThoracicRotation;
    if (str == "ThoracicLateralFlexion") return ROMJoint::ThoracicLateralFlexion;
    if (str == "ThoracicExtension") return ROMJoint::ThoracicExtension;
    if (str == "LumbarFlexion") return ROMJoint::LumbarFlexion;
    if (str == "LumbarExtension") return ROMJoint::LumbarExtension;
    if (str == "LumbarLateralFlexion") return ROMJoint::LumbarLateralFlexion;
    if (str == "LumbarRotation") return ROMJoint::LumbarRotation;
    if (str == "HipFlexion") return ROMJoint::HipFlexion;
    if (str == "HipExtension") return ROMJoint::HipExtension;
    if (str == "HipAbduction") return ROMJoint::HipAbduction;
    if (str == "HipAdduction") return ROMJoint::HipAdduction;
    if (str == "HipInternalRotation") return ROMJoint::HipInternalRotation;
    if (str == "HipExternalRotation") return ROMJoint::HipExternalRotation;
    if (str == "KneeFlexion") return ROMJoint::KneeFlexion;
    if (str == "KneeExtension") return ROMJoint::KneeExtension;
    if (str == "AnkleDorsiflexion") return ROMJoint::AnkleDorsiflexion;
    if (str == "AnklePlantarflexion") return ROMJoint::AnklePlantarflexion;
    if (str == "FootInversion") return ROMJoint::FootInversion;
    if (str == "FootEversion") return ROMJoint::FootEversion;
    if (str == "ToeFlexion") return ROMJoint::ToeFlexion;
    if (str == "ToeExtension") return ROMJoint::ToeExtension;
    return ROMJoint::Unknown;
}



#endif // ROMFIELDS_H
