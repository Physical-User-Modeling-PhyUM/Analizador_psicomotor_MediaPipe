/*!
 * \file sesionreport.cpp
 * \brief Implementación de la clase SesionReport.
 *
 * Esta clase permite registrar, almacenar y recuperar condiciones biomecánicas detectadas durante la ejecución de ejercicios.
 * Incluye soporte para condiciones categorizadas por serie, repetición y estado, además de rangos angulares y sobrecargas.
 */
#include "sesionreport.h"
/*!
 * \brief Constructor por defecto.
 */
SesionReport::SesionReport() {}


/*!
 * \brief Añade una condición a la estructura de datos de la sesión, organizándola por serie, repetición y estado.
 *
 * Si la condición ya existe, se actualiza su valor dependiendo del tipo:
 * - Se conserva el máximo, mínimo o se acumula el valor según el tipo de condición.
 * - Se incrementa el contador de ocurrencias.
 *
 * También se agrega una línea al log textual de eventos.
 *
 * \param serie Índice de la serie.
 * \param rep Índice de la repetición.
 * \param stateId ID del estado de la máquina al que pertenece la condición.
 * \param cond Objeto Condition con los detalles de la condición detectada.
 */
void SesionReport::addCondition(int serie, int rep, int stateId, Condition cond)
{
    auto& condMap = seriesData[serie][rep][stateId];

    // Buscar si ya existe una condición igual
    auto it = condMap.find(cond);
    if (it != condMap.end()) {

        switch (cond.type) {
         // Se guarda el mayor valor obtenido por serie,repeticion y estado
        case ConditionType::MaxAngle:
        case ConditionType::FastMovement:
        case ConditionType::SlowMovement:
        case ConditionType::symmetryDeviation:
        case ConditionType::ExerciseOvertime:
        case ConditionType::SetTime:
        case ConditionType::RestTime:
        case ConditionType::RestOverTime:
        case ConditionType::MaxStateTimeout:
        case ConditionType::MinStateTimeout:
            if (cond.value.toDouble() > it.key().value.toDouble()) {

                const_cast<Condition&>(it.key()).value = cond.value;
            }
            break;
        //En este caso se guarda el menor
        case ConditionType::MinAngle:
            if (cond.value.toDouble() < it.key().value.toDouble()) {
                const_cast<Condition&>(it.key()).value = cond.value;
            }
            break;

        case ConditionType::Increase:
        case ConditionType::Decrease:
        case ConditionType::OpositeDirection:
        case ConditionType::Not_Steady:
            if (cond.value.toDouble() > it.key().value.toDouble()) {
                const_cast<Condition&>(it.key()).value = cond.value;
            }
            break;

        case ConditionType::Has_Stopped:
        case ConditionType::is_Steady:
        {
            // Media: acumulador + count
            double sum = it.key().value.toMap().value("sum", 0.0).toDouble();
            int count = it.key().value.toMap().value("count", 0).toInt();

            sum += cond.value.toDouble();
            count++;

            QVariantMap updated;
            updated["sum"] = sum;
            updated["count"] = count;

            const_cast<Condition&>(it.key()).value = updated;
            break;
        }

        default:
            break;
        }
        //Incrementamos el contador de la condición
        ++it.value();

    } else {
        // No existe: insertar con contador 1
        condMap.insert(cond, 1);
    }

    QString stateName = stateNames.value(stateId, "unknown");
    logText += QString("{View: %1}--[Serie %2, Repetición %3, Estado %4 (%5)]--Tipo: %6, Línea: %7, valor: %8 \n")
                       .arg(PoseViewToString(cond.view))
                       .arg(serie)
                       .arg(rep)
                       .arg(stateId)
                       .arg(stateName)
                       .arg(conditionTypeToString(cond.type))
                       .arg(cond.keypointLine)
                       .arg(cond.value.toString());
}


/*!
 * \brief Asocia un nombre textual a un ID de estado.
 * \param idState ID numérico del estado.
 * \param name Nombre descriptivo a asignar.
 */
