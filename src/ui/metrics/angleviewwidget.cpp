#include "ui/metrics/angleviewwidget.h"
#include "ui/main/uitablemodel.h"
#include "utils/uiutils.h"
#include "utils/uiutils.h"



AngleViewerWidget::AngleViewerWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AngleViewWidget)
{
    ui->setupUi(this);

    connect(ui->seriesComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AngleViewerWidget::onSerieChanged);
    connect(ui->repsTable, &QTableView::clicked, this, &AngleViewerWidget::onRepSelected);
}

AngleViewerWidget::~AngleViewerWidget() {
    delete ui;
}



void AngleViewerWidget::setManager(MetricsManager* manager)
{
    metricsManager = manager;
    populateSeries();
}

void AngleViewerWidget::populateSeries()
{
    if (!metricsManager) return;

    QList<int> series = metricsManager->getReport().getStoredSeries();
    ui->seriesComboBox->clear();
    for (int s : series)
        ui->seriesComboBox->addItem(QString("Serie %1").arg(s), s);

    if (!series.isEmpty())
        onSerieChanged(0);
}

void AngleViewerWidget::onSerieChanged(int index)
{
    if (index < 0 || !metricsManager) return;

    currentSerie = ui->seriesComboBox->itemData(index).toInt();
    populateReps(currentSerie);
}

void AngleViewerWidget::populateReps(int serie)
{
    QList<int> reps = metricsManager->getReport().getStoredRepetitionsInSerie(serie);
    QList<QList<QVariant>> rows;
    for (int r : reps)
        rows << QList<QVariant>{r};

    auto* model = new UiTableModel(this);
    model->setDataSet(rows, {"Repetición"});
    ui->repsTable->setModel(model);
    ui->repsTable->resizeColumnsToContents();
}

void AngleViewerWidget::onRepSelected(const QModelIndex &index)
{
    if (!metricsManager || currentSerie < 0 || !index.isValid()) return;

    int rep = ui->repsTable->model()->data(ui->repsTable->model()->index(index.row(), 0)).toInt();
    populateOverloadsTable(currentSerie, rep);
    populateRangesTable(currentSerie, rep);
}

void AngleViewerWidget::populateOverloadsTable(int serie, int rep)
{
    if (!metricsManager) {
        qWarning() << " metricsManager es nullptr";
        return;
    }

    const auto& overloads = metricsManager->getReport().getGlobalAngleOverloads();
    const auto& stateNames = metricsManager->getReport().getStateNames();

    if (!overloads.contains(serie) || !overloads[serie].contains(rep)) {
        qWarning() << " Datos no encontrados para serie:" << serie << " rep:" << rep;
        return;
    }

    QList<QList<QVariant>> rows;
    QStringList headers{"Estado", "Vista", "Línea", "Valor"};

    auto stateMap = overloads.value(serie).value(rep);
    for (auto itState = stateMap.begin(); itState != stateMap.end(); ++itState) {
        QString stateName = stateNames.value(itState.key(), QString("S%1").arg(itState.key()));
        for (auto itView = itState.value().begin(); itView != itState.value().end(); ++itView) {
            QString viewStr = PoseViewToString(itView.key());
            for (auto itLine = itView.value().begin(); itLine != itView.value().end(); ++itLine) {
                rows << QList<QVariant>{stateName, viewStr, itLine.key(), itLine.value()};
            }
        }
    }

    auto* model = new UiTableModel(this);
    model->setDataSet(rows, headers);
    ui->overloadsTable->setModel(model);
   ui-> overloadsTable->resizeColumnsToContents();
}


void AngleViewerWidget::populateRangesTable(int serie, int rep)
{
    if (!metricsManager) {
        qWarning() << " metricsManager es nullptr";
        return;
    }

    const auto& ranges = metricsManager->getReport().getGlobalAngleRange();
    const auto& stateNames = metricsManager->getReport().getStateNames();

    if (!ranges.contains(serie) || !ranges[serie].contains(rep)) {
        qWarning() << " Datos no encontrados para serie:" << serie << " rep:" << rep;
        return;
    }

    QList<QList<QVariant>> rows;
    QStringList headers{"Estado", "Vista", "Línea", "Mínimo", "Máximo"};

    auto stateMap = ranges.value(serie).value(rep);
    for (auto itState = stateMap.begin(); itState != stateMap.end(); ++itState) {
        QString stateName = stateNames.value(itState.key(), QString("S%1").arg(itState.key()));
        for (auto itView = itState.value().begin(); itView != itState.value().end(); ++itView) {
            QString viewStr = PoseViewToString(itView.key());
            for (auto itLine = itView.value().begin(); itLine != itView.value().end(); ++itLine) {
                double minVal = itLine.value().first;
                double maxVal = itLine.value().second;
                rows << QList<QVariant>{stateName, viewStr, itLine.key(), minVal, maxVal};
            }
        }
    }

    auto* model = new UiTableModel(this);
    model->setDataSet(rows, headers);
    ui->rangesTable->setModel(model);
    ui->rangesTable->resizeColumnsToContents();
}
