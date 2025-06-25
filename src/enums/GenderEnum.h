/**
 * @file GenderEnum.h
 * @brief Enumerado que representa el género de un usuario.
 *
 * Este archivo define un enum `Gender` con valores posibles para el género biológico,
 * así como funciones auxiliares para convertir entre `Gender` y `QString`.
 * Se utiliza principalmente en perfiles de usuario o formularios relacionados.
 */

#ifndef GENDER_H
#define GENDER_H

#include <QString>

/**
 * @enum Gender
 * @brief Enumerado para representar el género de una persona.
 *
 * Proporciona una abstracción sobre posibles géneros registrados en el sistema:
 * - Male: masculino
 * - Female: femenino
 * - Unknown: sin especificar
 */
enum class Gender {
    Male,       ///< Género masculino
    Female,     ///< Género femenino
    Unknown     ///< No especificado o no binario
};

/**
 * @brief Convierte un valor del enum Gender a una cadena de texto.
 * @param field Valor del enum Gender.
 * @return Representación en QString del valor.
 */
inline QString GenderToString(Gender field) {
    switch (field) {
    case Gender::Male: return "Male";
    case Gender::Female: return "Female";
    default: return "Unknown";
    }
}

/**
 * @brief Convierte una cadena a un valor del enum Gender.
 * @param str Cadena que representa el género ("Male", "Female", etc.).
 * @return Valor correspondiente del enum Gender.
 */
inline Gender GenderFromString(const QString& str) {
    if (str == "Male") return Gender::Male;
    if (str == "Female") return Gender::Female;
    return Gender::Unknown;
}

#endif // GENDER_H
