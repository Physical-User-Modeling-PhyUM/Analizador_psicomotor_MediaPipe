/**
 * @file feedback.h
 * @brief Declara la clase FeedBack utilizada para generar mensajes según condiciones biomecánicas detectadas.
 *
 * La clase FeedBack se encarga de clasificar las condiciones generadas durante un ejercicio (por ejemplo, errores posturales,
 * sobreesfuerzo, o ejecución óptima), y generar mensajes categorizados para su presentación al usuario en tiempo real.
 */

#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QList>
#include "condition.h"

/**
 * @class FeedBack
 * @brief Clase que procesa una lista de condiciones y genera mensajes clasificados para retroalimentación al usuario.
 *
 * Al recibir una lista de condiciones detectadas durante el ejercicio, esta clase se encarga de analizarlas,
 * clasificarlas según su severidad (crítica, alerta, informativa) y generar los mensajes correspondientes.
 */
class FeedBack
{
public:
    /**
     * @brief Constructor que inicializa el objeto con una lista de condiciones.
     * @param conds Lista de condiciones detectadas por el sistema.
     */
    explicit FeedBack(QList<Condition> conds);

    /**
     * @brief Devuelve los mensajes clasificados como críticos.
     * @return Lista de cadenas con mensajes críticos.
     */
    QList<QString> getCriticalMessages() const;

    /**
     * @brief Devuelve los mensajes informativos.
     * @return Lista de cadenas informativas.
     */
    QList<QString> getInfoMessages() const;

    /**
     * @brief Devuelve los mensajes clasificados como alertas.
     * @return Lista de cadenas de alerta.
     */
    QList<QString> getAlerts() const;

    /**
     * @brief Comprueba si se ha detectado una condición específica.
     * @param cond Tipo de condición a verificar.
     * @return true si la condición está presente, false en caso contrario.
     */
    bool contains(ConditionType cond) const;

    /**
     * @brief Devuelve el mensaje asociado a una condición específica.
     * @param type Tipo de condición.
     * @return Cadena con el mensaje correspondiente.
     */
    QString getMessage(ConditionType type) const;

    /**
     * @brief Devuelve la categoría asociada a una condición.
     * @param type Tipo de condición.
     * @return Categoría de la condición.
     */
    ConditionCategory getCategory(ConditionType type) const;

    /**
     * @brief Indica si una condición es crítica.
     * @param type Tipo de condición.
     * @return true si es crítica, false en caso contrario.
     */
    bool isCritical(ConditionType type) const;

    /**
     * @brief Indica si una condición es una alerta.
     * @param type Tipo de condición.
     * @return true si es una alerta, false en caso contrario.
     */
    bool isAlert(ConditionType type) const;

    /**
     * @brief Indica si una condición es informativa.
     * @param type Tipo de condición.
     * @return true si es informativa, false en caso contrario.
     */
    bool isInfo(ConditionType type) const;

    /**
     * @brief Indica si una condición representa una ejecución óptima.
     * @param type Tipo de condición.
     * @return true si representa ejecución óptima, false en caso contrario.
     */
    bool isOptimal(ConditionType type) const;

    /**
     * @brief Devuelve la lista de condiciones procesadas.
     * @return Lista de tipos de condición detectados.
     */
    QList<ConditionType> getEntriesList() const;

private:
    QList<QString> criticalMessages; ///< Mensajes críticos acumulados.
    QList<QString> infoMessages;     ///< Mensajes informativos acumulados.
    QList<QString> alerts;           ///< Mensajes de alerta acumulados.

    QVector<bool> conditionsMap;     ///< Mapa interno para verificar condiciones detectadas.

    /**
     * @struct FeedbackEntry
     * @brief Estructura que almacena un mensaje y su categoría asociada.
     */
    struct FeedbackEntry {
        QString message;                 ///< Texto del mensaje.
        ConditionCategory category;     ///< Severidad de la condición.
    };

    QHash<ConditionType, FeedbackEntry> entries; ///< Mapa de entradas clasificadas por condición.

    /**
     * @brief Procesa la lista de condiciones y genera mensajes correspondientes.
     * @param conds Lista de condiciones a procesar.
     */
    void processConditions(QList<Condition> &conds);
};

#endif // FEEDBACK_H
