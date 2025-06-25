#ifndef SESIONREPORT_H
#define SESIONREPORT_H

#include <QHash>
#include <QString>
#include <QList>
#include <QSet>
#include <QPair>
#include "condition.h"
#include <nlohmann/json.hpp>

/*!
 * \file sesionreport.h
 * \brief Declaración de la clase SesionReport para el almacenamiento estructurado de condiciones detectadas durante la ejecución del ejercicio.
 *
 * Esta clase permite almacenar, consultar y exportar en JSON todas las condiciones generadas durante una sesión de entrenamiento,
 * clasificadas por serie, repetición y estado de la máquina. También permite almacenar los ángulos globales y los valores de sobrecarga por línea.
 */

class SesionReport
{
public:
    /*!
     * \brief Constructor por defecto de la clase SesionReport.
     */
    SesionReport();

    /*!
     * \brief Agrega una condición a la estructura de datos de la sesión.
     * \param serie Número de serie del ejercicio.
     * \param rep Número de repetición dentro de la serie.
     * \param stateId ID del estado activo al registrar la condición.
     * \param cond Condición que se desea registrar.
     */
    void addCondition(int serie, int rep, int stateId, Condition cond);

    /*!
     * \brief Devuelve las series que contienen datos registrados.
     * \return Lista de enteros representando índices de series.
     */
    QList<int> getStoredSeries() const;

    /*!
     * \brief Devuelve la lista de repeticiones almacenadas para una serie dada.
     * \param serie Número de serie a consultar.
     * \return Lista de repeticiones almacenadas.
     */
    QList<int> getStoredRepetitionsInSerie(int serie) const;

    /*!
     * \brief Obtiene todas las condiciones almacenadas en una repetición.
     * \param serie Número de serie.
     * \param rep Número de repetición.
     * \param expand Si es true, repite condiciones según su contador; si es false, lista única.
     * \return Lista de condiciones encontradas.
     */
    QList<Condition> getConditions(int serie, int rep, bool expand = true) const;

    /*!
     * \brief Devuelve las condiciones sin expandir pero junto con su ID de estado.
     * \param serie Serie objetivo.
     * \param rep Repetición objetivo.
     * \return Lista de pares <Condición, ID de estado>.
     */
    QList<QPair<Condition, int>> getRawConditions(int serie, int rep) const;

    /*!
     * \brief Asigna un nombre a un estado identificado por su ID.
     * \param idState ID del estado.
     * \param name Nombre descriptivo del estado.
     */
    void setStateName(int idState, const QString& name);

    /*!
     * \brief Devuelve las vistas utilizadas en la sesión (Frontal, Lateral...).
     * \return Conjunto de vistas utilizadas.
     */
    QSet<PoseView> getUsedViews() const;

    /*!
     * \brief Elimina toda la información almacenada en el objeto.
     */
    void reset();

    /*!
     * \brief Devuelve el ID único de la sesión.
     * \return Entero identificador de la sesión.
     */
    int getIdSesion() const;

    /*!
     * \brief Asigna un nuevo ID a la sesión.
     * \param newIdSesion Nuevo identificador.
     */
    void setIdSesion(int newIdSesion);

    /*!
     * \brief Establece los valores de sobrecarga angular global.
     * \param newGlobalAngleOverloads Estructura de datos por serie, rep, estado, vista, línea.
     */
    void setGlobalAngleOverloads(const QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, double>>>>> &newGlobalAngleOverloads);

    /*!
     * \brief Establece los rangos angulares globales por línea y vista.
     * \param data Mapa anidado de datos [serie][rep][estado][vista][línea] → {min, max}.
     */
    void setLineAngleRange(const QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, QPair<double, double>>>>>> & data);

    /*!
     * \brief Convierte la información de condiciones en formato JSON.
     * \return Objeto JSON representando series, repeticiones y condiciones.
     */
    nlohmann::json toSeriesJson() const;

    /*!
     * \brief Convierte los rangos angulares globales en formato JSON.
     * \return Objeto JSON de ángulos globales.
     */
    nlohmann::json toGlobalAnglesJson() const;

    /*!
     * \brief Carga la información de condiciones desde JSON.
     * \param j Objeto JSON con las condiciones por serie, repetición y estado.
     */
    void fromSeriesJson(const nlohmann::json& j);

    /*!
     * \brief Carga los rangos angulares globales desde un objeto JSON.
     * \param j Objeto JSON con la estructura de rangos.
     */
    void fromGlobalAnglesJson(const nlohmann::json& j);

    /*!
     * \brief Convierte los datos de sobrecarga angular a formato JSON.
     * \return JSON con los valores de sobrecarga angular.
     */
    nlohmann::json toGlobalOverloadsJson() const;

    /*!
     * \brief Carga los valores de sobrecarga desde JSON.
     * \param json Objeto JSON con datos de sobrecarga.
     */
    void fromGlobalOverloadsJson(const nlohmann::json& json);

    /*!
     * \brief Devuelve el texto completo del log de condiciones.
     * \return Cadena con el log de eventos.
     */
    QString getLog() const;

    /*!
     * \brief Establece un nuevo texto de log.
     * \param newLog Texto a asignar.
     */
    void setLog(const QString &newLog);

    /*!
     * \brief Devuelve el contenido completo de seriesData.
     * \return Estructura completa de condiciones por serie, repetición y estado.
     */
    QHash<int, QHash<int, QHash<int, QHash<Condition, int>>>> getSeriesData() const;

    /*!
     * \brief Devuelve una representación en texto plano del contenido de seriesData.
     * \return Cadena de texto con los datos.
     */
    QString printSeriesData() const;

    /*!
     * \brief Devuelve el nombre asignado a cada estado por ID.
     * \return Mapa de IDs de estado a nombre.
     */
    QHash<int, QString> getStateNames() const;

    /*!
     * \brief Asigna los nombres de todos los estados.
     * \param newStateNames Hash de nombres por ID de estado.
     */
    void setStateNames(const QHash<int, QString> &newStateNames);

    /*!
     * \brief Devuelve los rangos globales angulares por línea y estado.
     * \return Estructura por serie, rep, estado, vista, línea.
     */
    QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, QPair<double, double>>>>>> getGlobalAngleRange() const;

    /*!
     * \brief Devuelve los valores de sobrecarga angular por línea y estado.
     * \return Mapa anidado de series, repeticiones, estados, vistas, líneas.
     */
    QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, double>>>>> getGlobalAngleOverloads() const;

    /*!
     * \brief Devuelve los encabezados de columna para la visualización del log.
     * \return Lista de nombres de columnas.
     */
    QStringList getLogColumnHeaders() const;

    /*!
     * \brief Devuelve el contenido del log como tabla para representación UI.
     * \return Lista de filas con columnas: View, Serie, Repetición, Estado, Texto.
     */
    QList<QList<QVariant>> parseLog() const;

private:
    QHash<int, QHash<int, QHash<int, QHash<Condition, int>>>> seriesData; //!< Condiciones por serie, repetición y estado.
    QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, QPair<double, double>>>>>> globalAngleRange; //!< Rango angular por línea.
    QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, double>>>>> globalAngleOverloads; //!< Sobrecarga angular.
    QHash<int, QString> stateNames; //!< Nombre de estados.
    QString logText; //!< Texto acumulado del log de condiciones.
    int idSesion = -1; //!< ID de la sesión actual.
};


#endif // SESIONREPORT_H