void SesionReport::setStateName(int idState, const QString& name)
    {
        stateNames[idState] = name;
    }

    /*!
 * \brief Obtiene todas las condiciones asociadas a una serie y repetición.
 * \param serie Serie deseada.
 * \param rep Repetición dentro de la serie.
 * \param expand Si es true, las condiciones se expanden tantas veces como su contador; si es false, se listan una sola vez.
 * \return Lista de objetos Condition.
 */
QList<Condition> SesionReport::getConditions(int serie, int rep, bool expand) const {
    QList<Condition> result;
    if (!seriesData.contains(serie) || !seriesData[serie].contains(rep))
        return result;

    const auto& condMap = seriesData[serie][rep];
    for (auto itState = condMap.begin(); itState != condMap.end(); ++itState) {
        const QHash<Condition, int>& conds = itState.value();
        for (auto itCond = conds.begin(); itCond != conds.end(); ++itCond) {
            const Condition& c = itCond.key();
            int count = itCond.value();

            if (expand) {
                for (int i = 0; i < count; ++i) {
                    result.append(c);
                }
            } else {
                result.append(c);
            }
        }
    }

    return result;
}

/*!
 * \brief Recupera las condiciones sin expandir junto a su estado correspondiente.
 * \param serie Serie objetivo.
 * \param rep Repetición objetivo.
 * \return Lista de pares <Condition, ID de estado>.
 */

 QList<QPair<Condition, int>> SesionReport::getRawConditions(int serie, int rep) const {
        QList<QPair<Condition, int>> list;

        if (!seriesData.contains(serie) || !seriesData[serie].contains(rep))
            return list;

         auto& condMap = seriesData[serie][rep];
        for (auto itState = condMap.begin(); itState != condMap.end(); ++itState) {
            int stateId = itState.key();

             QHash<Condition, int>conds = itState.value();
            for (auto itCond = conds.begin(); itCond != conds.end(); ++itCond) {
                list.append(qMakePair(itCond.key(), stateId));
            }
        }

        return list;
    }

 /*!
 * \brief Reinicia por completo los datos almacenados en el reporte.
 */

void SesionReport::reset() {
    seriesData.clear();
    globalAngleRange.clear();
}




/*!
 * \brief Devuelve el ID asociado a la sesión.
 * \return Entero representando el ID.
 */
int SesionReport::getIdSesion() const {
    return idSesion;
}
/*!
 * \brief Establece un nuevo ID para la sesión.
 * \param newIdSesion Nuevo identificador.
 */
void SesionReport::setIdSesion(int newIdSesion) {
    idSesion = newIdSesion;
}
/*!
 * \brief Devuelve la lista de series almacenadas en el reporte.
 * \return Lista de índices de series.
 */
QList<int> SesionReport::getStoredSeries() const {
    return seriesData.keys();
}
/*!
 * \brief Devuelve la lista de repeticiones almacenadas para una serie específica.
 * \param serie Serie a consultar.
 * \return Lista de repeticiones.
 */
QList<int> SesionReport::getStoredRepetitionsInSerie(int serie) const {
    return seriesData.value(serie).keys();
}


/*!
 * \brief Asigna los rangos angulares globales para las líneas detectadas.
 * \param data Estructura anidada por serie, repetición, estado, vista, línea.
 */
void SesionReport::setLineAngleRange(const QMap<int,QMap<int,QMap<int,QHash<PoseView,QHash<QString, QPair<double, double>>>>>> & data) {
    globalAngleRange = data;
}

// QPair<double, double> SesionReport::getGlobalRange(const QString& line) const {
//     return globalAngleRange.value(line, qMakePair(0.0, 0.0));
// }

// QStringList SesionReport::getLinesWithGlobalRange() const {
//     return globalAngleRange.keys();
// }

/*!
 * \brief Serializa las condiciones de la sesión en formato JSON.
 * \return Objeto JSON estructurado por serie, repetición, estado y condición.
 */
