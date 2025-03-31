#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QDateTime>


#include <QDateTime>
#include <QImage>
#include "enums/enums.h"

class User : public QObject {
    Q_OBJECT

public:
    explicit User(int id, const QString &userName, const QString &email, const QString &password, QObject *parent = nullptr);
    virtual ~User() = default;

    int getId() const;
    QString getUserName() const;
    QString getEmail() const;
    void setUserName(const QString &newUserName);
    void setEmail(const QString &newEmail);
    void changePassword(const QString &newPassword);
    void cancelAccount();
    //virtual void assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) = 0;
    QDateTime getJoin_up_date() const;
    void setJoin_up_date(const QDateTime &newJoin_up_date);
    QDateTime getLast_login()const;
    void setLast_login(const QDateTime &newLast_login);
    QImage getProfile_Picture() const;
    void setProfile_Picture(const QImage &newProfile_Picture);

    UserType getUserType() const;
    void setUserType(UserType newUserType);

protected:
    int idUser;
    UserType userType;

    // Estos los podía definir en un enumerado como para cliente?
    QString userName;
    QString email;
    QString password;
    QDateTime join_up_date;
    QDateTime last_login;
    QImage profile_Picture;
    
};

#endif // USER_H
