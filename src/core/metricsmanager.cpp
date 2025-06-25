#include "core/metricsmanager.h"

MetricsManager::MetricsManager() {}

void MetricsManager::setReport(const SesionReport& report) {
    this->report = report;
}
QHash<ConditionType, int> MetricsManager::getGlobalConditionCounts()  {
    QHash<ConditionType, int> counts;

    QHash<int, QHash<int, QHash<int, QHash<Condition, int> >>> series = report.getSeriesData();

    for (auto itSerie = series.begin(); itSerie != series.end(); ++itSerie) {

        for (auto itRep = itSerie.value().begin(); itRep != itSerie.value().end(); ++itRep) {
            for (auto itState = itRep.value().begin(); itState != itRep.value().end(); ++itState) {

                for (auto itCond = itState.value().begin(); itCond != itState.value().end(); ++itCond) {

                    ConditionType tipo=itCond.key().type;
                    counts[tipo] += itCond.value();
                }
            }
        }
    }
    return counts;
}

QHash<int, QHash<ConditionType, int>> MetricsManager::getConditionCountsBySerie() {
    QHash<int, QHash<ConditionType, int>> result;

    QHash<int, QHash<int, QHash<int, QHash<Condition, int> >>> series = report.getSeriesData();

    for (auto itSerie = series.begin(); itSerie != series.end(); ++itSerie) {
        int serie = itSerie.key();
        for (auto itRep = itSerie.value().begin(); itRep != itSerie.value().end(); ++itRep) {
            for (auto itState = itRep.value().begin(); itState != itRep.value().end(); ++itState) {
                for (auto itCond = itState.value().begin(); itCond != itState.value().end(); ++itCond) {
                    result[serie][itCond.key().type] += itCond.value();
                }
            }
        }
    }

    return result;
}

QHash<int, QHash<ConditionType, int>> MetricsManager::getConditionCountsByRepetition(int serie) {

    QHash<int, QHash<ConditionType, int>> result;
     QHash<int, QHash<int, QHash<int, QHash<Condition, int> >>> series = report.getSeriesData();
      QHash<int,QHash<int, QHash<Condition, int>>> reps = series.value(serie);

    for (auto itRep = reps.begin(); itRep != reps.end(); ++itRep) {
        int rep = itRep.key();
        for (auto itState = itRep.value().begin(); itState != itRep.value().end(); ++itState) {
            for (auto itCond = itState.value().begin(); itCond != itState.value().end(); ++itCond) {
                 ConditionType tipo=itCond.key().type;
                result[rep][tipo] += itCond.value();
            }
        }
    }

    return result;
}

QHash<QString, QHash<ConditionType, int>> MetricsManager::getCountsPerKeypointLine(int serie, int rep) {
  QHash<QString, QHash<ConditionType, int>>  result;
    QHash<int, QHash<int, QHash<int, QHash<Condition, int> >>> series = report.getSeriesData();
    QHash<int,QHash<int, QHash<Condition, int>>> reps = series.value(serie);
    QHash<int, QHash<Condition, int>> stateData = reps.value(rep);

    for (auto itState = stateData.begin(); itState != stateData.end(); ++itState) {
        for (auto itCond = itState.value().begin(); itCond != itState.value().end(); ++itCond) {
               ConditionType tipo=itCond.key().type;
            Condition cond = itCond.key();

            result[cond.keypointLine][tipo] += itCond.value();
        }
    }

    return result;
}


SesionReport MetricsManager::getReport()
{
    return report;
}


QMap<ConditionType, QMap<QString, int>> MetricsManager::getHeatmapData(int serie) {
    QMap<ConditionType, QMap<QString, int>> result;
    QHash<int, QHash<int, QHash<int, QHash<Condition, int>>>> series = report.getSeriesData();
    QHash<int, QHash<int, QHash<Condition, int>>> reps = series.value(serie);

    for (auto itRep = reps.begin(); itRep != reps.end(); ++itRep) {
        int rep = itRep.key();
        QHash<int, QHash<Condition, int>> states = itRep.value();

        for (auto itState = states.begin(); itState != states.end(); ++itState) {
            int state = itState.key();
            QHash<Condition, int> conditions = itState.value();

            // Rellenar con ceros para asegurar orden lexicográfico correcto
            QString key = QString("Rep%1_S%2")
                              .arg(rep, 2, 10, QChar('0'))
                              .arg(state, 2, 10, QChar('0'));

            for (auto itCond = conditions.begin(); itCond != conditions.end(); ++itCond) {
                Condition cond = itCond.key();
                int count = itCond.value();
                result[cond.type][key] += count;
            }
        }
    }

    return result;
}



QList<int> MetricsManager::getAvailableSeries() {
    return report.getSeriesData().keys();
}



QList<QList<QPair<QString, int>>> MetricsManager::getSegments(int serie) {

    QList<QList<QPair<QString, int>>> result;

    QHash<int, QHash<int, QHash<int, QHash<Condition, int> >>> series = report.getSeriesData();
    QHash<int,QHash<int, QHash<Condition, int>>> reps = series.value(serie);

    for (auto itRep = reps.begin(); itRep != reps.end(); ++itRep) {
        QList<QPair<QString, int>> repSegments;
          QHash<int, QHash<Condition, int> > states = itRep.value();

        for (auto itState = states.begin(); itState != states.end(); ++itState) {
             QHash<Condition, int> condMap = itState.value();

            for (auto itCond = condMap.begin(); itCond != condMap.end(); ++itCond) {
                 Condition cond = itCond.key();

                if (cond.type == ConditionType::EndOfMovementPhase) {
                    QString label = QString("Estado %1").arg(cond.keypointLine);
                    repSegments.append(qMakePair(label, cond.value.toInt()));
                }
                else if (cond.type == ConditionType::SetTime) {
                    repSegments.append(qMakePair(QString("SetTimeOver"), cond.value.toInt()));
                }
                else if (cond.type == ConditionType::RestOverTime) {
                    repSegments.append(qMakePair(QString("RestOverTime"), cond.value.toInt()));
                }
            }
        }

        result.append(repSegments);
    }

    return result;
}
QList<QList<QVariant>> MetricsManager::getSerieConditionDistribution(int serie) {
    QList<QList<QVariant>> result;

    QHash<int, QHash<ConditionType, int>> repData = getConditionCountsByRepetition(serie);

    for (auto itRep = repData.begin(); itRep != repData.end(); ++itRep) {
        int rep = itRep.key();
        for (auto itCond = itRep.value().begin(); itCond != itRep.value().end(); ++itCond) {
            QList<QVariant> row;
            row << QString::number(serie)
                << QString::number(rep)
                << "-"   // Estado (placeholder)
                << conditionTypeToString(itCond.key())
                << "-"   // Línea (placeholder)
                << "-"   // Valor (placeholder)
                << itCond.value(); // Count
            result.append(row);
        }
    }

    return result;
}