nlohmann::json SesionReport::toSeriesJson() const
{
    nlohmann::json json;

    for (auto itSerie = seriesData.begin(); itSerie != seriesData.end(); ++itSerie) {
        int serie = itSerie.key();

        for (auto itRep = itSerie.value().begin(); itRep != itSerie.value().end(); ++itRep) {
            int rep = itRep.key();

            for (auto itState = itRep.value().begin(); itState != itRep.value().end(); ++itState) {
                int state = itState.key();

                QString stateName = stateNames.value(state, "Unknown");

                std::string stateKey = QString("%1|%2").arg(state).arg(stateName).toStdString();

                for (auto itCond = itState.value().begin(); itCond != itState.value().end(); ++itCond) {
                     Condition cond = itCond.key();
                    int count = itCond.value();

                    nlohmann::json j;

                    j["type"] = conditionTypeToString(cond.type).toStdString();
                    j["line"] = cond.keypointLine.toStdString();
                    j["view"] = PoseViewToString(cond.view).toStdString();
                    j["count"] = count;


                    if (cond.value.type() == QVariant::Double) {
                        j["value"] = cond.value.toDouble();

                    } else if (cond.value.type() == QVariant::Map) {

                        QVariantMap map = cond.value.toMap();
                        nlohmann::json jm;

                        for (auto it = map.begin(); it != map.end(); ++it) {
                            double val=it.value().toDouble();
                            jm[it.key().toStdString()] = val;
                        }
                        j["value"] = jm;
                    } else {
                        j["value"] = cond.value.toString().toStdString();
                    }

                    json[std::to_string(serie)][std::to_string(rep)][stateKey].push_back(j);
                }
            }
        }
    }

    return json;
}


/*!
 * \brief Serializa los rangos angulares globales en formato JSON.
 * \return Objeto JSON con rangos por serie, repetición, estado, vista y línea.
 */
nlohmann::json SesionReport::toGlobalAnglesJson() const {
    nlohmann::json json;

    for (auto itSerie = globalAngleRange.begin(); itSerie != globalAngleRange.end(); ++itSerie) {
        int serie = itSerie.key();
        for (auto itRep = itSerie.value().begin(); itRep != itSerie.value().end(); ++itRep) {
            int rep = itRep.key();

            for (auto itState = itRep.value().begin(); itState != itRep.value().end(); ++itState) {

                int state = itState.key();

                for (auto itView = itState.value().begin(); itView != itState.value().end(); ++itView) {

                    QString viewStr = PoseViewToString(itView.key());

                    for (auto itLine = itView.value().begin(); itLine != itView.value().end(); ++itLine) {
                        QString line = itLine.key();
                        double minVal = itLine.value().first;
                        double maxVal = itLine.value().second;
                         //json[serie][rep][std::to_string(state)][line.toStdString()] = { minVal, maxVal };
                        json[std::to_string(serie)][std::to_string(rep)][std::to_string(state)][viewStr.toStdString()][line.toStdString()] = { minVal, maxVal };
                    }
                }
            }
        }
    }

    return json;
}


/*!
 * \brief Deserializa el JSON que contiene condiciones organizadas por serie, repetición y estado.
 * \param json Objeto JSON con condiciones a cargar.
 */
