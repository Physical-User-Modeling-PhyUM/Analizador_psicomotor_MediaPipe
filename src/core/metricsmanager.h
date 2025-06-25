#ifndef METRICSMANAGER_H
#define METRICSMANAGER_H

#include "pose/sesionreport.h"
#include <QHash>
#include <QString>
#include <QMap>
#include <QSet>

class MetricsManager
{
public:
    MetricsManager();

    void setReport(const SesionReport& report);



    QHash<ConditionType, int> getGlobalConditionCounts();
    QHash<PoseView, QHash<ConditionType, int> > getConditionCountsByView();
    QHash<int, QHash<ConditionType, int> > getConditionCountsBySerie();
    QHash<int, QHash<ConditionType, int> > getConditionCountsByRepetition(int serie);
    QHash<QString, QHash<ConditionType, int> > getCountsPerKeypointLine(int serie, int rep);
    SesionReport getReport();
    QList<QList<QVariant>> getSerieConditionDistribution(int serie);
    QMap<ConditionType, QMap<QString, int> > getHeatmapData(int serie);
    QList<int> getAvailableSeries();
    QList<QList<QPair<QString, int>>> getSegments(int serie);


private:
    SesionReport report;
};

#endif // METRICSMANAGER_H
