#include "usermanager.h"

User* UserManager::createUser(const QString &type, const QMap<QString, QString> &data) {
    User* user = UserFactory::createUser(type, data);
    if (user) {
        dbManager.saveUser(*user);
    }
    return user;
}

void UserManager::cancelUser(int userId) {
    dbManager.cancelUser(userId);
}

void UserManager::updateUser(int userId, const QMap<QString, QString> &data) {
    User* user = lookUpUser(userId);
    if (user) {
       if (data.contains("userName")) user->setUserName(data["userName"]);
        if (data.contains("email")) user->setUserName(data["email"]);
        if (data.contains("password")) user->changePassword(data["password"]);
        dbManager.saveUser(*user);
    }
}

User* UserManager::lookUpUser(int userId) {
    QMap<QString, QString> userData = dbManager.getUserData(userId);
    if (!userData.isEmpty()) {
        return UserFactory::createUser(userData["type"], userData);
    }
    return nullptr;
}

UserPreferences UserManager::getUserPreferences(int userId) {
    return UserPreferences(dbManager.getUserPreferences(userId));
}

void UserManager::updateUserPreferences(int userId, const UserPreferences &preferences) {
    dbManager.updateUserPreferences(userId, preferences);
}
