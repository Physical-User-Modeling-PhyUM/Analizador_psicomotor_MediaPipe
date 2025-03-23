#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QObject>
#include <QList>
#include <memory>

#include "profiles/profiles.h"

#include "db/DBManager.h"
#include "profiles/userdatatypes.h"

class UserRepository : public QObject {
    Q_OBJECT
public:
    explicit UserRepository(DBManager* dbManager, QObject* parent = nullptr);

    QPointer<User> getUserById(int id);

    QList<QPointer<User>> getAllUsers();
    DBTable listUsers(UserType tipo);
    DBTable getUsersData(UserType tipo);

    void saveUser(const User& user);
    void deleteUser(int id);

private:
    DBManager* db;
};

#endif // USERREPOSITORY_H
