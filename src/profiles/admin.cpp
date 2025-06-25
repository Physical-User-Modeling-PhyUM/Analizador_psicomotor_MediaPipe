/*!
 * \file admin.cpp
 * \brief Implementación de la clase Admin.
 *
 * Define la lógica de inicialización y métodos de acceso a atributos específicos de usuarios administradores.
 */
#include "admin.h"
#include <QDebug>
/*!
 * \brief Constructor de la clase Admin.
 *
 * Valida y asigna los campos obligatorios como ID, nombre de usuario, correo electrónico y contraseña.
 * También inicializa los permisos específicos de administrador y la imagen de perfil si está disponible.
 *
 * \param dataMap Mapa con los valores clave de tipo UserField.
 * \param parent Objeto padre en la jerarquía de Qt.
 */
Admin::Admin(QHash<UserField, QVariant> dataMap, QObject *parent)
      : User(-1, "", "", "", parent), permissionLevel(0), canManageUsers(false), canModifyPlans(false)

{
    //Comprobamos que disponga de los campos suficientes para la clase usuario
    if (!dataMap.contains(UserField::IdUser)
        || !dataMap.contains(UserField::UserName)
        || !dataMap.contains(UserField::Email)
        || !dataMap.contains(UserField::Pass)) {
        qWarning() << "Error: Faltan campos requeridos para crear un Admin";
        return;
    }
    //editamos los campos necesarios y nos aseguramos que no se encuentran vacios
    QString userName = dataMap.value(UserField::UserName).toString().trimmed();
    QString email = dataMap.value(UserField::Email).toString().trimmed();
    QString password = dataMap.value(UserField::Pass).toString();

    if (userName.isEmpty() || email.isEmpty() || password.isEmpty()) {
        qWarning() << "Campos vacíos no permitidos";
        return;
    }

    // Comprobamos que el valor sea un valor numérico válido, si es un QString, se convertirá a cero
    int id = dataMap.value(UserField::IdUser).toInt();
    if (id < -1) {
        qWarning() << "Constructor>>ID no válido";
        return;
    }

    this->idUser = id;
    this->valid = true;
    this->userName = userName;
    this->email = email;
    this->password = password;
    this->userType=UserType::Admin;

    //resto de campos
    permissionLevel = dataMap.value(UserField::permissionLevel, 0).toInt();
    roleDescription = dataMap.value(UserField::roleDescription,"").toString();
    canManageUsers = dataMap.value(UserField::canManageUsers, false).toBool();
    canModifyPlans = dataMap.value(UserField::canModifyPlans, false).toBool();

    QVariant imageData = dataMap.value(UserField::Picture);
    QImage image;
    if (imageData.canConvert<QByteArray>()) {
        image.loadFromData(imageData.toByteArray());
    }

    if (image.isNull()) {
        qWarning() << "No se pudo cargar ninguna imagen del perfil. Usando imagen en blanco.";
    }

    this->profile_Picture = image;
}


/*!
 * \brief Getter para el nivel de permisos del administrador.
 * \return Valor entero del nivel de permisos.
 */
int Admin::getPermissionLevel() const {
    return permissionLevel;
}
/*!
 * \brief Setter para el nivel de permisos.
 * \param newLevel Nuevo valor entero.
 */
void Admin::setPermissionLevel(int newLevel) {
    permissionLevel = newLevel;
}
/*!
 * \brief Getter para la descripción del rol.
 * \return Cadena con la descripción del rol.
 */
QString Admin::getRoleDescription() const {
    return roleDescription;
}
/*!
 * \brief Setter para la descripción del rol.
 * \param desc Nuevo texto de descripción.
 */
void Admin::setRoleDescription(const QString &desc) {
    roleDescription = desc;
}
/*!
 * \brief Indica si el administrador tiene permiso para gestionar usuarios.
 * \return true si tiene permiso; false en caso contrario.
 */
bool Admin::canManage() const {
    return canManageUsers;
}
/*!
 * \brief Establece el permiso para gestionar usuarios.
 * \param value true para permitir, false para denegar.
 */
void Admin::setCanManage(bool value) {
    canManageUsers = value;
}
/*!
 * \brief Indica si el administrador puede modificar planes de entrenamiento.
 * \return true si tiene permiso; false en caso contrario.
 */
bool Admin::canModifyTrainingPlans() const {
    return canModifyPlans;
}
/*!
 * \brief Establece el permiso para modificar planes de entrenamiento.
 * \param value true para permitir, false para denegar.
 */
void Admin::setCanModifyTrainingPlans(bool value) {
    canModifyPlans = value;
}
