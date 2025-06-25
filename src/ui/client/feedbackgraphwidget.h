#ifndef FEEDBACKGRAPHWIDGET_H
#define FEEDBACKGRAPHWIDGET_H

#include <QWidget>
#include <QHash>
#include "pose/feedback.h"

class FeedBackGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FeedBackGraphWidget(QWidget *parent = nullptr);

    void updateFromFeedback(const FeedBack& feedback);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int optimalCount = 0;
    int alertCount = 0;
    int criticalCount = 0;
};

#endif // FEEDBACKGRAPHWIDGET_H
