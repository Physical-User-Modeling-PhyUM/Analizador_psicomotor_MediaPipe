#ifndef USERSADMINWIDGET_H
#define USERSADMINWIDGET_H

#include <QWidget>
#include "ui_usersadminwidget.h"

#include <QSharedPointer>
#include <QTableView>
#include <QPushButton>
#include <QMap>
#include "core/appcontroller.h"
#include "ui/main/uitablemodel.h"
#include "enums/UserFieldEnum.h"

namespace Ui {
class UsersAdminWidget;
}

class UsersAdminWidget : public QWidget {
    Q_OBJECT

public:
    explicit UsersAdminWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~UsersAdminWidget();

    void onUserTypeChanged(const QString &typeStr);
private slots:
    void onUserSelected(const QModelIndex& index);
    void onSaveClicked();
    void onDeleteClicked();
    void onNewClicked();
    void onEditTableDataChanged(const QModelIndex& m1, const QModelIndex& m2);

private:
    Ui_UsersAdminWidget *ui;
    QSharedPointer<AppController> controller;
    UiTableModel* userTableModel;
    UiTableModel* editTableModel;
    int selectedUserId = -1;
    QSharedPointer<User> editUser;


    void loadUsers();
    void populateEditTable(QSharedPointer<User> user);
    QHash<UserField, QVariant> collectEditTableData();
    void clearEditTable();
};

#endif // USERSADMINWIDGET_H
