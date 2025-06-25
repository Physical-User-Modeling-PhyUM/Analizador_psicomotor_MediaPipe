#include "feedbackgraphwidget.h"
#include <QPainter>
#include <QConicalGradient>
#include <QFont>
#include <QPen>

FeedBackGraphWidget::FeedBackGraphWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(200, 200);
}

void FeedBackGraphWidget::updateFromFeedback(const FeedBack &feedback)
{


    for (const ConditionType &type:feedback.getEntriesList()) {
         if (feedback.isCritical(type)) ++criticalCount;
         else if (feedback.isAlert(type)) ++alertCount;
         else if (feedback.isOptimal(type))  ++optimalCount;
    }
    //Actializa el widget(repinta)
    update();
}

void FeedBackGraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint centro = rect().center();
    int radio = qMin(width(), height()) / 3;

    QRectF circulo(centro.x() - radio,
                   centro.y() - radio - 10,
                   2 * radio, 2 * radio);


    painter.setBrush(QColor(0, 0, 0, 60));
    painter.setPen(Qt::NoPen);
    QRectF shadow = circulo.adjusted(0, 10, 0, 10);
    painter.drawEllipse(shadow);

    int total = optimalCount + alertCount + criticalCount;
    if (total == 0) return;

    double optPerc = 100.0 * optimalCount / total;
    double alertPerc = 100.0 * alertCount / total;
    double critPerc = 100.0 * criticalCount / total;

    int startAngle = 0;

    auto drawSegment = [&](double percent, const QColor &color) {
        int apertura = static_cast<int>(360.0 * 16 * percent / 100.0);

        QConicalGradient gradient(centro, startAngle / 16.0);
        gradient.setColorAt(0.0, color.lighter(130));
        gradient.setColorAt(0.5, color);
        gradient.setColorAt(1.0, color.darker(130));
        painter.setBrush(gradient);
        painter.drawPie(circulo, startAngle, apertura);
        startAngle += apertura;
    };

    drawSegment(optPerc, QColor(100, 220, 100));
    drawSegment(alertPerc, QColor(255, 200, 0));
    drawSegment(critPerc, QColor(240, 80, 80));

    // Draw legend
    painter.setPen(QPen(Qt::white));
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    int y = circulo.bottom() + 20;
    painter.drawText(20, y, QString("Optimal: %1%").arg(static_cast<int>(optPerc)));
    painter.drawText(20, y + 25, QString("Alerts: %1%").arg(static_cast<int>(alertPerc)));
    painter.drawText(20, y + 50, QString("Critical: %1%").arg(static_cast<int>(critPerc)));
}