void SesionReport::fromSeriesJson(const nlohmann::json& json)
{
    seriesData.clear();
    stateNames.clear();

    // Recorre todas las series
    for (auto serieIt = json.begin(); serieIt != json.end(); ++serieIt) {

        int serie = QString::fromStdString(serieIt.key()).toInt();

        auto serieJson = serieIt.value();

        // Recorre todas las repetciones de la serie
        for (auto repIt = serieJson.begin(); repIt != serieJson.end(); ++repIt) {
            //repIt.key();
            int rep = QString::fromStdString(repIt.key()).toInt();
            auto repJson = repIt.value();

            // Recorre cada estao
            for (auto stateIt = repJson.begin(); stateIt != repJson.end(); ++stateIt) {
                QString stateText = QString::fromStdString(stateIt.key());
                QStringList stateParts = stateText.split("|");

                int stateId = stateParts.value(0).toInt();
                QString stateName = stateParts.value(1, "Unknown");
                stateNames[stateId] = stateName;

                // Recorre cad con dición registrada en ese estado
                for (const auto& condJson : stateIt.value()) {

                    QString typeStr = QString::fromStdString(condJson["type"]);
                    QString lineStr = QString::fromStdString(condJson["line"]);
                    QString viewStr = QString::fromStdString(condJson["view"]);

                    int count = condJson["count"];

                    QVariant value;

                    if (condJson.contains("value")) {
                        if (condJson["value"].is_number()) {
                            value = condJson["value"].get<double>();
                        } else if (condJson["value"].is_object()) {
                            QVariantMap map;


                            for (const auto& i : condJson["value"].items()) {
                                QString key = QString::fromStdString(i.key());
                                double val = i.value().get<double>();
                                map[key] = val;
                            }
                            value = map;
                        } else {
                            value = QString::fromStdString(condJson["value"].get<std::string>());
                        }
                    }

                    Condition cond(conditionTypeFromString(typeStr), lineStr, value, PoseViewFromString(viewStr));
                    //seriesData[serie][rep][stateId] = count;
                    seriesData[serie][rep][stateId][cond] = count;
                }
            }
        }
    }
}


/*!
 * \brief Deserializa el JSON que contiene rangos angulares globales.
 * \param json Objeto JSON con rangos por estado y vista.
 */
void SesionReport::fromGlobalAnglesJson(const nlohmann::json& json) {
    globalAngleRange.clear();

    for (auto& serieIt : json.items()) {
        int serie = QString::fromStdString(serieIt.key()).toInt();

        for (auto& repIt : serieIt.value().items()) {
            int rep = QString::fromStdString(repIt.key()).toInt();

            for (auto& stateIt : repIt.value().items()) {
                int state = QString::fromStdString(stateIt.key()).toInt();

                for (auto& viewIt : stateIt.value().items()) {
                    PoseView view = PoseViewFromString(QString::fromStdString(viewIt.key()));

                    for (auto& lineIt : viewIt.value().items()) {
                        QString line = QString::fromStdString(lineIt.key());

                        const auto& arr = lineIt.value();

                        if (arr.is_array() && arr.size() == 2) {
                            double minVal = arr[0].get<double>();
                            double maxVal = arr[1].get<double>();
                            globalAngleRange[serie][rep][state][view][line] = qMakePair(minVal, maxVal);
                        } else {
                            qWarning() << "[fromGlobalAnglesJson] Formato inválido para línea:"
                                       << line << " en serie:" << serie << " rep:" << rep;
                        }
                    }
                }
            }
        }
    }
}

/*!
 * \brief Serializa en JSON los valores de sobrecarga angular global detectados.
 * \return Objeto JSON organizado por serie, repetición, estado, vista y línea.
 */
nlohmann::json SesionReport::toGlobalOverloadsJson() const {
    nlohmann::json json;
    int serie;
    int state;
    int rep;
    PoseView view;
    QString line,viewStr ;
    double value,maxVal;

    for (auto itSerie = globalAngleOverloads.begin(); itSerie != globalAngleOverloads.end(); ++itSerie) {
         serie = itSerie.key();
        for (auto itRep = itSerie.value().begin(); itRep != itSerie.value().end(); ++itRep) {
             rep = itRep.key();
            auto& stateMap = itRep.value();
            for (auto itState = stateMap.begin(); itState != stateMap.end(); ++itState){
                 state = itState.key();
                for (auto itView = itState.value().begin(); itView != itState.value().end(); ++itView) {
                     viewStr = PoseViewToString(itView.key());
                    for (auto itLine = itView.value().begin(); itLine != itView.value().end(); ++itLine) {
                         line = itLine.key();
                        value = itLine.value();
                        json[std::to_string(serie)][std::to_string(rep)][std::to_string(state)][viewStr.toStdString()][line.toStdString()] = value;
                    }
                }
            }
        }
    }

    return json;
}
/*!
 * \brief Deserializa los valores de sobrecarga desde un objeto JSON.
 * \param json Objeto JSON con datos de sobrecarga.
 */
