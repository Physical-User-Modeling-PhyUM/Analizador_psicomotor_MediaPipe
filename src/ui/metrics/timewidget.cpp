#include "timewidget.h"
#include <QtCharts/QBarSet>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHeaderView>

Q_LOGGING_CATEGORY(timeWidget, "timeWidget");

TimeWidget::TimeWidget(QWidget *parent) : QWidget(parent)
{
    serieSelector = new QComboBox();
    connect(serieSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TimeWidget::onSerieChanged);

    labelDuration = new QLabel("Duración serie: ");
    //labelRest = new QLabel("Rest time: ");
    labelOverSerie = new QLabel("Overtime serie: ");
    labelOverRest = new QLabel("Overtime descanso: ");

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Serie:", serieSelector);
    formLayout->addRow(labelDuration);
    //formLayout->addRow(labelRest);
    formLayout->addRow(labelOverSerie);
    formLayout->addRow(labelOverRest);

    chart = new QChart();
    chart->setTitle("Duración por estado en cada repetición");
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    stateTimes = new QTableWidget();

    QVBoxLayout* layOut = new QVBoxLayout(this);
    layOut->addLayout(formLayout);
    layOut->addWidget(chartView);
    layOut->addWidget(stateTimes);
}

void TimeWidget::setSeriesList(const QStringList& seriesN)
{
    serieSelector->clear();
    serieSelector->addItems(seriesN);
}

// void TimeWidget::setRepetitionSegments(const QList<QList<QPair<QString, int>>>& segments)
// {
//     currentSegments = segments;
//     updateChart();
// }

void TimeWidget::setRepetitionSegments(const QList<QList<QPair<QString, int>>>& data)
{
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    QStringList categories;
    QStackedBarSeries* series = new QStackedBarSeries;
    QMap<QString, QVector<qreal>> stateDurations;

    int numReps = data.size();
    qreal maxY = 0.0;

    for (int i = 0; i < numReps; ++i) {
        categories << QString("Rep %1").arg(i + 1);

        QMap<QString, qreal> repDur;
        for (const auto& segment : data[i]) {
            repDur[segment.first] += segment.second / 1000.0;
        }

        for (const QString& state : repDur.keys()) {
            if (!stateDurations.contains(state)) {
                stateDurations[state] = QVector<qreal>(numReps, 0.0);
            }
            stateDurations[state][i] = repDur[state];
        }

        qreal totalRep = std::accumulate(repDur.begin(), repDur.end(), 0.0,
                                         [](qreal acc, qreal val) { return acc + val; });
        maxY = std::max(maxY, totalRep);
    }


    for (auto it = stateDurations.begin(); it != stateDurations.end(); ++it) {
        QBarSet* barSet = new QBarSet(it.key());
        for (qreal dur : it.value()) {
            *barSet << dur;
        }
        series->append(barSet);
    }

    chart->addSeries(series);
    // Antes de crear los nuevos ejes en setRepetitionSegments


    // Eje X
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->clear();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Eje Y adaptado
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Duración (segundos)");
    axisY->setRange(0, std::ceil(maxY + 1));
    axisY->setTickCount(6);
    chart->removeAxis(axisY);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    series->setLabelsVisible(true);
    series->setLabelsFormat("@value s");

}



void TimeWidget::updateChart()
{
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    QStackedBarSeries* series = new QStackedBarSeries();
    QMap<QString, QBarSet*> sets;

    int numReps = currentSegments.size();
    QStringList categories;

    for (int repIdx = 0; repIdx < numReps; ++repIdx) {
        const auto& segmentsForRep = currentSegments[repIdx];
        categories << QString("Rep %1").arg(repIdx + 1);

        for (const auto& seg : segmentsForRep) {
            const QString& estado = seg.first;
            int duracion = seg.second / 1000; // milisegundos a segundos

            if (!sets.contains(estado))
                sets[estado] = new QBarSet(estado);

            while (sets[estado]->count() < repIdx)
                *(sets[estado]) << 0;

            *(sets[estado]) << duracion;
        }
    }

    for (auto set : sets)
        series->append(set);

    chart->addSeries(series);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axisX, series);
}



void TimeWidget::setReport(const SesionReport& rep)
{
    report = QSharedPointer<SesionReport>::create(rep);
    QStringList names;
    stateNames=report->getStateNames();
    QList<int> series = report->getStoredSeries();
    for (int serieId : series)
        names << QString("Serie %1").arg(serieId);

    setSeriesList(names);

    if (!names.isEmpty())
        onSerieChanged(0);
}



