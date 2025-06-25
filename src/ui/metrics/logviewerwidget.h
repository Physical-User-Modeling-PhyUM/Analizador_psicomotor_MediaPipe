#ifndef LOGVIEWERWIDGET_H
#define LOGVIEWERWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QSharedPointer>
#include "ui/main/uitablemodel.h"
#include "workouts/trainingsesion.h"
#include "core/appcontroller.h"
#include "ui_logviewerwidget.h"

class LogViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogViewerWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~LogViewerWidget();

    void loadSesion(QSharedPointer<TrainingSesion> sesion);

private slots:
    void onHeaderClicked(int column);

private:
    Ui_LogViewerWidget *ui;
    QSharedPointer<AppController> controller;
    QSharedPointer<TrainingSesion> currentSesion;

    UiTableModel* logModel = nullptr;
    QSortFilterProxyModel* proxyModel = nullptr;
};

#endif // LOGVIEWERWIDGET_H
