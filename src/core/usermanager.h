#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QPointer>
#include "repositories/userrepository.h"
#include "profiles/profiles.h"
#include "profiles/userpreferences.h"

class UserManager : public QObject {
    Q_OBJECT
public:
    explicit UserManager(UserRepository* repo, QObject* parent = nullptr);

    QPointer<User> getUser(int id);
    bool login(const QString& userName, const QString& password);
    void logout();

    bool createClient(const QMap<UserField, QVariant>& data, const ClientProfile& profile);
    void updateClientProfile(const ClientProfile& profile);
QPointer<ClientProfile> getClientProfile(int idClient);

    UserPreferences getUserPreferences(int idUser);
    void updateUserPreferences(const UserPreferences& prefs);

    QString newClient(const QString& userName, const QString& password);


signals:
    void userLoggedIn(QPointer<User>);
    void userLoggedOut();

private:
    UserRepository* repository;
    QPointer<User> currentUser;
};

#endif // USERMANAGER_H
