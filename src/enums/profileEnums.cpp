#include "profileEnums.h"

 QString ClientProfileDataToString(ClientProfileData field) {
    switch (field) {
    case ClientProfileData::General: return "General";
    case ClientProfileData::Functional: return "Functional";
    case ClientProfileData::Morfological: return "Morfological";
    case ClientProfileData::Medical: return "Medical";
    case ClientProfileData::LifeStyle: return "LifeStyle";
    case ClientProfileData::Additional: return "Additional";
    case ClientProfileData::ROM: return "ROM";
    default: return "All";
    }
}

 ClientProfileData ClientProfileDataFromString(const QString& str) {

    QString s = str.toLower();

    if (s == "general") return ClientProfileData::General;
    if (s == "functional") return ClientProfileData::Functional;
    if (s == "morfological") return ClientProfileData::Morfological;
    if (s == "medical") return ClientProfileData::Medical;
    if (s == "lifestyle") return ClientProfileData::LifeStyle;
    if (s == "additional") return ClientProfileData::Additional;
    if (s == "rom") return ClientProfileData::ROM;
    return ClientProfileData::All;
}


 QString GeneralDataFieldToString(GeneralDataField field) {
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

 GeneralDataField GeneralDataFieldFromString(const QString& str) {
    QString s = str.toLower();

    if (s == "qualification") return GeneralDataField::Qualification;
    if (s == "level") return GeneralDataField::Level;
    if (s == "language") return GeneralDataField::Language;
    if (s == "notifications") return GeneralDataField::Notifications;
    if (s == "units") return GeneralDataField::Units;
    if (s == "age") return GeneralDataField::Age;
    if (s == "gender") return GeneralDataField::Gender;
    if (s == "weight") return GeneralDataField::Weight;
    if (s == "height") return GeneralDataField::Height;
    if (s == "plan") return GeneralDataField::Plan;

    return GeneralDataField::Unknown;
}

 QPair<double, double> getGeneralDataFieldLimit(GeneralDataField field) {
    QHash<GeneralDataField, QPair<double, double>> limits = {
        {GeneralDataField::Age, {5, 120}},
        {GeneralDataField::Weight, {30.0, 170.0}},
        {GeneralDataField::Height, {100.0, 250.0}}

    };

    return limits.value(field, QPair<double, double>{-1, -1});
}

  QString FunctionalFieldToString(FunctionalField field) {
     switch (field) {
     case FunctionalField::StaticPosture: return "StaticPosture";
     case FunctionalField::HipMobility: return "HipMobility";
     case FunctionalField::AnkleMobility: return "AnkleMobility";
     case FunctionalField::SquatStrength: return "SquatStrength";
     case FunctionalField::BalanceTest: return "BalanceTest";
     case FunctionalField::ShoulderMobilityLevel: return "ShoulderMobilityLevel";
     case FunctionalField::CardioLevel: return "CardioLevel";
     default: return "Unknown";
     }
 }

  FunctionalField FunctionalFieldFromString(const QString& str) {
     QString s = str.toLower();
     if (s == "staticposture") return FunctionalField::StaticPosture;
     if (s == "hipmobility") return FunctionalField::HipMobility;
     if (s == "anklemobility") return FunctionalField::AnkleMobility;
     if (s == "squatstrength") return FunctionalField::SquatStrength;
     if (s == "balancetest") return FunctionalField::BalanceTest;
     if (s == "shouldermobilitylevel") return FunctionalField::ShoulderMobilityLevel;
     if (str == "cardiolevel") return FunctionalField::CardioLevel;
     return FunctionalField::Unknown;
 }

  QPair<double, double> getFunctionalFieldLimit(FunctionalField field) {
     QHash<FunctionalField, QPair<double, double>> limits = {
         {FunctionalField::HipMobility, {0, 10}},
         {FunctionalField::AnkleMobility, {0, 10}},
         {FunctionalField::ShoulderMobilityLevel, {0, 10}},
         {FunctionalField::SquatStrength, {0, 1000}},
         {FunctionalField::BalanceTest, {0, 100}},
         {FunctionalField::CardioLevel, {0, 10}}
         // StaticPosture no tiene sentido numérico
     };

     return limits.value(field, QPair<double, double>{-1, -1});
 }

   QString LifeStyleFieldsToString(LifeStyleFields field) {
      switch (field) {
      case LifeStyleFields::TrainingFrequency: return "TrainingFrequency";
      case LifeStyleFields::TypeOfTraining: return "TypeofTraining";
      case LifeStyleFields::PreviousSports: return "PreviousSports";
      case LifeStyleFields::SmokingHabits: return "SmokingHabits";
      default: return "Unknown";
      }
  }

   LifeStyleFields LifeStyleFieldsFromString(const QString& str) {
      QString s = str.toLower();

      if (s == "trainingfrequency") return LifeStyleFields::TrainingFrequency;
      if (s == "typeoftraining")    return LifeStyleFields::TypeOfTraining;
      if (s == "previoussports")    return LifeStyleFields::PreviousSports;
      if (s == "smokinghabits")     return LifeStyleFields::SmokingHabits;

      return LifeStyleFields::Unknown;
  }


   QPair<double, double> getLifeStyleFieldsLimit(LifeStyleFields field) {
      QHash<LifeStyleFields, QPair<double, double>> limits = {
      };

      return limits.value(field, QPair<double, double>{-1, -1});
  }

    QString MedicalRecordsFieldsToString(MedicalRecordsFields field) {
       switch (field) {
       case MedicalRecordsFields::PreviousInjuries: return "PreviousInjuries";
       case MedicalRecordsFields::MedicalLimitations: return "MedicalLimitations";
       case MedicalRecordsFields::PainZones: return "PainZones";

       default: return "Unknown";
       }
   }

    MedicalRecordsFields MedicalRecordsFieldsFromString(const QString& str) {
       QString s=str.toLower();
       if (s == "previousinjuries") return MedicalRecordsFields::PreviousInjuries;
       if (s == "medicallimitations") return MedicalRecordsFields::MedicalLimitations;
       if (s == "painzones") return MedicalRecordsFields::PainZones;
       return MedicalRecordsFields::Unknown;
   }


    QPair<double, double> getMedicalRecordsFieldsLimit(MedicalRecordsFields field) {

       QHash<MedicalRecordsFields, QPair<double, double>> limits = {
       };

       return limits.value(field, QPair<double, double>{-1, -1});
   }

     QString MorfologicalFieldsToString(MorfologicalFields field) {
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

     MorfologicalFields MorfologicalFieldsFromString(const QString& str) {
        QString s = str.toLower();

        if (s == "fatpercentage")        return MorfologicalFields::FatPercentage;
        if (s == "musclemass")           return MorfologicalFields::MuscleMass;
        if (s == "bmi")         return MorfologicalFields::BMI;
        if (s == "vo2max")               return MorfologicalFields::VO2Max;
        if (s == "heartrate")            return MorfologicalFields::HeartRate;
        if (s == "torsolength")      return MorfologicalFields::TorsoLength;
        if (s == "leglength")            return MorfologicalFields::LegLength;
        if (s == "armlength")            return MorfologicalFields::ArmLength;
        if (s == "forearmlength")      return MorfologicalFields::ForearmLength;
        if (s == "thighcircumference")   return MorfologicalFields::ThighCircumference;
        if (s == "waistcircumference")   return MorfologicalFields::WaistCircumference;

        return MorfologicalFields::Unknown;
    }
     QPair<double, double> getMorfologicalLimit(MorfologicalFields field) {

        QHash<MorfologicalFields, QPair<double, double>> limits = {
                                                                   {MorfologicalFields::FatPercentage, {0.0, 70.0}},
                                                                   {MorfologicalFields::MuscleMass, {0.0, 100.0}},
                                                                   {MorfologicalFields::BMI, {10.0, 60.0}},
                                                                   {MorfologicalFields::VO2Max, {0.0, 90.0}},
                                                                   {MorfologicalFields::HeartRate, {30.0, 220.0}},
                                                                   {MorfologicalFields::TorsoLength, {30.0, 80.0}},
                                                                   {MorfologicalFields::LegLength, {30.0, 120.0}},
                                                                   {MorfologicalFields::ArmLength, {20.0, 100.0}},
                                                                   {MorfologicalFields::ForearmLength, {15.0, 50.0}},
                                                                   {MorfologicalFields::ThighCircumference, {20.0, 100.0}},
                                                                   {MorfologicalFields::WaistCircumference, {40.0, 150.0}},
                                                                   };
        return limits.value(field, QPair<double, double>{-1, -1});
    }

      QString ROMJointToString(ROMJoint field) {
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

      ROMJoint ROMJointFromString(const QString& str) {
         QString s = str.toLower();

         if (s == "neckflexion") return ROMJoint::NeckFlexion;
         if (s == "neckextension") return ROMJoint::NeckExtension;
         if (s == "neckrotation") return ROMJoint::NeckRotation;
         if (s == "necklateralflexion") return ROMJoint::NeckLateralFlexion;

         if (s == "shoulderflexion") return ROMJoint::ShoulderFlexion;
         if (s == "shoulderextension") return ROMJoint::ShoulderExtension;
         if (s == "shoulderabduction") return ROMJoint::ShoulderAbduction;
         if (s == "shoulderadduction") return ROMJoint::ShoulderAdduction;
         if (s == "shoulderinternalrotation") return ROMJoint::ShoulderInternalRotation;
         if (s == "shoulderexternalrotation") return ROMJoint::ShoulderExternalRotation;

         if (s == "elbowflexion") return ROMJoint::ElbowFlexion;
         if (s == "elbowextension") return ROMJoint::ElbowExtension;

         if (s == "forearmsupination") return ROMJoint::ForearmSupination;
         if (s == "forearmpronation") return ROMJoint::ForearmPronation;

         if (s == "wristflexion") return ROMJoint::WristFlexion;
         if (s == "wristextension") return ROMJoint::WristExtension;
         if (s == "wristradialdeviation") return ROMJoint::WristRadialDeviation;
         if (s == "wristulnardeviation") return ROMJoint::WristUlnarDeviation;

         if (s == "fingerflexion") return ROMJoint::FingerFlexion;
         if (s == "thumbopposition") return ROMJoint::ThumbOpposition;

         if (s == "thoracicrotation") return ROMJoint::ThoracicRotation;
         if (s == "thoraciclateralflexion") return ROMJoint::ThoracicLateralFlexion;
         if (s == "thoracicextension") return ROMJoint::ThoracicExtension;

         if (s == "lumbarflexion") return ROMJoint::LumbarFlexion;
         if (s == "lumbarextension") return ROMJoint::LumbarExtension;
         if (s == "lumbarlateralflexion") return ROMJoint::LumbarLateralFlexion;
         if (s == "lumbarrotation") return ROMJoint::LumbarRotation;

         if (s == "hipflexion") return ROMJoint::HipFlexion;
         if (s == "hipextension") return ROMJoint::HipExtension;
         if (s == "hipabduction") return ROMJoint::HipAbduction;
         if (s == "hipadduction") return ROMJoint::HipAdduction;
         if (s == "hipinternalrotation") return ROMJoint::HipInternalRotation;
         if (s == "hipexternalrotation") return ROMJoint::HipExternalRotation;

         if (s == "kneeflexion") return ROMJoint::KneeFlexion;
         if (s == "kneeextension") return ROMJoint::KneeExtension;

         if (s == "ankledorsiflexion") return ROMJoint::AnkleDorsiflexion;
         if (s == "ankleplantarflexion") return ROMJoint::AnklePlantarflexion;

         if (s == "footinversion") return ROMJoint::FootInversion;
         if (s == "footeversion") return ROMJoint::FootEversion;

         if (s == "toeflexion") return ROMJoint::ToeFlexion;
         if (s == "toeextension") return ROMJoint::ToeExtension;

         return ROMJoint::Unknown;
     }



      QPair<double, double>  getROMLimit(ROMJoint field) {

         QHash<ROMJoint, QPair<double, double>> limits = {
                                                          {ROMJoint::NeckFlexion, {0, 80}},
                                                          {ROMJoint::NeckExtension, {0, 70}},
                                                          {ROMJoint::NeckRotation, {0, 90}},
                                                          {ROMJoint::NeckLateralFlexion, {0, 45}},
                                                          {ROMJoint::ShoulderFlexion, {0, 180}},
                                                          {ROMJoint::ShoulderExtension, {0, 60}},
                                                          {ROMJoint::ShoulderAbduction, {0, 180}},
                                                          {ROMJoint::ShoulderAdduction, {0, 50}},
                                                          {ROMJoint::ShoulderInternalRotation, {0, 90}},
                                                          {ROMJoint::ShoulderExternalRotation, {0, 90}},
                                                          {ROMJoint::ElbowFlexion, {0, 150}},
                                                          {ROMJoint::ElbowExtension, {0, 0}},
                                                          {ROMJoint::ForearmSupination, {0, 90}},
                                                          {ROMJoint::ForearmPronation, {0, 90}},
                                                          {ROMJoint::WristFlexion, {0, 80}},
                                                          {ROMJoint::WristExtension, {0, 70}},
                                                          {ROMJoint::WristRadialDeviation, {0, 20}},
                                                          {ROMJoint::WristUlnarDeviation, {0, 30}},
                                                          {ROMJoint::FingerFlexion, {0, 90}},
                                                          {ROMJoint::ThumbOpposition, {0, 60}},
                                                          {ROMJoint::ThoracicRotation, {0, 35}},
                                                          {ROMJoint::ThoracicLateralFlexion, {0, 30}},
                                                          {ROMJoint::ThoracicExtension, {0, 25}},
                                                          {ROMJoint::LumbarFlexion, {0, 60}},
                                                          {ROMJoint::LumbarExtension, {0, 25}},
                                                          {ROMJoint::LumbarLateralFlexion, {0, 25}},
                                                          {ROMJoint::LumbarRotation, {0, 20}},
                                                          {ROMJoint::HipFlexion, {0, 120}},
                                                          {ROMJoint::HipExtension, {0, 30}},
                                                          {ROMJoint::HipAbduction, {0, 45}},
                                                          {ROMJoint::HipAdduction, {0, 30}},
                                                          {ROMJoint::HipInternalRotation, {0, 45}},
                                                          {ROMJoint::HipExternalRotation, {0, 45}},
                                                          {ROMJoint::KneeFlexion, {0, 135}},
                                                          {ROMJoint::KneeExtension, {0, 0}},
                                                          {ROMJoint::AnkleDorsiflexion, {0, 20}},
                                                          {ROMJoint::AnklePlantarflexion, {0, 50}},
                                                          {ROMJoint::FootInversion, {0, 35}},
                                                          {ROMJoint::FootEversion, {0, 15}},
                                                          {ROMJoint::ToeFlexion, {0, 60}},
                                                          {ROMJoint::ToeExtension, {0, 60}},
                                                          };
         return limits.value(field, QPair<double, double>{-1, -1});
     }

     QStringList GeneralDataFieldNames() {
         QStringList list;
         for (int i = static_cast<int>(GeneralDataField::Qualification); i <= static_cast<int>(GeneralDataField::Plan); ++i)
             list << GeneralDataFieldToString(static_cast<GeneralDataField>(i));
         return list;
     }

     QStringList FunctionalFieldNames() {
         QStringList list;
         for (int i = static_cast<int>(FunctionalField::StaticPosture); i <= static_cast<int>(FunctionalField::CardioLevel); ++i)
             list << FunctionalFieldToString(static_cast<FunctionalField>(i));
         return list;
     }

     QStringList MorfologicalFieldNames() {
         QStringList list;
         for (int i = static_cast<int>(MorfologicalFields::FatPercentage); i <= static_cast<int>(MorfologicalFields::WaistCircumference); ++i)
             list << MorfologicalFieldsToString(static_cast<MorfologicalFields>(i));
         return list;
     }

     QStringList ROMJointNames() {
         QStringList list;
         for (int i = static_cast<int>(ROMJoint::NeckFlexion); i <= static_cast<int>(ROMJoint::ToeExtension); ++i)
             list << ROMJointToString(static_cast<ROMJoint>(i));
         return list;
     }

     QStringList MedicalRecordsFieldNames() {
         QStringList list;
         for (int i = static_cast<int>(MedicalRecordsFields::PreviousInjuries); i <= static_cast<int>(MedicalRecordsFields::PainZones); ++i)
             list << MedicalRecordsFieldsToString(static_cast<MedicalRecordsFields>(i));
         return list;
     }

     QStringList LifeStyleFieldNames() {
         QStringList list;
         for (int i = static_cast<int>(LifeStyleFields::TrainingFrequency); i <= static_cast<int>(LifeStyleFields::SmokingHabits); ++i)
             list << LifeStyleFieldsToString(static_cast<LifeStyleFields>(i));
         return list;
     }
