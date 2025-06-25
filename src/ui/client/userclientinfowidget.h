#ifndef USERCLIENTINFOWIDGET_H
#define USERCLIENTINFOWIDGET_H

#include <QWidget>
#include "core/appcontroller.h"
#include "profiles/client.h"
#include "ui_userclientinfowidget.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(useClientBoard)

class UserClientinfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserClientinfoWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~UserClientinfoWidget();
    void updateUserInfo();
    void setUser(QSharedPointer<Client> user);

private slots:
    void on_newPhoto_clicked();

private:
   Ui_UserClientinfoWidget *ui;
    QSharedPointer<AppController> controller;
    QSharedPointer<Client> userClient;
};

#endif // USERCLIENTINFOWIDGET_H
