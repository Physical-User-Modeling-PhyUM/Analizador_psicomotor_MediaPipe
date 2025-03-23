#include "userrepository.h"
#include <QDebug>

UserRepository::UserRepository(DBManager* dbManager, QObject* parent)
    : QObject(parent), db(dbManager) {}

std::shared_ptr<User> UserRepository::getUserById(int id) {
    QString query = QString("SELECT * FROM users WHERE id = %1").arg(id);
    auto rows = db->fetchRows(query);

    if (!rows.isEmpty()) {
        auto row = rows.first();
        return std::make_shared<User>(
            row["id"].toInt(),
            row["name"],
            row["email"],
            row["role"]
            );
    }

    return nullptr;
}

QList<std::shared_ptr<User>> UserRepository::getAllUsers() {
    QList<std::shared_ptr<User>> users;
    auto rows = db->fetchRows("SELECT * FROM users");

    for (const auto& row : rows) {
        users.append(std::make_shared<User>(
            row["id"].toInt(),
            row["name"],
            row["email"],
            row["role"]
            ));
    }

    return users;
}

void UserRepository::saveUser(const User& user) {
    QString query = QString(
                        "INSERT INTO users (id, name, email, role) VALUES (%1, '%2', '%3', '%4')")
                        .arg(user.getId())
                        .arg(user.getUserName())
                        .arg(user.getEmail());

    db->executeQuery(query);
}

void UserRepository::deleteUser(int id) {
    QString query = QString("DELETE FROM users WHERE id = %1").arg(id);
    db->executeQuery(query);
}