void SesionReport::fromGlobalOverloadsJson(const nlohmann::json& json) {
    globalAngleOverloads.clear();

    for (auto& serieIt : json.items()) {
        int serie = QString::fromStdString(serieIt.key()).toInt();

        for (auto& repIt : serieIt.value().items()) {
            int rep = QString::fromStdString(repIt.key()).toInt();

            for (auto& stateIt : repIt.value().items()) {
                int state = QString::fromStdString(stateIt.key()).toInt();

                for (auto& viewIt : stateIt.value().items()) {
                    PoseView view = PoseViewFromString(QString::fromStdString(viewIt.key()));

                    for (auto& lineIt : viewIt.value().items()) {
                        QString line = QString::fromStdString(lineIt.key());

                        const auto& val = lineIt.value();
                        if (val.is_number()) {
                            double value = val.get<double>();
                            globalAngleOverloads[serie][rep][state][view][line] = value;
                        } else {
                            qWarning() << "[fromGlobalOverloadsJson] Formato inválido para línea:"
                                       << line << " en serie:" << serie << " rep:" << rep;
                        }
                    }
                }
            }
        }
    }
}

/*!
 * \brief Devuelve el texto completo del log de eventos generados durante la sesión.
 * \return Cadena de texto acumulado.
 */
QString SesionReport::getLog() const
{
    return logText;
}
/*!
 * \brief Establece el contenido del log de eventos.
 * \param newLog Texto de log a asignar.
 */
void SesionReport::setLog(const QString &newLog)
{
    logText = newLog;
}
/*!
 * \brief Devuelve la estructura completa de condiciones registradas.
 * \return Mapa anidado [serie][repetición][estado][condición].
 */
QHash<int, QHash<int, QHash<int, QHash<Condition, int> > > > SesionReport::getSeriesData() const
{
    return seriesData;
}

/*!
 * \brief Devuelve una representación textual detallada de todas las condiciones registradas.
 * \return Cadena con la información formateada.
 */
QString SesionReport::printSeriesData() const
{
    QString str="";
    if (seriesData.isEmpty()) {
        return "No hay datos en seriesData.\n";
    }
    for (auto itSerie = seriesData.constBegin(); itSerie != seriesData.constEnd(); ++itSerie) {
        int serie = itSerie.key();
         auto repsMap = itSerie.value();


        for (auto itRep = repsMap.constBegin(); itRep != repsMap.constEnd(); ++itRep) {
            int rep = itRep.key();
             auto statesMap = itRep.value();


            for (auto itState = statesMap.constBegin(); itState != statesMap.constEnd(); ++itState) {
                int stateId = itState.key();
                 QHash<Condition,int> condMap = itState.value();
                QString stateName = stateNames.value(stateId, "Unknown");


                for (auto itCond = condMap.constBegin(); itCond != condMap.constEnd(); ++itCond) {
                     Condition c = itCond.key();
                    int count = itCond.value();

                    QVariant val = c.value;
                    QString view  = PoseViewToString(c.view);
                    QString type  = conditionTypeToString(c.type);
                    QString line  = c.keypointLine;

                     str+= QString("Serie %1 | Rep %2 | Estado %3 (%4) | Tipo: %5 | Línea: %6 | View: %7 | Valor: %8 | Count: %9 /n")
                                    .arg(serie)
                                    .arg(rep)
                                    .arg(stateId)
                                    .arg(stateName)
                                    .arg(type)
                                    .arg(line)
                                    .arg(view)
                                    .arg(val.toString())
                                    .arg(count);
                }
            }
        }
    }
    return str;
}
/*!
 * \brief Devuelve la tabla de nombres de estado registrados.
 * \return Hash con ID de estado como clave y nombre como valor.
 */
