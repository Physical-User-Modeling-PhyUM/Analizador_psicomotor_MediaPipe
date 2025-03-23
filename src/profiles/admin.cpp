#include "admin.h"

Admin::Admin(int id, const QString &userName, const QString &email, const QString &password, int permissionLevel, QObject *parent)
    : User(id, userName, email, password, parent), permissionLevel(permissionLevel) {}

void Admin::assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) {
    // Admins typically do not have plans, implement logic accordingly
}

int Admin::getPermissionLevel() const {
    return permissionLevel;
}

void Admin::setPermissionLevel(int newLevel) {
    permissionLevel = newLevel;
}
