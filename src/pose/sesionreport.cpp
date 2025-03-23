#include "sesionreport.h"

SesionReport::SesionReport() {}

void SesionReport::addCondition(int rep, const QString& keypointLine, ConditionType type) {
    repConditionCounts[rep][keypointLine][type]++;
}

int SesionReport::getConditionCount(int rep, const QString& keypointLine, ConditionType type) const {
    if (repConditionCounts.contains(rep) && repConditionCounts[rep].contains(keypointLine)) {
        return repConditionCounts[rep][keypointLine].value(type, 0);
    }
    return 0;
}

QHash<QString, QHash<ConditionType, int>> SesionReport::getRepetitionData(int rep) const {
    return repConditionCounts.value(rep, QHash<QString, QHash<ConditionType, int>>());
}

QList<int> SesionReport::getStoredRepetitions() const {
    return repConditionCounts.keys();
}

void SesionReport::reset() {
    repConditionCounts.clear();
}

// Método para contar el total de ocurrencias de un tipo de condición en todo el ejercicio

int SesionReport::getTotalOccurrences(ConditionType type) const {
    int count = 0;
    for (const auto& repData : repConditionCounts) {
        for (const auto& keypointData : repData) {
            count += keypointData.value(type, 0);
        }
    }
    return count;
}

// Método para obtener estadísticas agregadas de todas las condiciones detectadas en un ejercicio
QHash<ConditionType, int> SesionReport::getAggregatedStatistics() const {
    QHash<ConditionType, int> statistics;
    for (const auto& repData : repConditionCounts) {
        for (const auto& keypointData : repData) {
            for (auto it = keypointData.begin(); it != keypointData.end(); ++it) {
                statistics[it.key()] += it.value();
            }
        }
    }
    return statistics;
}

// // Método para imprimir estadísticas generales
// void SesionReport::printStatistics() const {
//     QHash<ConditionType, int> stats = getAggregatedStatistics();
//     qDebug() << "Resumen del ejercicio:";
//     for (auto it = stats.begin(); it != stats.end(); ++it) {
//         qDebug() << "Condición" << static_cast<int>(it.key()) << "detectada" << it.value() << "veces.";
//     }
// }
