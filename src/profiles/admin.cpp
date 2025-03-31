#include "admin.h"
#include <QDebug>

Admin::Admin(QMap<UserField, QVariant> dataMap, QObject *parent)
    : User(
          dataMap.value(UserField::IdUser).toInt(),
          dataMap.value(UserField::UserName).toString(),
          dataMap.value(UserField::Email).toString(),
          dataMap.value(UserField::Pass).toString(),
          parent
          ),
    permissionLevel(dataMap.value(UserField::Level).toInt()),
    roleDescription(dataMap.value(UserField::Qualification).toString()),
    canManageUsers(dataMap.value(UserField::Notifications).toBool()),
    canModifyPlans(dataMap.value(UserField::Units).toBool())
{
    if (!dataMap.contains(UserField::IdUser)
        || !dataMap.contains(UserField::UserName)
        || !dataMap.contains(UserField::Email)
        || !dataMap.contains(UserField::Pass)) {
        qWarning() << "Error: Faltan campos requeridos para crear un Admin";
        return;
    }
}

void Admin::assignPlan(int, const QDateTime &, const QDateTime &) {
    // No aplica para Admin
}

int Admin::getPermissionLevel() const {
    return permissionLevel;
}

void Admin::setPermissionLevel(int newLevel) {
    permissionLevel = newLevel;
}

QString Admin::getRoleDescription() const {
    return roleDescription;
}

void Admin::setRoleDescription(const QString &desc) {
    roleDescription = desc;
}

bool Admin::canManage() const {
    return canManageUsers;
}

void Admin::setCanManage(bool value) {
    canManageUsers = value;
}

bool Admin::canModifyTrainingPlans() const {
    return canModifyPlans;
}

void Admin::setCanModifyTrainingPlans(bool value) {
    canModifyPlans = value;
}
