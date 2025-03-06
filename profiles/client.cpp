#include "client.h"

Client::Client(int id, const QString &userName, const QString &email, const QString &password, int expLevel, QObject *parent)
    : User(id, userName, email, password, parent), expLevel(expLevel) {}

void Client::assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) {
    // Implement plan assignment logic
}

int Client::getExpLevel() const {
    return expLevel;
}

void Client::setExpLevel(int newExpLevel) {
    expLevel = newExpLevel;
}
