#include "client.h"


Client::Client(QMap<DataType, QVariant> dataMap, QObject* parent)
    : User(
          dataMap.value(DataType::IdUser).toInt(),
          dataMap.value(DataType::UserName).toString(),
          dataMap.value(DataType::Email).toString(),
          dataMap.value(DataType::Pass).toString(),
          parent
          ),
    expLevel(dataMap.value(DataType::Level).toInt())
{
    if (!dataMap.contains(DataType::IdUser)
        || !dataMap.contains(DataType::UserName)
        ||!dataMap.contains(DataType::Email)
        || !dataMap.contains(DataType::Pass)) {
        qWarning() << "Error: Faltan campos requeridos para crear un Client";
        return;
    }
}


void Client::assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) {
    // Implement plan assignment logic
}

int Client::getExpLevel() const {
    return expLevel;
}

void Client::setExpLevel(int newExpLevel) {
    expLevel = newExpLevel;
}
