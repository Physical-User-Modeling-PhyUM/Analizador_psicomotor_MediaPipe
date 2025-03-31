#ifndef PREFERENCESFIELD_H
#define PREFERENCESFIELD_H

#include <QString>

enum class PreferencesField {
    Language,
    VisualFeedback,
    AudioFeedback,
    Unknown
};

inline QString PreferencesFieldToString(PreferencesField field) {
    switch (field) {
    case PreferencesField::Language: return "Language";
    case PreferencesField::VisualFeedback: return "VisualFeedback";
    case PreferencesField::AudioFeedback: return "AudioFeedback";
    default: return "Unknown";
    }
}

inline PreferencesField PreferencesFieldFromString(const QString& str) {
    if (str == "Language") return PreferencesField::Language;
    if (str == "VisualFeedback") return PreferencesField::VisualFeedback;
    if (str == "AudioFeedback") return PreferencesField::AudioFeedback;
    return PreferencesField::Unknown;
}

#endif // PREFERENCESFIELD_H
