#ifndef USERCLIENTCONFIGWIDGET_H
#define USERCLIENTCONFIGWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include "core/appcontroller.h"
#include "ui_userclientconfigwidget.h"

class UserClientConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserClientConfigWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~UserClientConfigWidget();
    void loadConfig();
private:
    Ui_UserClientConfigWidget* ui;
    QSharedPointer<AppController> controller;


    void saveUserPreferences();
    //void populateConnectionsTable();
};

#endif // USERCLIENTCONFIGWIDGET_H

