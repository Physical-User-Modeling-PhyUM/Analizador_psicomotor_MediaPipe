#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"
#include "enums/GeneralDataEnum.h"

class Admin : public User {
public:
    explicit Admin(QMap<UserField, QVariant> dataMap, QObject *parent = nullptr);


    int getPermissionLevel() const;
    void setPermissionLevel(int newLevel);

    QString getRoleDescription() const;
    void setRoleDescription(const QString &desc);

    bool canManage() const;
    void setCanManage(bool value);

    bool canModifyTrainingPlans() const;
    void setCanModifyTrainingPlans(bool value);

private:
    int permissionLevel;
    QString roleDescription;
    bool canManageUsers;
    bool canModifyPlans;
};

#endif // ADMIN_H
