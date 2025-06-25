#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/loginmanager.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QSharedPointer<LoginManager> loginManager, QWidget *parent = nullptr);
    ~LoginWidget();

private slots:
    //void handleLogin();





    void on_pushButton_2_clicked();

    void on_RegisterButton_2_clicked();

    void on_RegisterButton_clicked();

    void on_LoginButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::LoginWidget *ui;
    QSharedPointer<LoginManager> loginManager;
};

#endif // LOGINWIDGET_H
