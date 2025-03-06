#include "userdbmanager.h"

QMap<QString, QString> UserDBManager::listUsers(const QString &type) {
    // Logic to retrieve users from the database
    return QMap<QString, QString>();
}

QMap<QString, QString> UserDBManager::getUserData(int userId) {
    // Logic to retrieve a specific user's data from the database
    return QMap<QString, QString>();
}

void UserDBManager::saveUser(const User &user) {
    // Logic to save user details into the database
}

void UserDBManager::cancelUser(int userId) {
    // Logic to remove user from the database
}

QMap<QString, QString> UserDBManager::getUserPreferences(int userId)
{

}
