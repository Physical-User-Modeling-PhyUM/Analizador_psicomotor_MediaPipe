#include "userfactory.h"

User* UserFactory::createUser(const QString &type, const QMap<QString, QString> &data) {
    int id = data["id"].toInt();
    QString name = data["userName"];
    QString email = data["email"];
    QString password = data["password"];

    if (type == "FitnessTrainer") {
        QString qualifications = data["qualifications"];
        return new FitnessTrainer(id, name, email, password, qualifications);
    } else if (type == "Client") {
        int expLevel = data["expLevel"].toInt();
        return new Client(id, name, email, password, expLevel);
    } else if (type == "Admin") {
        int permissionLevel = data["permissionLevel"].toInt();
        return new Admin(id, name, email, password, permissionLevel);
    }
    return nullptr;
}
