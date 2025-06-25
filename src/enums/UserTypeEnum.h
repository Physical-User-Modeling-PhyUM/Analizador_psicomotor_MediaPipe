#ifndef USERTYPE_H
#define USERTYPE_H

#include <QString>

/**
 * @file UserTypeEnum.h
 * @brief Define el tipo de usuario dentro del sistema y proporciona funciones auxiliares para su conversión.
 *
 * Este archivo contiene la definición del enumerado `UserType`, que clasifica a los usuarios según su rol en la plataforma:
 * administrador, cliente o entrenador físico. También incluye funciones auxiliares para la conversión entre representación
 * enumerada y cadena de texto, lo que facilita su uso en bases de datos, formularios o interfaces gráficas.
 */

/// Enum que representa los distintos roles o tipos de usuario en el sistema.
enum class UserType {
    Admin,     ///< Usuario con permisos de administración global del sistema.
    Client,    ///< Usuario cliente que recibe entrenamiento o seguimiento.
    Trainer,   ///< Usuario entrenador físico que diseña y supervisa planes.
    Unknown    ///< Tipo de usuario no reconocido o sin asignar.
};

/**
 * @brief Convierte un valor del enumerado `UserType` a una cadena legible.
 *
 * Esta función se utiliza principalmente para mostrar el tipo de usuario en la interfaz o al mapearlo en la base de datos.
 *
 * @param field Tipo de usuario como enumerado.
 * @return Cadena representativa del tipo de usuario.
 */
inline QString UserTypeToString(UserType field) {
    switch (field) {
    case UserType::Admin: return "Admin";
    case UserType::Client: return "Client";
    case UserType::Trainer: return "Trainer";
    default: return "Unknown";
    }
}
/**
 * @brief Convierte una cadena en el valor correspondiente del enumerado `UserType`.
 *
 * Esta función acepta una cadena en minúsculas (case-insensitive) y devuelve el valor correspondiente del tipo de usuario.
 *
 * @param str Cadena con el nombre del tipo de usuario.
 * @return Valor de `UserType` equivalente.
 */
inline UserType UserTypeFromString(const QString& str) {
    QString s = str.toLower();
    if (s == "admin") return UserType::Admin;
    if (s == "client") return UserType::Client;
    if (s == "trainer") return UserType::Trainer;
    return UserType::Unknown;
}

#endif // USERTYPE_H
