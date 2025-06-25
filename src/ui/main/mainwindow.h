#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QPointer>

#include "ui_mainwindow.h"
#include "core/appcontroller.h"
#include "loginwidget.h"
#include "ui/client/userclientmainboardwidget.h"
#include "ui/fitnesstrainer/trainerboardwidget.h"
#include "ui/admin/adminmainboardwidget.h"
#include "profiles/user.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSharedPointer<AppController> appController;

    LoginWidget* loginWidget;
    UserClientMainBoardWidget* clientBoard;
    TrainerBoardWidget* trainerBoard;
    AdminMainBoardWidget* adminBoard;

private slots:
    void onLoginSuccess(QSharedPointer<User> user);
    void showLoginScreen();

};

#endif // MAINWINDOW_H
