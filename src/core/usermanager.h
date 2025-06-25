/**
 * @file usermanager.h
 * @brief Declaración de la clase UserManager para la gestión lógica de usuarios y sus perfiles.
 *
 * Proporciona una capa de abstracción para manejar usuarios, autenticación, perfiles de cliente
 * y asignación de planes de entrenamiento, utilizando un `UserRepository` como fuente de datos.
 */
#ifndef USERMANAGER_H
#define USERMANAGER_H
/**
 * @class UserManager
 * @brief Clase que centraliza la lógica de negocio relacionada con los usuarios del sistema.
 *
 * Gestiona creación, actualización, eliminación y autenticación de usuarios,
 * así como asignación de workouts y recuperación de preferencias.
 * Soporta perfiles específicos como `ClientProfile` y preferencias personalizadas.
 */
#include <QObject>
#include <QPointer>
#include "repositories/userrepository.h"
#include "profiles/profiles.h"
#include "profiles/userpreferences.h"
#include <QLoggingCategory>


Q_DECLARE_LOGGING_CATEGORY(userManager);

class UserManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor de UserManager.
     * @param repo Repositorio de usuarios.
     * @param parent Objeto padre opcional.
     */
    explicit UserManager(QSharedPointer<UserRepository> repo, QObject* parent = nullptr);

    /**
     * @brief Devuelve un usuario a partir de su ID.
     */
    QSharedPointer<User> getUser(int id);

    /**
     * @brief Autentica a un usuario por nombre y contraseña (método por implementar).
     */
    bool login(const QString& userName, const QString& password);

    /**
     * @brief Finaliza la sesión del usuario actual y guarda su último login.
     */
    void logout();

    /**
     * @brief Carga directamente como usuario autenticado al usuario con el ID dado.
     */
    bool loging_As(int idUser);

    /**
     * @brief Crea un nuevo cliente con su perfil completo.
     */
    bool createClient(const QHash<UserField, QVariant>& data, QSharedPointer<ClientProfile> profile);

    /**
     * @brief Actualiza el perfil de un cliente existente.
     */
    void updateClientProfile(QSharedPointer<ClientProfile> profile);

    /**
     * @brief Devuelve el perfil del cliente especificado.
     */
    QSharedPointer<ClientProfile> getClientProfile(int idClient);

    /**
     * @brief Recupera las preferencias de un usuario.
     */
    UserPreferences getUserPreferences(int idUser);

    /**
     * @brief Guarda las preferencias de un usuario.
     */
    void updateUserPreferences(const UserPreferences& prefs);

    /**
     * @brief Crea un nuevo nombre de cliente (método en desuso o pendiente).
     */
    QString newClient(const QString& userName, const QString& password);

    /**
     * @brief Obtiene el workout activo para el usuario actual según la fecha.
     */
    int getUserWorkout(QDateTime date);

    /**
     * @brief Asigna un workout a un cliente en una fecha dada.
     */
    bool assignWorkoutToClient(QDateTime fecha, int idWorkout, int idClient = -1);

    /**
     * @brief Elimina la asignación de un workout en una fecha específica.
     */
    bool unassignWorkoutFromClient(QDateTime fecha, int idClient = -1);

    /**
     * @brief Devuelve el usuario actualmente autenticado.
     */
    QSharedPointer<User> getCurrentUser() const;

    /**
     * @brief Establece manualmente el usuario actual.
     */
    void setCurrentUser(QSharedPointer<User> newCurrentUser);

    /**
     * @brief Lista todos los usuarios del tipo especificado.
     */
    QList<QSharedPointer<User>> listUsers(UserType tipo);

    /**
     * @brief Actualiza los datos de un usuario.
     */
    bool updateUser(QSharedPointer<User> user);

    /**
     * @brief Elimina un usuario por su ID.
     */
    bool deleteUser(int user);

    /**
     * @brief Obtiene el siguiente ID disponible para creación de usuario.
     */
    int getNextUserId();

signals:
    /**
     * @brief Señal emitida cuando un usuario inicia sesión correctamente.
     */
    void userLoggedIn(QSharedPointer<User>);

    /**
     * @brief Señal emitida cuando un usuario cierra sesión.
     */
    void userLoggedOut();

    /**
     * @brief Señal genérica para mostrar errores en la interfaz.
     * @param msg Mensaje a mostrar.
     * @param type Tipo de mensaje Qt.
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    QSharedPointer<UserRepository> repository;  ///< Acceso a los datos de usuario.
    QSharedPointer<User> currentUser;           ///< Usuario autenticado en el sistema.
};

#endif // USERMANAGER_H
