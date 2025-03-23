#ifndef PROFILERECORDS_H
#define PROFILERECORDS_H
#include <QString>
#include <QMap>

// Enum que representa métricas físicas del cliente
enum class BodyRecord {
    FatPercentage,
    MuscleMass,
    BMI,
    HeartRate,
    VO2Max
};

// Convierte BodyMetric → QString
inline QString toString(BodyRecord metric) {
    switch (metric) {
    case BodyRecord::FatPercentage: return "fatPercentage";
    case BodyRecord::MuscleMass:    return "muscleMass";
    case BodyRecord::BMI:           return "BMI";
    case BodyRecord::HeartRate:     return "heartRate";
    case BodyRecord::VO2Max:        return "VO2Max";
    }
    return "Unknown";
}

// Convierte QString → BodyMetric
inline BodyRecord fromString(const QString& key) {

    static const QMap<QString, BodyRecord> map = {
        { "fatPercentage", BodyRecord::FatPercentage },
        { "muscleMass",    BodyRecord::MuscleMass },
        { "BMI",           BodyRecord::BMI },
        { "heartRate",     BodyRecord::HeartRate },
        { "VO2Max",        BodyRecord::VO2Max }
    };

    return map.value(key, BodyRecord::FatPercentage);  // valor por defecto si no se encuentra
}



#endif // PROFILERECORDS_H
