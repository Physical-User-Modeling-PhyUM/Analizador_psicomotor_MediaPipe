#ifndef USERFIELD_H
#define USERFIELD_H

#include <QString>
#include <QHash>

/**
 * @file UserFieldEnum.h
 * @brief Contiene la definición del enumerado UserField y funciones auxiliares para su conversión.
 *
 * Este archivo declara el enumerado `UserField`, que agrupa todos los posibles campos de datos asociados a un usuario del sistema,
 * incluyendo clientes, entrenadores físicos y administradores. Está diseñado para ser utilizado en procesos de serialización,
 * mapeo de datos desde/hacia la base de datos y en formularios de edición o visualización de perfiles.
 */

/// Enum que representa todos los posibles campos de datos asociados a un usuario del sistema.
enum class UserField {
    IdUser,              ///< Identificador único del usuario.
    IdClient,            ///< Identificador del cliente (si aplica).
    IdTrainer,           ///< Identificador del entrenador (si aplica).
    IdAdmin,             ///< Identificador del administrador (si aplica).
    Email,               ///< Correo electrónico del usuario.
    Name,                ///< Nombre del usuario.
    UserName,            ///< Nombre de usuario (alias para login).
    FamilyName,          ///< Apellido del usuario.
    Pass,                ///< Contraseña (hashed) del usuario.
    Picture,             ///< Ruta o referencia a la imagen de perfil.

    // Campos específicos de clientes
    Plan,                ///< Nombre del plan de entrenamiento asignado al cliente.
    last_login,          ///< Fecha/hora del último inicio de sesión.
    join_up_date,        ///< Fecha de alta en la plataforma.

    // Campos específicos de entrenadores físicos
    Qualification,       ///< Formación académica o certificaciones.
    specialtyArea,       ///< Área de especialización (e.g., fuerza, rehabilitación).
    experienceYears,     ///< Años de experiencia profesional.
    resume,              ///< Descripción del perfil profesional.

    // Campos específicos de administradores
    permissionLevel,     ///< Nivel de permisos del administrador.
    roleDescription,     ///< Descripción del rol administrativo.
    canManageUsers,      ///< Indicador de si puede gestionar usuarios.
    canModifyPlans,      ///< Indicador de si puede modificar planes de entrenamiento.

    Unknown              ///< Campo no reconocido o sin clasificar.
};

/**
 * @brief Convierte un valor de UserField a su representación en texto (clave de base de datos o nombre legible).
 * @param field Campo del enumerado `UserField`.
 * @return Cadena representativa del campo.
 */
inline QString UserFieldToString(UserField field) {
    switch (field) {

    case UserField::IdUser: return "IdUser";
    case UserField::IdClient: return "IdClient";
    case UserField::IdTrainer: return "IdTrainer";
    case UserField::IdAdmin: return "IdAdmin";
    case UserField::Email: return "Email";
    case UserField::Name: return "Name";
    case UserField::UserName: return "UserName";
    case UserField::FamilyName: return "FamilyName";
    case UserField::Plan: return "Plan";
    case UserField::Qualification: return "Qualification";
    case UserField::Pass: return "Password";
    case UserField::Picture: return "profile_picture";
    case UserField::last_login: return "last_login";
    case UserField::join_up_date: return "join_up_date";

    case UserField::specialtyArea: return "specialtyArea";
    case UserField::experienceYears: return "experienceYears";
    case UserField::resume: return "resume";
    case UserField::permissionLevel: return "permissionLevel";
    case UserField::roleDescription: return "roleDescription";
    case UserField::canManageUsers: return "canManageUsers";
    case UserField::canModifyPlans: return "canModifyPlans";




    default: return "Unknown";
    }
}


/**
 * @brief Convierte una cadena en el valor correspondiente del enumerado UserField.
 * @param str Cadena en formato base de datos o legible.
 * @return Valor de `UserField` equivalente.
 */
inline UserField UserFieldFromString(const QString& str) {
    QString s = str.toLower();
    if (s == "iduser") return UserField::IdUser;
    if (s == "idclient") return UserField::IdClient;
    if (s == "idtrainer") return UserField::IdTrainer;
    if (s == "idadmin") return UserField::IdAdmin;
    if (s == "email") return UserField::Email;
    if (s == "name") return UserField::Name;
    if (s == "username") return UserField::UserName;
    if (s == "familyname") return UserField::FamilyName;
    if (s == "password") return UserField::Pass;
    if (s == "profile_picture") return UserField::Picture;
    if (s == "last_login") return UserField::last_login;
    if (s == "join_up_date") return UserField::join_up_date;
    if (s == "plan") return UserField::Plan;
    if (s == "qualification") return UserField::Qualification;
    if (s == "specialtyarea") return UserField::specialtyArea;
    if (s == "experienceyears") return UserField::experienceYears;
    if (s == "resume") return UserField::resume;
    if (s == "permissionlevel") return UserField::permissionLevel;
    if (s == "roledescription") return UserField::roleDescription;
    if (s == "canmanageusers") return UserField::canManageUsers;
    if (s == "canmodifyplans") return UserField::canModifyPlans;


    return UserField::Unknown;
}


/**
 * @brief Función de hash personalizada para `UserField`, utilizada para almacenar valores en `QHash`.
 * @param key Clave del tipo `UserField`.
 * @param seed Semilla para el cálculo hash.
 * @return Valor hash asociado.
 */
inline uint qHash(const UserField& key, uint seed = 0) {
    return ::qHash(static_cast<int>(key), seed);
}


#endif // USERFIELD_H
