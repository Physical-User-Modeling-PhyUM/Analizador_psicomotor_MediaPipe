/**
 * @file userrepository.h
 * @brief Declaración de la clase UserRepository, encargada de la gestión de usuarios y perfiles en la base de datos.
 *
 * Esta clase encapsula la lógica necesaria para acceder, almacenar y manipular información de usuarios,
 * incluyendo datos específicos para Clientes, Administradores y Entrenadores, así como sus preferencias y perfiles físicos.
 * Se apoya en la clase DBManager para interactuar con la base de datos subyacente.
 */

#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QObject>
#include <QList>
#include "profiles/profiles.h"
#include "core/dbmanager.h"
#include <QPointer>
#include "enums/enums.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(UserRepo);

/**
 * @class UserRepository
 * @brief Repositorio responsable de la persistencia y recuperación de datos de usuarios y sus perfiles asociados.
 *
 * Esta clase se utiliza como capa de acceso a datos para la entidad `User`, así como para la gestión de perfiles específicos
 * como `ClientProfile` y preferencias de usuario (`UserPreferences`). También permite operaciones de alta, baja y modificación,
 * encapsulando los detalles de acceso a la base de datos y ofreciendo una interfaz clara para el resto del sistema.
 */
class UserRepository : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase.
     * @param dbManager Puntero al gestor de base de datos a utilizar.
     * @param parent Objeto padre de Qt, por defecto nullptr.
     */
    explicit UserRepository(DBManager* dbManager, QObject* parent = nullptr);

    /**
     * @brief Recupera un usuario por su ID, devolviendo una instancia concreta según su tipo.
     * @param id Identificador único del usuario.
     * @return Puntero compartido al objeto User correspondiente, o nullptr si no se encuentra.
     */
    QSharedPointer<User> getUserById(int id);

    /**
     * @brief Devuelve una tabla con todos los usuarios del tipo especificado.
     * @param tipo Tipo de usuario a filtrar (Client, Trainer, Admin, o Unknown).
     * @return Instancia de DBTable con los datos encontrados.
     */
    DBTable listUsers(UserType tipo);

    /**
     * @brief Recupera los datos puros de la base de datos sin instanciar objetos de dominio.
     * @param tipo Tipo de usuario a consultar.
     * @return Tabla con los datos de los usuarios.
     */
    DBTable getUsersData(UserType tipo);

    /**
     * @brief Recupera el perfil completo de un cliente específico.
     * @param idClient ID del cliente.
     * @return Puntero compartido a ClientProfile con los datos obtenidos.
     */
    QSharedPointer<ClientProfile> getClientProfile(int idClient);

    /**
     * @brief Guarda o actualiza el perfil de un cliente.
     * @param id ID del cliente.
     * @param profile Puntero al perfil del cliente a guardar.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
    bool saveClientProfile(int id, QSharedPointer<ClientProfile> profile);

    /**
     * @brief Guarda o actualiza un usuario (y sus datos específicos por rol).
     * @param user Objeto User a persistir.
     * @return true si la operación fue exitosa.
     */
    bool saveUser(QSharedPointer<User> user);

    /**
     * @brief Elimina un usuario y sus datos asociados en todas las tablas correspondientes.
     * @param id ID del usuario a eliminar.
     * @return true si se eliminaron correctamente todos los registros.
     */
    bool deleteUser(int id);

    /**
     * @brief Comprueba si un nombre de usuario está disponible o es válido.
     * @param newUserName Nombre de usuario a verificar.
     * @return 1 si está disponible, 0 si ya está en uso (puede ser ampliado).
     */
    int checkUserName(QString newUserName) const;

    /**
     * @brief Recupera las preferencias del usuario desde la base de datos.
     * @param idUser ID del usuario.
     * @return Estructura UserPreferences con los valores almacenados.
     */
    UserPreferences getUserPreferences(int idUser);

    /**
     * @brief Guarda las preferencias del usuario en la base de datos.
     * @param prefs Estructura de preferencias a guardar.
     * @return true si la operación fue exitosa.
     */
    bool saveUserPreferences(UserPreferences prefs);

    /**
     * @brief Genera y devuelve el siguiente ID disponible para un nuevo usuario.
     * @return ID numérico único.
     */
    int getNextUserId();

private:
    /**
     * @brief Referencia al gestor de base de datos para todas las operaciones de persistencia.
     */
    DBManager* db;

    /**
     * @brief Construye un objeto DBTable a partir de un mapa de datos genérico.
     * @param idClient ID del cliente al que pertenecen los datos.
     * @param data Mapa de campos y valores a guardar.
     * @param tableType Tipo de tabla donde se almacenarán los datos.
     * @return Objeto DBTable listo para ser persistido.
     */
    DBTable buildDBTable(int idClient, const QVariantMap& data, TypeBDEnum tableType);
};

#endif // USERREPOSITORY_H