QHash<int, QString> SesionReport::getStateNames() const
{
    return stateNames;
}
/*!
 * \brief Asigna todos los nombres de estado.
 * \param newStateNames Hash con los nombres de estado.
 */
void SesionReport::setStateNames(const QHash<int, QString> &newStateNames)
{
    stateNames = newStateNames;
}
/*!
 * \brief Devuelve los rangos globales angulares.
 * \return Mapa anidado por serie, repetición, estado, vista, línea.
 */
QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, QPair<double, double> > > > > > SesionReport::getGlobalAngleRange() const
{
    return globalAngleRange;
}
/*!
 * \brief Devuelve las sobrecargas angulares registradas.
 * \return Mapa anidado por serie, repetición, estado, vista, línea.
 */
QMap<int, QMap<int, QMap<int, QHash<PoseView, QHash<QString, double> > > > > SesionReport::getGlobalAngleOverloads() const
{
    return globalAngleOverloads;
}
/*!
 * \brief Establece los valores de sobrecarga angular.
 * \param newGlobalAngleOverloads Estructura de datos completa.
 */
void SesionReport::setGlobalAngleOverloads(const QMap<int, QMap<int, QMap<int,QHash<PoseView, QHash<QString, double> >>>> &newGlobalAngleOverloads)
{
    globalAngleOverloads = newGlobalAngleOverloads;
}
/*!
 * \brief Devuelve los encabezados de columna para representar el log en una vista tabular.
 * \return Lista de nombres de columna.
 */
QStringList SesionReport::getLogColumnHeaders() const {
    return { "View", "Serie", "Repetición", "Estado", "Texto Completo" };
}
/*!
 * \brief Parsea el log acumulado en filas tabulares para mostrar en tablas de UI.
 * \return Lista de filas, donde cada fila es una lista de celdas QVariant.
 */
QList<QList<QVariant>> SesionReport::parseLog() const {
    QList<QList<QVariant>> rows;

    QStringList lines = logText.split("\n", Qt::SkipEmptyParts);

    for (const QString& line : lines) {
        QString trimmedLine = line.trimmed();

        QString view = "";
        QString serie = "";
        QString rep = "";
        QString state = "";
        //forato en el log
        //{View: Front}--[Serie 1, Repetición 1, Estado 0 (unknown)]-

        int viewStart = trimmedLine.indexOf("{View: ");
        int viewEnd = trimmedLine.indexOf("}", viewStart);
        if (viewStart != -1 && viewEnd != -1) {
            view = trimmedLine.mid(viewStart + 7, viewEnd - (viewStart + 7)).trimmed();
        }

        int serieStart = trimmedLine.indexOf("[Serie ");
        int serieEnd = trimmedLine.indexOf("]", serieStart);

        if (serieStart != -1 && serieEnd != -1) {
            QString insideBrackets = trimmedLine.mid(serieStart + 1, serieEnd - serieStart - 1); // sin los corchetes

            // dentro de los brackets debería ser: "Serie 1, Repetición 1, Estado 0 (unknown)"
            QStringList parts = insideBrackets.split(",");
            for (const QString& part : parts) {
                QString clean = part.trimmed();

                if (clean.startsWith("Serie ")) {
                    serie = clean.mid(6).trimmed();
                } else if (clean.startsWith("Repetición ")) {
                    rep = clean.mid(11).trimmed();
                } else if (clean.startsWith("Estado ")) {
                    QString tmp = clean.mid(7).trimmed();
                    int spaceIndex = tmp.indexOf(" ");
                    if (spaceIndex > 0) {
                        state = tmp.left(spaceIndex);
                    } else {
                        state = tmp;
                    }
                }
            }
        }
        // Eliminamos la parte referente a los indices : "{View...]}--"
         int sepIndex = trimmedLine.indexOf("]--");
         QString simplifiedText = (sepIndex != -1) ? trimmedLine.mid(sepIndex + 3).trimmed() : trimmedLine;

         rows.append({ view, serie, rep, state, simplifiedText });

        //rows.append({ view, serie, rep, state, trimmedLine });
    }

    return rows;
}

