/**
 * @file usermanager.cpp
 * @brief Implementación de la clase UserManager para la gestión y control de usuarios.
 */
#include "usermanager.h"
#include <QDebug>
#include <QTime>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(userManager, "UserManager")
/**
 * @brief Constructor que inicializa el repositorio de usuarios.
 */
UserManager::UserManager(QSharedPointer<UserRepository> repo, QObject* parent)
    : QObject(parent), repository(repo), currentUser(nullptr) {}
/**
 * @brief Recupera un usuario por ID desde el repositorio.
 */
QSharedPointer<User> UserManager::getUser(int id) {
    return repository->getUserById(id);
}
/**
 * @brief Método placeholder de autenticación.
 * @note Siempre devuelve false.
 */
bool UserManager::login(const QString& email, const QString& password) {
    // implementar autenticación real
    qDebug(userManager) << "Login not implemented. Always returns false.";

    return false;
}
/**
 * @brief Inicia sesión como un usuario específico por ID.
 */
bool UserManager::loging_As(int idUser){
     qDebug(userManager) << "Logged as "<<idUser;
     currentUser=getUser(idUser);
     if (currentUser==nullptr)  {
         qWarning(userManager) << "Logged fallido as"<<idUser;
         return false;}
     return true;
}
/**
 * @brief Cierra sesión del usuario actual, guarda el último login y emite señal.
 */
void UserManager::logout() {

    currentUser->setLast_login(QDateTime::currentDateTime());
    repository->saveUser(currentUser);
    currentUser = nullptr;
    emit userLoggedOut();
}
/**
 * @brief Crea un nuevo cliente con su perfil y lo guarda en el repositorio.
 */
bool UserManager::createClient(const QHash<UserField, QVariant>& data,  QSharedPointer<ClientProfile>profile) {
    QSharedPointer<Client> client( new Client(data));
    if (!client) return false;

    client->setProfile(profile);
    repository->saveUser(client);
    return true;
}
/**
 * @brief Actualiza el perfil de cliente en la base de datos.
 */
void UserManager::updateClientProfile( QSharedPointer<ClientProfile> profile) {
    repository->saveClientProfile(profile->getIdClient(),profile);
}
/**
 * @brief Recupera el perfil de un cliente por su ID.
 */
QSharedPointer<ClientProfile> UserManager::getClientProfile(int idClient) {
    return repository->getClientProfile(idClient);
}
/**
 * @brief Recupera las preferencias del usuario desde el repositorio.
 */
UserPreferences UserManager::getUserPreferences(int idUser) {
    return repository->getUserPreferences(idUser);
}
/**
 * @brief Guarda las preferencias de un usuario.
 */
void UserManager::updateUserPreferences(const UserPreferences& prefs) {
    repository->saveUserPreferences(prefs);
    qDebug(userManager) << "Preferenciass de usuario guardadas" << prefs.getIdUser();
}



/**
 * @brief Obtiene el ID del workout activo en una fecha dada para el usuario actual.
 */
int UserManager::getUserWorkout(QDateTime date)
{
    if (currentUser == nullptr) return -1;

    if (currentUser->getUserType() == UserType::Client)
    {
       QSharedPointer<Client> clientPtr = qSharedPointerCast<Client>(currentUser);
        int workoutId = clientPtr->getActiveWorkout(date);
        if (workoutId != -1){

            return workoutId;
        }
        else {
            emit ui_errorMessage("No hay planes de entrenamientos disponibles", QtMsgType::QtWarningMsg);

            return -1;
        }
    }
    else {
        emit ui_errorMessage("No hay planes de entrenamientos disponibles", QtMsgType::QtWarningMsg);
        return -1;
    }
}

/**
 * @brief Asigna un workout a un cliente (actual o por ID) en una fecha dada.
 */
