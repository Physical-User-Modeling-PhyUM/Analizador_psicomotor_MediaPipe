#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "clientprofile.h"


class Client : public User {
public:

    explicit Client(QMap<UserField,QVariant>,QObject *parent = nullptr);
    ~Client() override;


    ClientProfile getProfile() const;
    void setProfile(const ClientProfile &newProfile);
    int getWorkoutId(QDateTime fecha);
    void setWorkout(QDateTime fecha, int wokoutId);
    bool isDateAvailable(QDateTime fecha);
private:

    QString plan;
    ClientProfile profile;
    QHash <QDateTime,int> workoutCalendar;

};

#endif // CLIENT_H