void TimeWidget::onSerieChanged(int index)
{
    if (!report || index < 0 || index >= report->getStoredSeries().size())
        return;

    int serieId = report->getStoredSeries()[index];
    const auto& reps = report->getSeriesData().value(serieId);
    const auto& stateNames = report->getStateNames();

    QList<QList<QPair<QString, int>>> segments;
    QMap<int, QMap<int, int>> maxTimeouts;
    QMap<int, QSet<int>> hasMinTimeout;

    // QMap<int, QMap<int, int>> serieOverDurations;
    // QMap<int, QMap<int, int>> repOverDurations;
    QSet<int> states;

    qint64 totalOverSetTime = 0;
    qint64 totalRestTime = 0;
    qint64 workingTime=0;
    // qint64 initTime = -1;
    // qint64 endTime = -1;


    for (auto itRep = reps.begin(); itRep != reps.end(); ++itRep) {
        int repIndex = itRep.key();
        const auto& statesMap = itRep.value();
        QList<QPair<QString, int>> segmentList;

        for (auto itState = statesMap.begin(); itState != statesMap.end(); ++itState) {
            int stateId = itState.key();
            const auto& conds = itState.value();
            states.insert(stateId);

            for (const auto& [cond, _] : conds.asKeyValueRange()) {
                if (cond.type == ConditionType::EndOfMovementPhase) {
                    QString name = stateNames.value(stateId);
                    if (name.isEmpty() || name == "Unknown")
                        name = QString("Estado %1").arg(stateId);
                    segmentList.append(qMakePair(name, cond.value.toInt()));
                    //qDebug(timeWidget) << " Serie:" << serieId << "Repetición:" << repIndex << "Estado:" << stateId << "Tiempo(ms):" << cond.value;

                }
                // if (cond.type == ConditionType::exerciseInit)
                //     {initTime = cond.value.toLongLong();}
                // if (cond.type == ConditionType::EndOfExercise)
                //     {endTime = cond.value.toLongLong();}
                if (cond.type == ConditionType::SetTime)
                {
                    if (cond.value.toInt()>totalOverSetTime)  totalOverSetTime = cond.value.toInt();
                    //qDebug(timeWidget)<<"Repeticion"<<repIndex<<" Estado"<<cond.keypointLine.toInt()<<"SetTime"<<totalOverSetTime;
                }

                if (cond.type == ConditionType::EndOfRepetition){
                    //qDebug(timeWidget)<<"Repeticion"<<repIndex<<" Estado"<<cond.keypointLine.toInt()<<"EndOfRepetition"<<cond.value.toInt();
                    workingTime +=cond.value.toInt();
                }
                if (cond.type == ConditionType::RestOverTime)
                {
                    if (cond.value.toInt()>totalRestTime)  totalRestTime = cond.value.toInt();
                     //qDebug(timeWidget)<<"Repeticion"<<repIndex<<" Estado"<<cond.keypointLine.toInt()<<"RestOverTime"<<totalRestTime;
                }
                if (cond.type == ConditionType::MaxStateTimeout){
                    //if( maxTimeouts[repIndex][cond.keypointLine.toInt()]<cond.value.toInt()){maxTimeouts[repIndex][cond.keypointLine.toInt()] = cond.value.toInt();}
                    if (!maxTimeouts[repIndex].contains(cond.keypointLine.toInt()) || maxTimeouts[repIndex][cond.keypointLine.toInt()] < cond.value.toInt()) {
                        maxTimeouts[repIndex][cond.keypointLine.toInt()] = cond.value.toInt();
                    }
                    //qDebug(timeWidget)<<"Repeticion"<<repIndex<<" Estado"<<cond.keypointLine.toInt()<<"valor"<<cond.value.toInt()<<"guardado:"<<maxTimeouts[repIndex][cond.keypointLine.toInt()];
                 }
                if (cond.type == ConditionType::MinStateTimeout)
                 { hasMinTimeout[repIndex].insert(cond.keypointLine.toInt());}
            }
        }
        segments.append(segmentList);
    }

    setRepetitionSegments(segments);

    // if (initTime > 0 && endTime > initTime) {
    //     labelDuration->setText(QString("Duración serie: %1 s").arg((endTime - initTime) / 1000));
    // } else {
    //     labelDuration->setText("Duración serie: no finalizada");
    // }


    labelDuration->setText(QString("Serie working time: %1 s").arg(workingTime / 1000));
    labelOverSerie->setText(QString("Overtime serie: %1 s").arg(totalOverSetTime / 1000));
    labelOverRest->setText(QString("Overtime descanso: %1 s").arg(totalRestTime / 1000));

    // Tabla de tiempos
    stateTimes->clear();
    QStringList headers;
    for (int i = 0; i < reps.size(); ++i)
        headers << QString("Rep %1").arg(i + 1);
    stateTimes->setColumnCount(headers.size());
    stateTimes->setHorizontalHeaderLabels(headers);

    QStringList rowLabels;
    QList<int> sortedStates = states.values();
    std::sort(sortedStates.begin(), sortedStates.end());

    stateTimes->setRowCount(sortedStates.size());

    for (int row = 0; row < sortedStates.size(); ++row) {
        int stateId = sortedStates[row];
        QString name = stateNames.value(stateId);
        if (name.isEmpty() || name == "Unknown")
            name = QString("Estado %1").arg(stateId);
        rowLabels << name;
    }

    stateTimes->setVerticalHeaderLabels(rowLabels);


    for (int rep = 0; rep < headers.size(); ++rep) {
        for (int row = 0; row < sortedStates.size(); ++row) {
            int stateId = sortedStates[row];
            if (maxTimeouts.contains(rep) && maxTimeouts[rep].contains(stateId)) {
                qreal val = (maxTimeouts[rep][stateId])/1000;
                //qDebug(timeWidget)<<" dibujado-> Repeticion"<<rep<<" Estado"<<stateId<<"valor"<< maxTimeouts[rep][stateId]<<"val "<<val;
                QTableWidgetItem* item = new QTableWidgetItem(QString::number(val, 'f', 2));
                Qt::GlobalColor color;
                if (val>0){color=Qt::red;}
                else if ( hasMinTimeout[rep].contains(stateId)) color= Qt::green;
                else color=Qt::yellow;
                item->setBackground(color);

                stateTimes->setItem(row, rep, item);
            }
        }
    }
    stateTimes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    stateTimes->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
