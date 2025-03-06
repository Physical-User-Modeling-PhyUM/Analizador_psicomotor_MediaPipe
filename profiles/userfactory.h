#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "user.h"
#include "fitnesstrainer.h"
#include "client.h"
#include "admin.h"

class UserFactory {
public:
    static User* createUser(const QString &type, const QMap<QString, QString> &data);
};

#endif // USERFACTORY_H
