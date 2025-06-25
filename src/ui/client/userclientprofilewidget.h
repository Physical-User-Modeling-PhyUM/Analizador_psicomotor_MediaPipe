#ifndef USERCLIENTPROFILEWIDGET_H
#define USERCLIENTPROFILEWIDGET_H

#include <QWidget>
#include "core/appcontroller.h"
#include "ui_userclientprofilewidget.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(UserClientProfileBoard)


class UserClientprofileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserClientprofileWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~UserClientprofileWidget();
  void setUser(QSharedPointer<Client> user);
private:
    Ui_UserClientprofileWidget *ui;
    QSharedPointer<Client> currentUser;
    QSharedPointer<AppController> controller;
};

#endif // USERCLIENTPROFILEWIDGET_H
