#ifndef WORKOUTFIELDENUM_H
#define WORKOUTFIELDENUM_H

#include <QString>
#include <QHash>

/**
 * @file WorkoutFieldEnum.h
 * @brief Define los campos asociados a una entidad de tipo entrenamiento (Workout) y funciones auxiliares para su conversión.
 *
 * Este archivo declara el enumerado `WorkoutField`, que especifica los distintos atributos que describen un entrenamiento dentro del sistema.
 * También proporciona funciones auxiliares para convertir entre los valores del enum y su representación textual,
 * útil para persistencia, edición dinámica y generación de formularios en la interfaz de usuario.
 */

/// Enum que representa los distintos campos disponibles para describir un entrenamiento o plan de ejercicio.
enum class WorkoutField {
    IdWorkout,     ///< Identificador único del entrenamiento.
    Name,          ///< Nombre del entrenamiento.
    Description,   ///< Descripción general del entrenamiento.
    duration,      ///< Duración total estimada del entrenamiento (en minutos o segundos, según configuración).
    Unknown        ///< Campo desconocido o no mapeado.
};

/**
 * @brief Convierte un valor de `WorkoutField` en una cadena de texto representativa.
 *
 * Esta función es útil para mapear campos del modelo a nombres de columnas en bases de datos o claves de formularios.
 *
 * @param field Campo de tipo `WorkoutField`.
 * @return Cadena de texto correspondiente al campo.
 */
inline QString WorkoutFieldToString(WorkoutField field) {
    switch (field) {

    case WorkoutField::IdWorkout: return "IdWorkout";
    case WorkoutField::Name: return "Name";
    case WorkoutField::Description: return "Description";
    case WorkoutField::duration: return "duration";
    default: return "Unknown";
    }
}


/**
 * @brief Convierte una cadena de texto en el valor correspondiente del enumerado `WorkoutField`.
 *
 * La conversión es insensible a mayúsculas/minúsculas.
 *
 * @param str Cadena con el nombre del campo.
 * @return Valor del enumerado `WorkoutField` correspondiente, o `WorkoutField::Unknown` si no coincide.
 */
inline WorkoutField WorkoutFieldFromString(const QString& str) {
    QString s = str.toLower();
    if (s == "idworkout") return WorkoutField::IdWorkout;
    if (s == "name") return WorkoutField::Name;
    if (s == "description") return WorkoutField::Description;
    if (s == "duration") return WorkoutField::duration;



    return WorkoutField::Unknown;
}

/**
 * @brief Generador de hash personalizado para `WorkoutField`, utilizado para estructuras `QHash`.
 * @param key Clave del tipo `WorkoutField`.
 * @param seed Valor de semilla inicial.
 * @return Valor hash calculado.
 */
inline uint qHash(const WorkoutField& key, uint seed = 0) {
    return ::qHash(static_cast<int>(key), seed);
}

#endif // WORKOUTFIELDENUM_H
