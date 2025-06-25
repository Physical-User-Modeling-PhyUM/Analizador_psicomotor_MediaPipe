/*!
 * \file user.h
 * \brief Declaración de la clase base User.
 *
 * La clase User representa un usuario genérico del sistema, proporcionando atributos comunes
 * como nombre, correo electrónico, contraseña, fechas de acceso y perfil. Sirve como clase base
 * para tipos de usuario más específicos como Client, FitnessTrainer o Admin.
 */

#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QImage>
#include "enums/enums.h"

/*!
 * \class User
 * \brief Clase base abstracta que representa a un usuario del sistema.
 *
 * Esta clase proporciona los atributos y métodos básicos para la gestión de usuarios,
 * incluyendo identificación, autenticación y metadatos generales. Se espera que las
 * clases derivadas amplíen su funcionalidad.
 */
class User : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief Constructor del usuario.
     * \param id ID numérico del usuario.
     * \param userName Nombre de usuario visible.
     * \param email Dirección de correo electrónico.
     * \param password Contraseña en formato plano o encriptado.
     * \param parent Objeto padre para la jerarquía de Qt.
     */
    explicit User(int id, const QString &userName, const QString &email, const QString &password, QObject *parent = nullptr);

    /*!
     * \brief Destructor virtual.
     */
    virtual ~User() = default;

    /// Accesores y modificadores
    int getId() const;
    void setId(int newIdUser);

    QString getUserName() const;
    void setUserName(const QString &newUserName);

    QString getEmail() const;
    void setEmail(const QString &newEmail);

    void changePassword(const QString &newPassword);
    QString getPassword();

    /*!
     * \brief Cancela o desactiva la cuenta del usuario.
     *
     * Método virtual que puede redefinirse en las subclases para eliminar datos o aplicar
     * una lógica personalizada.
     */
    void cancelAccount();

    QDateTime getJoin_up_date() const;
    void setJoin_up_date(const QDateTime &newJoin_up_date);

    QDateTime getLast_login() const;
    void setLast_login(const QDateTime &newLast_login);

    QImage getProfile_Picture() const;
    void setProfile_Picture(const QImage &newProfile_Picture);

    UserType getUserType() const;
    void setUserType(UserType newUserType);

    /*!
     * \brief Indica si el usuario fue construido correctamente.
     * \return true si el objeto está en estado válido; false en caso contrario.
     */
    bool isValid() const;

protected:
    int idUser;                      //!< Identificador único del usuario.
    UserType userType;              //!< Tipo de usuario (Client, Trainer, Admin...).
    bool valid = false;             //!< Estado de validez del objeto.

    QString userName;               //!< Nombre de usuario.
    QString email;                  //!< Dirección de correo electrónico.
    QString password;               //!< Contraseña.
    QDateTime join_up_date;         //!< Fecha de alta en el sistema.
    QDateTime last_login;           //!< Fecha del último acceso.
    QImage profile_Picture;         //!< Imagen de perfil del usuario.
};

#endif // USER_H
