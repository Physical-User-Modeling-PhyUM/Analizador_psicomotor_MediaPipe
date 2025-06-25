/*!
 * \file user.cpp
 * \brief Implementación de la clase base User.
 *
 * Contiene la lógica de inicialización del usuario genérico y los métodos de acceso
 * a sus atributos principales, como nombre, correo, fechas y tipo de usuario.
 */
#include "user.h"
/*!
 * \brief Constructor del objeto User.
 *
 * Inicializa los campos fundamentales del usuario. No realiza validaciones internas,
 * ya que se espera que estas sean gestionadas por las clases derivadas (como Client, Trainer).
 *
 * \param id Identificador del usuario.
 * \param userName Nombre visible del usuario.
 * \param email Dirección de correo electrónico.
 * \param password Contraseña.
 * \param parent Objeto padre dentro del modelo de objetos de Qt.
 */
User::User(int id, const QString &userName, const QString &email, const QString &password, QObject *parent)
    : QObject(parent), idUser(id), userName(userName), email(email), password(password)
{
    // if (id < 0 || userName.isEmpty() || email.isEmpty() || password.isEmpty()) {
    //     qWarning() << "Error: intento de crear un User con datos inválidos"
    //                << "id:"<<id
    //                <<"userName:"<<userName
    //                <<"Email:"<<email
    //                <<"pass"<<password;

    //         ;
    //}
}

/*!
 * \brief Devuelve el ID del usuario.
 */
int User::getId() const {
    return idUser;
}
/*!
 * \brief Devuelve el nombre del usuario.
 */
QString User::getUserName() const {
    return userName;
}
/*!
 * \brief Devuelve el correo electrónico del usuario.
 */
QString User::getEmail() const {
    return email;
}
/*!
 * \brief Asigna un nuevo nombre de usuario.
 */
void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}
/*!
 * \brief Asigna un nuevo correo electrónico.
 */
void User::setEmail(const QString &newEmail)
{
    email=newEmail;
}
/*!
 * \brief Cambia la contraseña del usuario.
 */
void User::changePassword(const QString &newPassword) {
    password = newPassword;
}
/*!
 * \brief Devuelve la contraseña actual del usuario.
 *
 * Nota: en un sistema real, la contraseña nunca debería estar disponible en texto plano.
 */
QString User::getPassword()
{
    return password;
}
/*!
 * \brief Marca la cuenta del usuario como cancelada.
 *
 * Método virtual que puede ser redefinido por clases hijas.
 */
void User::cancelAccount() {
    // Implement account cancellation logic
}

/*!
 * \brief Devuelve la fecha de alta del usuario.
 */
QDateTime User::getJoin_up_date() const
{
    return join_up_date;
}
/*!
 * \brief Establece la fecha de alta del usuario.
 */
void User::setJoin_up_date(const QDateTime &newJoin_up_date)
{
    join_up_date = newJoin_up_date;
}
/*!
 * \brief Devuelve la fecha del último inicio de sesión.
 */
QDateTime User::getLast_login() const
{
    return last_login;
}
/*!
 * \brief Establece la fecha del último inicio de sesión.
 */
void User::setLast_login(const QDateTime &newLast_login)
{
    last_login = newLast_login;
}
/*!
 * \brief Devuelve la imagen de perfil del usuario.
 */
QImage User::getProfile_Picture() const
{
    return profile_Picture;
}
/*!
 * \brief Establece la imagen de perfil del usuario.
 */
void User::setProfile_Picture(const QImage &newProfile_Picture)
{
    profile_Picture = newProfile_Picture;
}
/*!
 * \brief Devuelve el tipo de usuario (Client, Trainer, Admin...).
 */
UserType User::getUserType() const
{
    return userType;
}
/*!
 * \brief Devuelve si el usuario ha sido construido correctamente.
 */
bool User::isValid() const { return valid; }
/*!
 * \brief Establece el ID del usuario.
 */
void User::setId(int newIdUser)
{
    idUser = newIdUser;
}
/*!
 * \brief Establece el tipo de usuario.
 */
void User::setUserType(UserType newUserType)
{
    userType = newUserType;
}
