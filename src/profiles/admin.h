#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"

class Admin : public User {
public:
    explicit Admin(int id, const QString &userName, const QString &email, const QString &password, int permissionLevel, QObject *parent = nullptr);
    void assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) override;

    int getPermissionLevel() const;
    void setPermissionLevel(int newLevel);

private:
    int permissionLevel;
};

#endif // ADMIN_H
