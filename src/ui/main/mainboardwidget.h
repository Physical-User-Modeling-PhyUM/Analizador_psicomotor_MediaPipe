#ifndef MAINBOARDWIDGET_H
#define MAINBOARDWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/appcontroller.h"
#include "ui_mainboardwidget.h"

class MainBoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~MainBoardWidget();

private:
    Ui_MainBoardwidget *ui;
    QSharedPointer<AppController> controller;
};

#endif // MAINBOARDWIDGET_H
