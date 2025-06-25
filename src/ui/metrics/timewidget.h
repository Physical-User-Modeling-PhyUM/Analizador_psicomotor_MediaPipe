#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QChartView>
#include <QTableWidget>
#include <QSharedPointer>
#include "pose/sesionreport.h"
#include <QLoggingCategory>


Q_DECLARE_LOGGING_CATEGORY(timeWidget)


    class TimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimeWidget(QWidget *parent = nullptr);
    void setReport(const SesionReport& rep);
    void setRepetitionSegments(const QList<QList<QPair<QString, int>>>& segments);


private slots:
    void onSerieChanged(int index);

private:
    void setSeriesList(const QStringList& seriesNames);
    void setSerieMetrics();

    void updateChart();
    void updateTable();

    QComboBox* serieSelector;
    QLabel* labelDuration;
    QLabel* labelRest;
    QLabel* labelOverSerie;
    QLabel* labelOverRest;
    QChart* chart;
    QChartView* chartView;
    QTableWidget* stateTimes;
    QList<QList<QPair<QString, int>>> currentSegments;
    QSharedPointer<SesionReport> report;
    QHash<int, QString> stateNames;
};

#endif // TIMEWIDGET_H
