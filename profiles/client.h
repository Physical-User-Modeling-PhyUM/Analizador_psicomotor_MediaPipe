#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"

class Client : public User {
public:
    explicit Client(int id, const QString &userName, const QString &email, const QString &password, int expLevel, QObject *parent = nullptr);
    void assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) override;

    int getExpLevel() const;
    void setExpLevel(int newExpLevel);

private:
    int expLevel;
};

#endif // CLIENT_H
