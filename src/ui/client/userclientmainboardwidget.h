#ifndef USERCLIENTMAINBOARDWIDGET_H
#define USERCLIENTMAINBOARDWIDGET_H

#include <QWidget>
#include "profiles/client.h"
#include "ui/client/userclientconfigwidget.h"
#include "ui/client/userclienttrainingsesionwidget.h"
#include "ui_userclientmainboardwidget.h"
#include "core/appcontroller.h"
#include <QLoggingCategory>
#include "userclientinfowidget.h"
#include "userclienttrainingsesionwidget.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPointer>
#include "profiles/user.h"
#include "userclientprofilewidget.h"

class UserClientMainBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserClientMainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~UserClientMainBoardWidget();
      void setUser(QSharedPointer<Client> user);

public slots:
    void onUiErrorMessage(const QString &msg, QtMsgType type);
private slots:
    void onSectionChanged(int index);
signals:
    void logoutRequested();
private:
    Ui_UserClientMainBoardWidget *ui;
    QSharedPointer<Client> currentUser;
    QSharedPointer<AppController> controller;

    QPointer<UserClientinfoWidget> infoWidget;
    QPointer<UserClientTrainingSesionWidget> trainingSelectionWidget;
    QPointer<UserClientprofileWidget> profileWidget;
    QPointer<UserClientConfigWidget> configWidget;


};

#endif // USERCLIENTMAINBOARDWIDGET_H
