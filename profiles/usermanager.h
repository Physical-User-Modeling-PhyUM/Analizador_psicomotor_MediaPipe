#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include "userdbmanager.h"
#include "userpreferences.h"
#include "userfactory.h"

class UserManager {
public:
    User* createUser(const QString &type, const QMap<QString, QString> &data);
    void cancelUser(int userId);
    void updateUser(int userId, const QMap<QString, QString> &data);
    User* lookUpUser(int userId);
    UserPreferences getUserPreferences(int userId);
    void updateUserPreferences(int userId, const UserPreferences &preferences);


private:
    UserDBManager dbManager;
};

#endif // USERMANAGER_H
