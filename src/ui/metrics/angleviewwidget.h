#ifndef ANGLEVIEWERWIDGET_H
#define ANGLEVIEWERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QTableView>
#include <QVBoxLayout>
#include <QLabel>
#include "core/metricsmanager.h"
#include "ui_angleviewwidget.h"

class AngleViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AngleViewerWidget(QWidget *parent = nullptr);
    ~AngleViewerWidget();

    void setManager(MetricsManager* manager);

private slots:
    void onSerieChanged(int index);
    void onRepSelected(const QModelIndex &index);

private:
    void populateSeries();
    void populateReps(int serie);
    void populateOverloadsTable(int serie, int rep);
    void populateRangesTable(int serie, int rep);

    MetricsManager* metricsManager = nullptr;
    Ui_AngleViewWidget* ui;

    int currentSerie = -1;
};

#endif // ANGLEVIEWERWIDGET_H
