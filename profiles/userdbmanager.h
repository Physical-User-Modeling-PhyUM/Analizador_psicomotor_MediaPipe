#ifndef USERDBMANAGER_H
#define USERDBMANAGER_H

#include <QMap>
#include "user.h"
#include "userpreferences.h"

class UserDBManager {
public:
    QMap<QString, QString> listUsers(const QString &type);
    QMap<QString, QString> getUserData(int userId);
    void saveUser(const User &user);
    void cancelUser(int userId);
    QMap<QString, QString> getUserPreferences(int userId);
    void updateUserPreferences(int userId,const UserPreferences &preferences );
};

#endif // USERDBMANAGER_H
