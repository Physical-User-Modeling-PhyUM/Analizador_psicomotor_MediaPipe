#ifndef HEATMAPWIDGET_H
#define HEATMAPWIDGET_H

#include <QWidget>
#include <QMap>
#include <QGraphicsScene>
#include "core/metricsmanager.h"
#include "ui_heatmapwidget.h"

class HeatmapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeatmapWidget(QWidget *parent = nullptr);
    ~HeatmapWidget();
    void drawHeatmapForSerie(int serie);
public slots:
    void setManager(MetricsManager *manager);

private slots:
    void updateHeatmap();
    void onSerieChanged(int index);

private:
    Ui_HeatMapWidget *ui;
    MetricsManager *metricsManager = nullptr;
    QGraphicsScene *scene = nullptr;
    QList<int> seriesDisponibles;


    QColor getValue_Color(int value, int max);
};

#endif // HEATMAPWIDGET_H
