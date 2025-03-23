#ifndef SESIONREPORT_H
#define SESIONREPORT_H

#include <QHash>
#include <QString>
#include <QList>
#include "condition.h"

class SesionReport
{
public:
    SesionReport();
    void addCondition(int rep, const QString& keypointLine, ConditionType type);
    int getConditionCount(int rep, const QString& keypointLine, ConditionType type) const;
    QHash<QString, QHash<ConditionType, int>> getRepetitionData(int rep) const;
    QList<int> getStoredRepetitions() const;
    void reset();


    int getTotalOccurrences(ConditionType type) const;
    QHash<ConditionType, int> getAggregatedStatistics() const;
    void printStatistics() const;

private:
    QHash<int, QHash<QString, QHash<ConditionType, int>>> repConditionCounts;

};

#endif // SESIONREPORT_H