bool UserManager::assignWorkoutToClient( QDateTime fecha, int idWorkout, int idClient)
{
    //En este caso se incluira el workout al currentUser
    if (idClient==-1 ){
        if(currentUser==nullptr) {
            emit ui_errorMessage("No se puede asignar un plan al usuario seleccionado", QtMsgType:: QtWarningMsg);
            return false;
        }
        if (currentUser->getUserType()==UserType::Client)
            {
                //Client* clientPtr = static_cast<Client*>(currentUser.data());
            QSharedPointer<Client> clientPtr = qSharedPointerCast<Client>(currentUser);
            clientPtr->setWorkout(fecha,idWorkout);
                return true;
            }
    }
    else {
        QSharedPointer<User> user = repository->getUserById(idClient);
        if (user == nullptr) {
            emit ui_errorMessage("Error recuperando los datos del usuario seleccionado", QtMsgType::QtWarningMsg);
            return false;
        }
        QSharedPointer<Client> clientPtr = qSharedPointerCast<Client>(user);
        if (clientPtr == nullptr) {
            emit ui_errorMessage("Error al convertir el usuario a cliente", QtMsgType::QtWarningMsg);
            return false;
        }
        clientPtr->setWorkout(fecha, idWorkout);
        return repository->saveUser(clientPtr);


    };
    return true;
}
/**
 * @brief Elimina el workout asignado a un cliente (actual o por ID) en una fecha.
 */
bool UserManager::unassignWorkoutFromClient( QDateTime fecha,int idClient)
{
    //En este caso se incluira el workout al currentUser
    if (idClient==-1 ){
        if(currentUser==nullptr) {
            emit ui_errorMessage("No se puede asignar un plan al usuario seleccionado", QtMsgType:: QtWarningMsg);
            return false;
        }
        if (currentUser->getUserType()==UserType::Client)
        {
            //Client* clientPtr = static_cast<Client*>(currentUser.data());
            QSharedPointer<Client> clientPtr = qSharedPointerCast<Client>(currentUser);
            clientPtr->delWorkout(fecha);
            return true;
        }
    }
    else {
        QSharedPointer<User> user= repository->getUserById(idClient);
        if (user==nullptr){
            emit ui_errorMessage("Error recuperando los datos del usuario seleccionado", QtMsgType:: QtWarningMsg);
            return false;
        }
        QSharedPointer<Client> clientPtr = qSharedPointerCast<Client>(currentUser);
        if (clientPtr==nullptr){
            emit ui_errorMessage("Error recuperando los datos del usuario seleccionado", QtMsgType:: QtWarningMsg);
            return false;
        }
        clientPtr->delWorkout(fecha);
        return repository->saveUser(clientPtr);

    };
    return true;
}
/**
 * @brief Actualiza los datos de un usuario en el repositorio.
 */
bool UserManager::updateUser(QSharedPointer<User>user){

     return repository->saveUser(user);
}
/**
 * @brief Elimina un usuario por su identificador.
 */
bool UserManager::deleteUser( int user){

    return repository->deleteUser(user);
}
/**
 * @brief Devuelve el usuario actualmente autenticado.
 */
QSharedPointer<User> UserManager::getCurrentUser() const
{
    return currentUser;
}
/**
 * @brief Establece un nuevo usuario como usuario autenticado.
 */
void UserManager::setCurrentUser(QSharedPointer<User> newCurrentUser)
{
    currentUser = newCurrentUser;
}
/**
 * @brief Lista todos los usuarios del tipo especificado, eliminando duplicados.
 */
QList<QSharedPointer<User>> UserManager::listUsers(UserType tipo)
{
    // QList<QSharedPointer<User>> usersList;
    // //vamos a listar todos los usuarios del sistema para tener los datos comunes de los clientes
    // //es decir los datos de la tabla Users
     //DBTable table = repository->listUsers(UserType::Unknown);

    // //table.print();
    // QSharedPointer<User> user =nullptr;
    // for (int i = 0; i < table.rowCount(); ++i) {

    //     if (table.value(i, "userType").toString().toLower()==UserTypeToString(tipo).toLower()||
    //         tipo==UserType::Unknown){
    //          user = getUser(table.value(i, "idUser").toInt());
    //     }

    //     if (user != nullptr && user->isValid())
    //         usersList.append(user);
    // }
    QList<QSharedPointer<User>> usersList;
    QSet<int> ids;
    DBTable table = repository->listUsers(UserType::Unknown);
    for (int i = 0; i < table.rowCount(); ++i) {
        int id = table.value(i, "idUser").toInt();

        if (ids.contains(id)) continue;

        if (table.value(i, "userType").toString().toLower() == UserTypeToString(tipo).toLower() || tipo == UserType::Unknown) {
            QSharedPointer<User> user = getUser(id);
            if (user != nullptr && user->isValid()) {
                usersList.append(user);
                ids.insert(id);
            }
        }
    }

    qDebug(userManager) << "Se ha creado una lista de usuarios con " << usersList.size();
    return usersList;
}
/**
 * @brief Devuelve el siguiente ID libre para nuevo usuario.
 */
int UserManager::getNextUserId() {
    return repository->getNextUserId();
}
