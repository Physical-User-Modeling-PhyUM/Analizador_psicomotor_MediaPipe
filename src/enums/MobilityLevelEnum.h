#ifndef MOBILITYLEVEL_H
#define MOBILITYLEVEL_H

#include <QString>

enum class MobilityLevel {
    Limited,
    Average,
    Good,
    Unknown
};

inline QString MobilityLevelToString(MobilityLevel field) {
    switch (field) {
    case MobilityLevel::Limited: return "Limited";
    case MobilityLevel::Average: return "Average";
    case MobilityLevel::Good: return "Good";
    default: return "Unknown";
    }
}

inline MobilityLevel MobilityLevelFromString(const QString& str) {
    if (str == "Limited") return MobilityLevel::Limited;
    if (str == "Average") return MobilityLevel::Average;
    if (str == "Good") return MobilityLevel::Good;
    return MobilityLevel::Unknown;
}

#endif // MOBILITYLEVEL_H
