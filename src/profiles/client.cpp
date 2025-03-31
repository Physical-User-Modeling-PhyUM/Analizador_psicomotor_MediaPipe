#include "client.h"


Client::Client(QMap<UserField, QVariant> dataMap, QObject* parent)
    : User(
          dataMap.value(UserField::IdUser).toInt(),
          dataMap.value(UserField::UserName).toString(),
          dataMap.value(UserField::Email).toString(),
          dataMap.value(UserField::Pass).toString(),
          parent
          )

{
    if (!dataMap.contains(UserField::IdUser)
        || !dataMap.contains(UserField::UserName)
        ||!dataMap.contains(UserField::Email)
        || !dataMap.contains(UserField::Pass)) {
        qWarning() << "Error: Faltan campos requeridos para crear un Client";
        return;
    }

}
Client::~Client() = default;

// void Client::assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) {
//     // A ver  que estructura se usa para esto
// }

ClientProfile Client::getProfile() const
{
    return profile;
}

void Client::setProfile(const ClientProfile &newProfile)
{
    profile = newProfile;
}

int Client::getWorkoutId(QDateTime fecha)
{
    return workoutCalendar.value(fecha);
}

void Client::setWorkout(QDateTime fecha, int workoutId)
{
    workoutCalendar.insert(fecha,workoutId);
}

bool Client::isDateAvailable(QDateTime fecha)
{
    return workoutCalendar.contains(fecha);
}
