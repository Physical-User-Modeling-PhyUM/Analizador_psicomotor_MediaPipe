/**
 * @file MobilityLevelEnum.h
 * @brief Enumerado que define los niveles de movilidad física de un usuario.
 *
 * Este archivo proporciona un enum `MobilityLevel` para representar la capacidad de movilidad
 * en perfiles de usuario, junto con funciones auxiliares para su conversión desde y hacia cadenas de texto.
 */

#ifndef MOBILITYLEVEL_H
#define MOBILITYLEVEL_H

#include <QString>

/**
 * @enum MobilityLevel
 * @brief Representa el nivel de movilidad de un usuario.
 *
 * Este enumerado se utiliza en los perfiles funcionales de los usuarios
 * para clasificar su capacidad física general.
 */
enum class MobilityLevel {
    Limited,    ///< Movilidad limitada
    Average,    ///< Movilidad media o funcional
    Good,       ///< Movilidad completa o sin restricciones
    Unknown     ///< No especificado
};

/**
 * @brief Convierte un valor de `MobilityLevel` a una cadena legible.
 * @param field Valor del enum a convertir.
 * @return Cadena representando el nivel de movilidad.
 */
inline QString MobilityLevelToString(MobilityLevel field) {
    switch (field) {
    case MobilityLevel::Limited: return "Limited";
    case MobilityLevel::Average: return "Average";
    case MobilityLevel::Good: return "Good";
    default: return "Unknown";
    }
}

/**
 * @brief Convierte una cadena a un valor del enumerado `MobilityLevel`.
 * @param str Cadena a convertir (no distingue mayúsculas/minúsculas).
 * @return Valor del enum correspondiente, o `Unknown` si no coincide.
 */
inline MobilityLevel MobilityLevelFromString(const QString& str) {
    QString s = str.toLower();
    if (s == "limited") return MobilityLevel::Limited;
    if (s == "average") return MobilityLevel::Average;
    if (s == "good") return MobilityLevel::Good;
    return MobilityLevel::Unknown;
}

#endif // MOBILITYLEVEL_H
