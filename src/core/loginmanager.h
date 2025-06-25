/**
 * @file loginmanager.h
 * @brief Declaración de la clase LoginManager para la gestión de autenticación y registro de usuarios.
 *
 * Esta clase encapsula la lógica necesaria para el inicio de sesión de usuarios, verificación de credenciales,
 * registro de nuevos clientes y cambio de contraseñas. Utiliza hashing SHA-256 para proteger las contraseñas
 * y permite trabajar conjuntamente con el módulo UserManager para acceder a los datos del usuario.
 */

#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QString>
#include <QObject>
#include "core/dbmanager.h"
#include "core/usermanager.h"
#include "profiles/user.h"
#include "repositories/userrepository.h"

/**
 * @class LoginManager
 * @brief Clase responsable de la autenticación y gestión de contraseñas de los usuarios del sistema.
 *
 * Permite autenticar usuarios por nombre de usuario o correo electrónico, validar contraseñas
 * usando hash SHA-256, registrar nuevos clientes con contraseña cifrada y permitir el cambio seguro de contraseña.
 * Emite señales que permiten notificar el resultado del proceso de autenticación o registro.
 */
class LoginManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor del LoginManager.
     * @param db Gestor de base de datos.
     * @param uManager Gestor de usuarios.
     * @param parent Objeto padre Qt (opcional).
     */
    explicit LoginManager(QSharedPointer<DBManager> db,QSharedPointer<UserManager> uManager, QObject* parent = nullptr);

    /**
     * @brief Inicia sesión para un usuario con email o nombre de usuario y contraseña.
     * @param User_Email Email o nombre de usuario.
     * @param password Contraseña en texto plano.
     * @return Puntero al usuario autenticado, o nullptr si falla.
     */
    QSharedPointer<User> login(const QString& User_Email, const QString& password);

    /**
     * @brief Registra un nuevo cliente en el sistema.
     * @param user_Name Nombre de usuario.
     * @param user_Email Correo electrónico.
     * @param password Contraseña en texto plano.
     * @return Mapa con los campos del nuevo usuario a registrar.
     */
    QHash<UserField, QVariant> registerNewClient(const QString& user_Name,const QString& user_Email, const QString& password);

    /**
     * @brief Cambia la contraseña de un usuario si se verifica la contraseña antigua.
     * @param idUser ID del usuario.
     * @param oldPassword Contraseña actual.
     * @param newPassword Nueva contraseña deseada.
     * @return true si el cambio se realizó correctamente.
     */
    bool changePassword(int idUser,const QString& oldPassword,const QString& newPassword);

    /**
     * @brief Calcula el hash SHA-256 de una contraseña.
     * @param password Contraseña en texto plano.
     * @return Hash hexadecimal como cadena.
     */
    QString hashPassword(const QString& password) const;

    /**
     * @brief Verifica si una contraseña coincide con su hash.
     * @param password Contraseña ingresada.
     * @param hashed Hash previamente almacenado.
     * @return true si coinciden.
     */
    bool verifyPassword(const QString& password, const QString& hashed) const;

private:
    QSharedPointer<DBManager> db; ///< Gestor de acceso a la base de datos.
    QSharedPointer<UserManager> userManager; ///< Gestor de lógica relacionada con usuarios.

signals:
    /**
     * @brief Señal emitida cuando el login es exitoso.
     * @param user Usuario autenticado.
     */
    void loginSuccess(QSharedPointer<User> user);

    /**
     * @brief Señal emitida cuando se registra un nuevo cliente.
     * @param data Datos del nuevo cliente.
     */
    void newClientReg(QHash<UserField, QVariant> data);

    /**
     * @brief Señal emitida cuando hay un error de login.
     * @param msg Mensaje descriptivo del error.
     */
    void loginError(QString msg);
};

#endif // LOGINMANAGER_H
