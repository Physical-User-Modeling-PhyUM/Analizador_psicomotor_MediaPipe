/**
 * @file PreferencesFieldEnum.h
 * @brief Enumerado que representa los campos configurables en las preferencias del usuario.
 *
 * Define las claves de configuración relacionadas con la personalización de la experiencia del usuario,
 * tales como idioma, feedback visual y auditivo.
 * Incluye funciones auxiliares para conversión entre el enum y su representación textual.
 */

#ifndef PREFERENCESFIELD_H
#define PREFERENCESFIELD_H

#include <QString>

/**
 * @enum PreferencesField
 * @brief Representa los distintos campos configurables en las preferencias de un usuario.
 *
 * Se utiliza en objetos como `UserPreferences` para identificar y manipular ajustes personalizados.
 */
enum class PreferencesField {
    Language,         ///< Idioma de la aplicación
    VisualFeedback,   ///< Activación de feedback visual
    AudioFeedback,    ///< Activación de feedback auditivo
    Unknown           ///< Campo desconocido o no definido
};

/**
 * @brief Convierte un valor `PreferencesField` a una cadena legible.
 * @param field Campo del enum a convertir.
 * @return Nombre del campo como cadena.
 */
inline QString PreferencesFieldToString(PreferencesField field) {
    switch (field) {
    case PreferencesField::Language: return "Language";
    case PreferencesField::VisualFeedback: return "VisualFeedback";
    case PreferencesField::AudioFeedback: return "AudioFeedback";
    default: return "Unknown";
    }
}

/**
 * @brief Convierte una cadena en un valor del enum `PreferencesField`.
 * @param str Cadena representando el nombre del campo.
 * @return Valor correspondiente del enum, o `Unknown` si no se reconoce.
 */
inline PreferencesField PreferencesFieldFromString(const QString& str) {
    if (str == "Language") return PreferencesField::Language;
    if (str == "VisualFeedback") return PreferencesField::VisualFeedback;
    if (str == "AudioFeedback") return PreferencesField::AudioFeedback;
    return PreferencesField::Unknown;
}

#endif // PREFERENCESFIELD_H
