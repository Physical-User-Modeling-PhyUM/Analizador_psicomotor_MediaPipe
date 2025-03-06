#include "user.h"

User::User(int id, const QString &userName, const QString &email, const QString &password, QObject *parent)
    : QObject(parent), idUser(id), userName(userName), email(email), password(password) {}

int User::getId() const {
    return idUser;
}

QString User::getUserName() const {
    return userName;
}

QString User::getEmail() const {
    return email;
}

void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

void User::setEmail(const QString &newEmail)
{
    email=newEmail;
}

void User::changePassword(const QString &newPassword) {
    password = newPassword;
}

void User::cancelAccount() {
    // Implement account cancellation logic
}
