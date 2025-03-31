#ifndef FEEDBACKMODE_H
#define FEEDBACKMODE_H

#include <QString>

enum class FeedbackMode {
    Visual,
    Audio,
    Both,
    None,
    Unknown
};

inline QString FeedbackModeToString(FeedbackMode field) {
    switch (field) {
    case FeedbackMode::Visual: return "Visual";
    case FeedbackMode::Audio: return "Audio";
    case FeedbackMode::Both: return "Both";
    case FeedbackMode::None: return "None";
    default: return "Unknown";
    }
}

inline FeedbackMode FeedbackModeFromString(const QString& str) {
    if (str == "Visual") return FeedbackMode::Visual;
    if (str == "Audio") return FeedbackMode::Audio;
    if (str == "Both") return FeedbackMode::Both;
    if (str == "None") return FeedbackMode::None;
    return FeedbackMode::Unknown;
}

#endif // FEEDBACKMODE_H
