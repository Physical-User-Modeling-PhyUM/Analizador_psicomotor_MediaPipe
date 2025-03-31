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

QDateTime User::getJoin_up_date() const
{
    return join_up_date;
}

void User::setJoin_up_date(const QDateTime &newJoin_up_date)
{
    join_up_date = newJoin_up_date;
}

QDateTime User::getLast_login() const
{
    return last_login;
}

void User::setLast_login(const QDateTime &newLast_login)
{
    last_login = newLast_login;
}

QImage User::getProfile_Picture() const
{
    return profile_Picture;
}

void User::setProfile_Picture(const QImage &newProfile_Picture)
{
    profile_Picture = newProfile_Picture;
}

UserType User::getUserType() const
{
    return userType;
}

void User::setUserType(UserType newUserType)
{
    userType = newUserType;
}
