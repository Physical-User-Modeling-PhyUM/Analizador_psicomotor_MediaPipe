/*!
 * \file client.h
 * \brief Declaración de la clase Client.
 *
 * La clase Client representa a un usuario del sistema con perfil de cliente.
 * Contiene su perfil físico y un calendario de entrenamientos asignados por fecha.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "clientprofile.h"

/*!
 * \class Client
 * \brief Representa a un cliente del sistema, con su perfil físico y plan de entrenamiento.
 *
 * Hereda de la clase User y añade atributos y métodos específicos para gestionar el perfil del cliente
 * y su calendario de entrenamientos.
 */
class Client : public User {
public:
    /*!
     * \brief Constructor explícito que inicializa un cliente a partir de un mapa de campos.
     * \param dataMap Mapa con los campos obligatorios definidos por UserField.
     * \param parent Objeto padre en la jerarquía de Qt.
     */
    explicit Client(QHash<UserField,QVariant> dataMap, QObject *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~Client() override;

    /*!
     * \brief Devuelve el perfil físico del cliente.
     * \return Puntero compartido a ClientProfile.
     */
    QSharedPointer<ClientProfile> getProfile() const;

    /*!
     * \brief Asigna un nuevo perfil físico al cliente.
     * \param newProfile Perfil a asignar.
     */
    void setProfile(QSharedPointer<ClientProfile> newProfile);

    /*!
     * \brief Obtiene el ID del entrenamiento asignado a una fecha concreta.
     * \param fecha Fecha del entrenamiento.
     * \return ID del entrenamiento o -1 si no hay asignación.
     */
    int getWorkoutId(QDateTime fecha);

    /*!
     * \brief Asigna un entrenamiento a una fecha.
     * \param fecha Fecha de asignación.
     * \param workoutId ID del entrenamiento.
     */
    void setWorkout(QDateTime fecha, int workoutId);

    /*!
     * \brief Elimina el entrenamiento asignado a una fecha.
     * \param fecha Fecha del entrenamiento a eliminar.
     */
    void delWorkout(QDateTime fecha);

    /*!
     * \brief Comprueba si existe un entrenamiento asignado en una fecha.
     * \param fecha Fecha a verificar.
     * \return true si la fecha ya está asignada, false si está disponible.
     */
    bool isDateAvailable(QDateTime fecha);

    /*!
     * \brief Devuelve el plan asignado al cliente.
     * \return Nombre del plan actual.
     */
    QString getPlan() const;

    /*!
     * \brief Asigna un nuevo plan al cliente.
     * \param newPlan Texto con el nombre del nuevo plan.
     */
    void setPlan(const QString &newPlan);

    /*!
     * \brief Devuelve el calendario completo de entrenamientos del cliente.
     * \return Mapa de fechas a IDs de entrenamiento.
     */
    QMap<QDateTime, int> getWorkoutCalendar() const;

    /*!
     * \brief Devuelve el entrenamiento activo más reciente antes o en una fecha determinada.
     * \param date Fecha objetivo.
     * \return ID del entrenamiento activo o -1 si no se encuentra.
     */
    int getActiveWorkout(QDateTime date) const;

signals:
    /*!
     * \brief Señal para mostrar errores relacionados con la UI.
     * \param msg Mensaje de error.
     * \param type Tipo de mensaje (por defecto QtWarningMsg).
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    QString plan;                                 //!< Nombre del plan asignado.
    QSharedPointer<ClientProfile> profile;        //!< Perfil físico del cliente.
    QMap<QDateTime, int> workoutCalendar;         //!< Calendario de entrenamientos asignados.
};

#endif // CLIENT_H
