/*!
 * \file userpreferences.cpp
 * \brief Implementación de la clase UserPreferences.
 *
 * Implementa los métodos de acceso y modificación de las preferencias de usuario, así como
 * la conversión entre estructuras de conexiones y su representación JSON.
 */
#include "userpreferences.h"
#include <QHash>

/*!
 * \brief Constructor que inicializa desde un mapa de preferencias.
 * \param data Mapa clave-valor (QString, QString). Actualmente no implementado.
 */

UserPreferences::UserPreferences(const QMap<QString, QString> &data)
{

}
/*!
 * \brief Constructor por defecto.
 *
 * Inicializa todas las preferencias con valores predeterminados seguros.
 */
UserPreferences::UserPreferences()
    : idUser(-1), dualMode(false), infoSound(true), alertSound(true), mute(false), connectionsJson("") {}

// Métodos de acceso básicos

int UserPreferences::getIdUser() const
{
    return idUser;
}

void UserPreferences::setIdUser(int newIdUser)
{
    idUser = newIdUser;
}

bool UserPreferences::getDualMode() const
{
    return dualMode;
}

void UserPreferences::setDualMode(bool newDualMode)
{
    dualMode = newDualMode;
}

bool UserPreferences::getInfoSound() const
{
    return infoSound;
}

void UserPreferences::setInfoSound(bool newInfoSound)
{
    infoSound = newInfoSound;
}

bool UserPreferences::getAlertSound() const
{
    return alertSound;
}

void UserPreferences::setAlertSound(bool newAlertSound)
{
    alertSound = newAlertSound;
}

bool UserPreferences::getMute() const
{
    return mute;
}

void UserPreferences::setMute(bool newMute)
{
    mute = newMute;
}
/*!
 * \brief Devuelve la cadena JSON que representa las conexiones entre keypoints.
 * \return Cadena en formato JSON.
 */
QString UserPreferences::getConnectionsJson() const
{
    return connectionsJson;
}
/*!
 * \brief Establece directamente la cadena JSON de conexiones.
 * \param newConnectionsJson Cadena con la nueva representación JSON.
 */
void UserPreferences::setConnectionsJson(const QString &newConnectionsJson)
{
    connectionsJson = newConnectionsJson;
}
/*!
 * \brief Convierte la representación JSON de conexiones a una estructura `QHash`.
 *
 * Cada conexión se espera como un array de dos enteros en la forma `[from, to]`.
 * Se genera una etiqueta tipo `"from-to"` como valor para cada par.
 *
 * \return Hash con pares (from, to) como clave y etiquetas como valor.
 */
QHash<QPair<int, int>, QString> UserPreferences::getConnections() const {
    QHash<QPair<int, int>, QString> result;
    if (connectionsJson.isEmpty()) return result;

    try {
        nlohmann::json json = nlohmann::json::parse(connectionsJson.toStdString());
        for (const auto& item : json) {
            if (item.is_array() && item.size() == 2) {
                int from = item[0];
                int to = item[1];
                QString label = QString("%1-%2").arg(from).arg(to);
                result.insert({from, to}, label);
            }
        }
    } catch (...) {
         //qDebug() << "No se pudo interpretar el JSON de conexiones.";
        //error()
    }

    return result;
}
/*!
 * \brief Establece las conexiones entre keypoints y actualiza la cadena JSON correspondiente.
 * \param conns Hash de conexiones en forma (from, to) → etiqueta.
 */
void UserPreferences::setConnections(const QHash<QPair<int, int>, QString>& conns) {
    nlohmann::json jsonArray = nlohmann::json::array();
    for (auto it = conns.begin(); it != conns.end(); ++it) {
        int from = it.key().first;
        int to = it.key().second;
        jsonArray.push_back({from, to});
    }
    connectionsJson = QString::fromStdString(jsonArray.dump());
}

