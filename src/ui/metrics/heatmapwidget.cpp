#include "heatmapwidget.h"
#include "ui_heatmapwidget.h"

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QtMath>

HeatmapWidget::HeatmapWidget(QWidget *parent)
    : QWidget(parent),
    ui(new   Ui_HeatMapWidget),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    connect(ui->comboSerieSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &HeatmapWidget::onSerieChanged);
}

HeatmapWidget::~HeatmapWidget()
{
    delete ui;
}

void HeatmapWidget::setManager(MetricsManager *manager)
{
    metricsManager = manager;
    seriesDisponibles = metricsManager->getAvailableSeries();

    ui->comboSerieSelector->clear();
    for (int serie : seriesDisponibles) {
        ui->comboSerieSelector->addItem(QString("Serie %1").arg(serie), serie);
    }

    if (!seriesDisponibles.isEmpty()) {
        drawHeatmapForSerie(seriesDisponibles.first());
    }
}

void HeatmapWidget::updateHeatmap() {
    int index = ui->comboSerieSelector->currentIndex();
    if (index >= 0) {
        int serie = ui->comboSerieSelector->itemData(index).toInt();
        drawHeatmapForSerie(serie);
    }
}


void HeatmapWidget::onSerieChanged(int index)
{
    if (index < 0 || !metricsManager) return;
    int serie = ui->comboSerieSelector->itemData(index).toInt();
    drawHeatmapForSerie(serie);
}

// void HeatmapWidget::drawHeatmapForSerie(int serie)
// {
//     scene->clear();

//     QMap<ConditionType, QMap<QString, int>> matrix = metricsManager->getHeatmapData(serie);

//     QStringList repsEtapas;
//     for (const auto& perCond : matrix) {
//         for (const QString& repKey : perCond.keys()) {
//             if (!repsEtapas.contains(repKey)) {
//                 repsEtapas.append(repKey);
//             }
//         }
//     }

//     std::sort(repsEtapas.begin(), repsEtapas.end());

//     QStringList condiciones;
//     for (ConditionType type : matrix.keys()) {
//         condiciones.append(conditionTypeToString(type));
//     }

//     int cellW = 50;
//     int cellH = 30;
//     int margin = 80;

//     int maxValue = 1;
//     for (const auto& row : matrix) {
//         for (int val : row.values()) {
//             if (val > maxValue) maxValue = val;
//         }
//     }

//     for (int i = 0; i < condiciones.size(); ++i) {
//         scene->addText(condiciones[i])->setPos(0, margin + i * cellH);
//     }

//     for (int j = 0; j < repsEtapas.size(); ++j) {
//         scene->addText(repsEtapas[j])->setPos(margin + j * cellW, 0);
//     }

//     for (int i = 0; i < condiciones.size(); ++i) {
//         ConditionType cond = conditionTypeFromString(condiciones[i]);

//         for (int j = 0; j < repsEtapas.size(); ++j) {
//             int value = matrix[cond].value(repsEtapas[j], 0);
//             QColor color = getValue_Color(value, maxValue);

//             QGraphicsRectItem *rect = scene->addRect(margin + j * cellW,
//                                                      margin + i * cellH,
//                                                      cellW, cellH,
//                                                      QPen(Qt::black), QBrush(color));

//             if (value > 0) {
//                 QGraphicsTextItem *text = scene->addText(QString::number(value));
//                 text->setPos(margin + j * cellW + 10, margin + i * cellH + 5);
//             }
//         }
//     }
// }
void HeatmapWidget::drawHeatmapForSerie(int serie)
{
    scene->clear();

    QMap<ConditionType, QMap<QString, int>> matrix = metricsManager->getHeatmapData(serie);

    QStringList repsEtapas;
    for (const auto& perCond : matrix) {
        for (const QString& repKey : perCond.keys()) {
            if (!repsEtapas.contains(repKey)) {
                repsEtapas.append(repKey);
            }
        }
    }
    std::sort(repsEtapas.begin(), repsEtapas.end());


    QStringList condiciones;
    for (ConditionType type : matrix.keys()) {
        condiciones.append(conditionTypeToString(type));
    }

    // Establecemos un tamaó para las celdas y los margenes de la tabla
    int cellW = 50;
    int cellH = 30;
    int marginX = 160;
    int marginY = 2 * cellH + 20;  // dejamos dos filas para las cabeceras

    // Obtener máximo valor para normalizar colores
    int maxValue = 1;
    for (const auto& row : matrix) {
        for (int val : row.values()) {
            if (val > maxValue) maxValue = val;
        }
    }

    // Leyendas verticales (condiciones)
    for (int i = 0; i < condiciones.size(); ++i) {
        QGraphicsTextItem* textItem = scene->addText(condiciones[i]);
        textItem->setTextWidth(marginX - 10);
        textItem->setPos(5, marginY + i * cellH);
    }

    // Leyendas horizontales: RepX (fila 0) y State X (fila 1)
    for (int j = 0; j < repsEtapas.size(); ++j) {
        QString key = repsEtapas[j];
        QStringList parts = key.split("_");
       // QStringList parts2 = parts.value(0).split("-");

        QString stateLabel = parts.value(1);
        QString repLabel;
        (stateLabel=="S00")? repLabel = parts.value(0):" ";

        int x = marginX + j * cellW;


        QGraphicsTextItem* repText = scene->addText(repLabel);
        //repText->setDefaultTextColor(Qt::darkBlue);
        repText->setPos(x + 5, 0);

        // State X
        QGraphicsTextItem* stateText = scene->addText(stateLabel);
        //stateText->setDefaultTextColor(Qt::darkGreen);
        stateText->setPos(x + 2, cellH);
    }

    // Pintar celdas
    for (int i = 0; i < condiciones.size(); ++i) {
        ConditionType cond = conditionTypeFromString(condiciones[i]);

        for (int j = 0; j < repsEtapas.size(); ++j) {
            int value = matrix[cond].value(repsEtapas[j], 0);
            QColor color = getValue_Color(value, maxValue);

            int x = marginX + j * cellW;
            int y = marginY + i * cellH;

            scene->addRect(x, y, cellW, cellH, QPen(Qt::black), QBrush(color));

            if (value > 0) {
                QGraphicsTextItem* text = scene->addText(QString::number(value));
                text->setDefaultTextColor(Qt::black);
                text->setPos(x + 10, y + 5);
            }
        }
    }

    // Ajustar la escena completa
    int totalWidth = marginX + repsEtapas.size() * cellW + 50;
    int totalHeight = marginY + condiciones.size() * cellH + 50;
    scene->setSceneRect(0, 0, totalWidth, totalHeight);
}


QColor HeatmapWidget::getValue_Color(int value, int max)
{
    if (value == 0) return QColor(Qt::white);

    int intensity = qMin(255, static_cast<int>(255.0 * value / max));
    return QColor(255, 255 - intensity, 255 - intensity); // de blanco → rojo
}
