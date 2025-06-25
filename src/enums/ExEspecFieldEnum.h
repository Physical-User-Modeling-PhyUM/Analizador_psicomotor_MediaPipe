/**
 * @file ExEspecFieldEnum.h
 * @brief Enumerado que define los campos de una especificación de ejercicio.
 *
 * Este enumerado se utiliza para abstraer el acceso a los atributos de una clase `ExerciseEspec`,
 * permitiendo validar, editar o mostrar dinámicamente los campos asociados a un ejercicio,
 * como el número de series, repeticiones, duración o grupo muscular objetivo.
 */

#ifndef EXESPECFIELDENUM_H
#define EXESPECFIELDENUM_H

#include <QString>

/**
 * @enum ExEspecField
 * @brief Enum que representa los diferentes campos editables en una especificación de ejercicio.
 *
 * Este enum facilita operaciones como validación, serialización, UI dinámica, y acceso por nombre.
 */
enum class ExEspecField {
    IdEx,           ///< Identificador único del ejercicio
    Name,           ///< Nombre del ejercicio
    Description,    ///< Descripción textual del ejercicio
    ExersiseType,   ///< Tipo de ejercicio (fuerza, movilidad, etc.)
    TargetMuscle,   ///< Músculo objetivo principal
    Equipment,      ///< Equipamiento requerido
    Series,         ///< Número de series
    Repetitions,    ///< Repeticiones por serie
    Duration,       ///< Duración del ejercicio en segundos
    Weight,         ///< Peso o carga asociada
    RestTime,       ///< Tiempo de descanso entre series

    Unknown         ///< Valor por defecto para campos no reconocidos
};

/**
 * @brief Convierte un valor de ExEspecField a su representación en cadena.
 * @param field Campo a convertir.
 * @return Nombre del campo como QString.
 */
inline QString ExEspecFieldToString(ExEspecField field) {
    switch (field) {
    case ExEspecField::IdEx: return "IdEx";
    case ExEspecField::Name: return "Name";
    case ExEspecField::Description: return "Description";
    case ExEspecField::ExersiseType: return "ExersiseType";
    case ExEspecField::TargetMuscle: return "TargetMuscle";
    case ExEspecField::Equipment: return "Equipment";
    case ExEspecField::Series: return "Series";
    case ExEspecField::Repetitions: return "Repetitions";
    case ExEspecField::Duration: return "Duration";
    case ExEspecField::Weight: return "Weight";
    case ExEspecField::RestTime: return "RestTime";
    default: return "Unknown";
    }
}

/**
 * @brief Convierte una cadena en minúsculas al campo ExEspecField correspondiente.
 * @param str Cadena con el nombre del campo.
 * @return Valor correspondiente del enumerado ExEspecField.
 */
inline ExEspecField ExEspecFieldFromString(const QString& str) {
    QString s = str.toLower();

    if (s == "idex") return ExEspecField::IdEx;
    if (s == "name") return ExEspecField::Name;
    if (s == "description") return ExEspecField::Description;
    if (s == "exersisetype") return ExEspecField::ExersiseType;
    if (s == "targetmuscle") return ExEspecField::TargetMuscle;
    if (s == "equipment") return ExEspecField::Equipment;
    if (s == "series") return ExEspecField::Series;
    if (s == "repetitions") return ExEspecField::Repetitions;
    if (s == "duration") return ExEspecField::Duration;
    if (s == "weight") return ExEspecField::Weight;
    if (s == "resttime") return ExEspecField::RestTime;

    return ExEspecField::Unknown;
}

#endif // EXESPECFIELDENUM_H
