/*!
 * \file client.cpp
 * \brief Implementación de la clase Client.
 *
 * La clase gestiona la inicialización del cliente, así como su calendario de entrenamientos
 * y su perfil físico.
 */

#include "client.h"
/*!
 * \brief Constructor de la clase Client.
 *
 * Inicializa el objeto con los valores proporcionados en el mapa `dataMap`. Se validan los campos obligatorios:
 * ID, nombre de usuario, correo electrónico y contraseña. Se establece el tipo de usuario como Client y se cargan
 * datos adicionales como la imagen de perfil y las fechas de alta y último acceso.
 *
 * \param dataMap Mapa con los datos del usuario.
 * \param parent Objeto padre (opcional).
 */
Client::Client(QHash<UserField, QVariant> dataMap, QObject* parent)
    : User(-1, "", "", "", parent)  // Inicialización segura por defecto
{
    // Validar que existan todos los campos requeridos para User
    if (!dataMap.contains(UserField::IdUser)
        || !dataMap.contains(UserField::UserName)
        || !dataMap.contains(UserField::Email)
        || !dataMap.contains(UserField::Pass)) {
        qWarning() << "Error: Faltan campos requeridos para crear un Client";
        return;
    }

    // Comprobamos que el valor sea un valor numérico válido, si es un QString, se convertirá a cero
    int id = dataMap.value(UserField::IdUser).toInt();
    if (id < -1) {
        qWarning() << "Constructor>>ID no válido";
        return;
    }

    // Validarmo lod campos obligatorios como cadenas no vacías
    QString userName = dataMap.value(UserField::UserName).toString().trimmed();
    QString email = dataMap.value(UserField::Email).toString().trimmed();
    QString password = dataMap.value(UserField::Pass).toString();
     QString p = dataMap.value(UserField::Plan).toString();
    QDateTime join = dataMap.value(UserField::join_up_date).toDateTime();
     QDateTime log = dataMap.value(UserField::last_login).toDateTime();

    //QString planstr = dataMap.value(UserField::Plan,"Not assigned yet").toString().trimmed();;
    if (userName.isEmpty() || email.isEmpty() || password.isEmpty()) {
        qWarning() << "Campos obligatorios vacíos al crear un Client";
        return;
    }
    this->valid = true;

    idUser = id;
    this->userName = userName;
    this->email = email;
    this->password = password;
    this->userType=UserType::Client;
    this->profile=nullptr;
    this->workoutCalendar=QMap <QDateTime,int>();
    this->join_up_date=join;
    this->last_login=log;
    this->plan=p;

    //this->plan=planstr;

    // QImage image("config/blank-profile.jpg");
    // if (image.isNull()) {
    //     qWarning() << "No se pudo cargar la imagen del perfil.";
    // }
    // this->profile_Picture = image;

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
 * \brief Destructor por defecto.
 */
Client::~Client() = default;

// void Client::assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) {
//     // A ver  que estructura se usa para esto
// }
/*!
 * \brief Devuelve el perfil físico del cliente.
 */
QSharedPointer<ClientProfile> Client::getProfile() const
{
    return profile;
}
/*!
 * \brief Establece un nuevo perfil y le asigna el ID del cliente.
 */
void Client::setProfile(QSharedPointer<ClientProfile>newProfile)
{
    profile = newProfile;
    profile->setIdClient(this->idUser);
}
/*!
 * \brief Devuelve el ID del entrenamiento asignado en una fecha concreta.
 */
int Client::getWorkoutId(QDateTime fecha)
{
    return workoutCalendar.value(fecha,-1);
}
/*!
 * \brief Asigna un entrenamiento a una fecha concreta.
 */
void Client::setWorkout(QDateTime fecha, int workoutId)
{

    workoutCalendar.insert(fecha,workoutId);
}
/*!
 * \brief Elimina el entrenamiento asignado a una fecha específica.
 */
void Client::delWorkout(QDateTime fecha)
{
    workoutCalendar.remove(fecha);
}
/*!
 * \brief Verifica si una fecha ya tiene un entrenamiento asignado.
 */
bool Client::isDateAvailable(QDateTime fecha)
{
    return workoutCalendar.contains(fecha);
}
/*!
 * \brief Devuelve el nombre del plan asignado.
 */
QString Client::getPlan() const
{
    return plan;
}
/*!
 * \brief Asigna un nuevo plan al cliente.
 */
void Client::setPlan(const QString &newPlan)
{
    plan = newPlan;
}
/*!
 * \brief Devuelve el entrenamiento activo más reciente antes o igual a la fecha dada.
 */
int Client::getActiveWorkout(QDateTime date) const
{
    int selectedWorkoutId = -1;
    for (auto it = workoutCalendar.begin(); it != workoutCalendar.end(); ++it) {
        if (it.key() <= date) {
            selectedWorkoutId = it.value();
        } else {
            break;
        }
    }
    return selectedWorkoutId;
}
/*!
 * \brief Devuelve el mapa completo del calendario de entrenamientos.
 */
QMap<QDateTime, int> Client::getWorkoutCalendar() const
{
    return workoutCalendar;
}
