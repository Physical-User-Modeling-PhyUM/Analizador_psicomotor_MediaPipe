#include "UserManager.h"
#include <QDebug>

UserManager::UserManager(UserRepository* repo, QObject* parent)
    : QObject(parent), repository(repo), currentUser(nullptr) {}

QPointer<User> UserManager::getUser(int id) {
    return repository->getUserById(id);
}

bool UserManager::login(const QString& email, const QString& password) {
    // implementar autenticación real
    qDebug() << "Login not implemented. Always returns false.";
    return false;
}

void UserManager::logout() {
    currentUser = nullptr;
    emit userLoggedOut();
}

bool UserManager::createClient(const QMap<UserField, QVariant>& data, const ClientProfile& profile) {
    auto* client = new Client(data);
    if (!client) return false;

    client->setProfile(profile);
    repository->saveUser(*client);
    return true;
}

void UserManager::updateClientProfile(const ClientProfile& profile) {
    repository->saveClientProfile(profile);
}

QPointer<ClientProfile> UserManager::getClientProfile(int idClient) {
    return repository->getClientProfile(idClient);
}

UserPreferences UserManager::getUserPreferences(int idUser) {
    // cargar desde base de datos  userPreferences
    return UserPreferences(QMap<QString, QString>());
}

void UserManager::updateUserPreferences(const UserPreferences& prefs) {
    // guardar preferencias en base de datos
    qDebug() << "Preferences saved for user" << prefs.getUserId();
}
