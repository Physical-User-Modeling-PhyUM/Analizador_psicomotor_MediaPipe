/*!
 * \file userpreferences.h
 * \brief Declaración de la clase UserPreferences.
 *
 * La clase UserPreferences encapsula las configuraciones personalizadas de un usuario,
 * incluyendo opciones relacionadas con sonido, modo de captura, y conexiones entre keypoints.
 * Admite serialización en formato JSON para facilitar su almacenamiento.
 */

#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include <QString>
#include <QMap>
#include <nlohmann/json.hpp>

/*!
 * \class UserPreferences
 * \brief Clase que almacena las preferencias de configuración de un usuario.
 *
 * Esta clase permite gestionar configuraciones como el modo dual de cámara, el uso de sonidos informativos
 * y de alerta, el estado de silencio general, y una estructura JSON que representa conexiones entre keypoints.
 * La configuración puede exportarse/importarse desde una cadena JSON.
 */
class UserPreferences {
public:
    /*!
     * \brief Constructor a partir de un mapa de clave-valor (a definir).
     * \param data Mapa con las preferencias iniciales como cadenas.
     */
    explicit UserPreferences(const QMap<QString, QString> &data);

    /*!
     * \brief Constructor por defecto.
     */
    UserPreferences();

    /// Métodos de acceso al ID del usuario
    int getIdUser() const;
    void setIdUser(int newIdUser);

    /// Métodos de acceso al modo dual
    bool getDualMode() const;
    void setDualMode(bool newDualMode);

    /// Métodos de acceso al sonido informativo
    bool getInfoSound() const;
    void setInfoSound(bool newInfoSound);

    /// Métodos de acceso al sonido de alerta
    bool getAlertSound() const;
    void setAlertSound(bool newAlertSound);

    /// Métodos de acceso al modo silencio
    bool getMute() const;
    void setMute(bool newMute);

    /// Conexiones codificadas en JSON como QString
    QString getConnectionsJson() const;
    void setConnectionsJson(const QString &newConnectionsJson);

    /*!
     * \brief Devuelve el conjunto de conexiones entre keypoints.
     * \return Hash con pares de enteros como clave y etiquetas descriptivas como valor.
     */
    QHash<QPair<int, int>, QString> getConnections() const;

    /*!
     * \brief Establece el conjunto de conexiones y actualiza su representación JSON.
     * \param conns Hash de conexiones (par de enteros → etiqueta).
     */
    void setConnections(const QHash<QPair<int, int>, QString>& conns);

private:
    int idUser;               //!< ID del usuario propietario de estas preferencias.
    bool dualMode;            //!< Indica si se habilita la captura con dos cámaras.
    bool infoSound;           //!< Activa o desactiva el sonido informativo.
    bool alertSound;          //!< Activa o desactiva el sonido de alerta.
    bool mute;                //!< Desactiva todos los sonidos si está activo.
    QString connectionsJson;  //!< Conexiones codificadas en formato JSON.
};

#endif // USERPREFERENCES_H
