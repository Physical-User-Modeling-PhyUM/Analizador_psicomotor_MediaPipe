/*!
 * \file admin.h
 * \brief Declaración de la clase Admin.
 *
 * La clase Admin representa a un usuario con privilegios administrativos dentro del sistema.
 * Hereda de la clase base User y extiende sus funcionalidades con permisos adicionales de gestión.
 */

#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"
#include "enums/profileEnums.h"

/*!
 * \class Admin
 * \brief Clase que representa a un administrador del sistema.
 *
 * Un administrador puede tener distintos niveles de permiso y capacidades para gestionar usuarios
 * y modificar planes de entrenamiento. Esta clase extiende la funcionalidad básica de User.
 */
class Admin : public User {
public:
    /*!
     * \brief Constructor explícito del administrador.
     * \param dataMap Mapa con los campos clave-valor necesarios para inicializar un objeto Admin.
     * \param parent Objeto padre en la jerarquía Qt.
     */
    explicit Admin(QHash<UserField, QVariant> dataMap, QObject *parent = nullptr);

    /*!
     * \brief Obtiene el nivel de permisos del administrador.
     * \return Entero que representa el nivel de permisos.
     */
    int getPermissionLevel() const;

    /*!
     * \brief Establece el nivel de permisos del administrador.
     * \param newLevel Nuevo valor del nivel de permisos.
     */
    void setPermissionLevel(int newLevel);

    /*!
     * \brief Obtiene la descripción del rol del administrador.
     * \return Cadena de texto con la descripción.
     */
    QString getRoleDescription() const;

    /*!
     * \brief Establece la descripción del rol.
     * \param desc Nueva descripción del rol.
     */
    void setRoleDescription(const QString &desc);

    /*!
     * \brief Verifica si el administrador tiene permisos para gestionar usuarios.
     * \return true si puede gestionar, false en caso contrario.
     */
    bool canManage() const;

    /*!
     * \brief Establece el permiso de gestión de usuarios.
     * \param value true para permitir gestión, false para denegarla.
     */
    void setCanManage(bool value);

    /*!
     * \brief Verifica si el administrador puede modificar planes de entrenamiento.
     * \return true si puede modificar, false en caso contrario.
     */
    bool canModifyTrainingPlans() const;

    /*!
     * \brief Establece el permiso de modificación de planes de entrenamiento.
     * \param value true para permitir modificación, false para denegarla.
     */
    void setCanModifyTrainingPlans(bool value);

private:
    int permissionLevel;         //!< Nivel de permisos del administrador.
    QString roleDescription;     //!< Descripción del rol asignado.
    bool canManageUsers;         //!< Indica si puede gestionar usuarios.
    bool canModifyPlans;         //!< Indica si puede modificar planes de entrenamiento.
};

#endif // ADMIN_H
