#include "usuario.h"

Usuario::Usuario(const QString &nombre, const QString &email, QObject *parent)
    : QObject(parent), nombre(nombre), email(email) {}

QString Usuario::getNombre() const {
    return nombre;
}

QString Usuario::getEmail() const {
    return email;
}
