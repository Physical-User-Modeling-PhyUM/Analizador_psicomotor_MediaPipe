#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QObject>
#include <QList>


#include "profiles/profiles.h"

#include "core/DBManager.h"
#include <QPointer>
#include "enums/enums.h"


class UserRepository : public QObject {
    Q_OBJECT
public:
    explicit UserRepository(DBManager* dbManager, QObject* parent = nullptr);

    QPointer<User> getUserById(int id);

    DBTable listUsers(UserType tipo);

    DBTable getUsersData(UserType tipo);

    QPointer<ClientProfile> getClientProfile(int idClient);
    void saveClientProfile(const ClientProfile& profile);

    void saveUser(const User& user);
    void deleteUser(int id);

    int checkUserName (QString newUserName) const;

private:
    DBManager* db;


};

#endif // USERREPOSITORY_H
