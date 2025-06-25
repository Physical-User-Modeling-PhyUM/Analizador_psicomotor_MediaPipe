/**
 * @file loginmanager.cpp
 * @brief Implementación de la clase LoginManager para autenticación y registro de usuarios.
 */

#include "core/loginmanager.h"
#include <QCryptographicHash>
#include <QDebug>

/**
 * @brief Constructor que inicializa el LoginManager con la base de datos y el gestor de usuarios.
 */
LoginManager::LoginManager(QSharedPointer<DBManager> db,QSharedPointer<UserManager> uManager, QObject* parent)
    : QObject(parent), db(db), userManager(uManager) {}

/**
 * @brief Devuelve el hash SHA-256 de una contraseña en formato hexadecimal.
 * @param password Contraseña en texto plano.
 * @return Cadena con el hash en hexadecimal.
 */
QString LoginManager::hashPassword(const QString& password) const {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}
/**
 * @brief Verifica si una contraseña coincide con su hash.
 * @param password Contraseña en texto plano.
 * @param hashed Hash previamente almacenado.
 * @return true si coinciden.
 */
bool LoginManager::verifyPassword(const QString& password, const QString& hashed) const {
    return hashPassword(password) == hashed;
}
/**
 * @brief Intenta iniciar sesión con nombre de usuario o email y contraseña.
 *
 * Este método busca primero por nombre de usuario, y si no lo encuentra, lo hace por email.
 * Luego compara la contraseña con el hash almacenado.
 *
 * @param User_Email Correo o nombre de usuario.
 * @param password Contraseña ingresada.
 * @return Usuario autenticado o nullptr si falla.
 */
QSharedPointer<User> LoginManager::login(const QString& User_Email, const QString& password) {

    QSharedPointer<User> user=nullptr;
    DBTable tableE =db->get(TypeBDEnum::User,DBFilter::Email,User_Email);
    DBTable tableU =db->get(TypeBDEnum::User,DBFilter::UserName,User_Email);
    QString columnUser=UserFieldToString(UserField::IdUser);
    QString columnPass=UserFieldToString(UserField::Pass);

    if (tableU.rowCount() == 0 && tableE.rowCount() == 0) {
        emit loginError(QString("No existe un Usuario con esos datos"));
        return nullptr;
    };
    int idUser=-1;
    if (tableU.rowCount() != 0){

        //descomentar para que funcione el cifrado de password

        // QString storedHash = tableU.value(0, columnPass).toString();
        // if (!verifyPassword(password, storedHash)) {
        //     emit loginError(QString("Usuario o password incorrectos"));
        //     return nullptr;
        // };
        if(!(tableU.value(0, columnPass).toString()==password)) return nullptr;

        idUser=tableU.value(0, columnUser).toInt();
    }else{
        QString storedHash = tableE.value(0, columnPass).toString();
        if (!verifyPassword(password, storedHash)) {
            emit loginError(QString("Usuario o password incorrectos"));
            return nullptr;
        }

        idUser=tableE.value(0, columnUser).toInt();
    }
    if(idUser!=-1) user=userManager->getUser(idUser);

    if (user==nullptr) {
        emit loginError(QString("Error recuperando el ususario"));
        return nullptr;
    }

   return user;
}
/**
 * @brief Registra un nuevo cliente creando un mapa de datos con contraseña encriptada.
 * @param user_Name Nombre de usuario.
 * @param user_Email Correo electrónico.
 * @param password Contraseña en texto plano.
 * @return QHash con campos de usuario.
 */
 QHash<UserField, QVariant> LoginManager::registerNewClient(const QString& user_Name,const QString& user_Email, const QString& password) {

    QHash<UserField, QVariant> dataMap;
    dataMap.insert(UserField::IdUser,-1);
    dataMap.insert(UserField::UserName,user_Name);
    dataMap.insert(UserField::Email,user_Email);
    dataMap.insert(UserField::Pass,hashPassword(password));

    //QString pass=client->getPassword();
    //client->changePassword(hashPassword(pass));
    return dataMap;
}
 /**
 * @brief Cambia la contraseña de un usuario si la contraseña antigua es válida.
 * @param idUser ID del usuario.
 * @param oldPassword Contraseña actual.
 * @param newPassword Nueva contraseña.
 * @return true si se realizó el cambio, false en caso contrario.
 */
bool LoginManager::changePassword(int idUser,const QString& oldPassword,const QString& newPassword) {

    QString newHash = hashPassword(newPassword);

    DBTable table= db->get(TypeBDEnum::User,DBFilter::IdUser,QString::number(idUser));
    if (table.rowCount()==0) {
        emit loginError(QString("No se ha podido cambiar el password, /n si el problema persiste contacte con un administrador"));
        return false;
    }
    QString column=UserFieldToString(UserField::Pass);
    QString storedHash = table.value(0, column).toString();
    if (!verifyPassword(oldPassword, storedHash)) {
        emit loginError(QString(" password anterior incorrecto"));
        return false;
    };

    table.setValue(0,column,newPassword);
        return true;
}
