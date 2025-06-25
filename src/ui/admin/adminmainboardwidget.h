
// AdminMainBoardWidget.h
#ifndef ADMINMAINBOARDWIDGET_H
#define ADMINMAINBOARDWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/appcontroller.h"
#include "appconfigwidget.h"
#include "usersadminwidget.h"
#include "ui_adminmainboardwidget.h"



class AdminMainBoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit AdminMainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~AdminMainBoardWidget();

signals:
    void logoutRequested();

private slots:
    void sectionChanged(int index);

private:
    Ui_AdminMainBoardWidget *ui;
    QSharedPointer<AppController> controller;
    AppConfigWidget* configWidget;
    UsersAdminWidget* usersWidget;
};

#endif // ADMINMAINBOARDWIDGET_H
