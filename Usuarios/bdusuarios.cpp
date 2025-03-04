#include "bd_usuarios.h"

BDUsuarios::BDUsuarios(QObject *parent) : QObject(parent) {}

bool BDUsuarios::insertarUsuario(Usuario *usuario) {
    if (usuarios.contains(usuario->getEmail())) {
        return false;
    }
    usuarios.insert(usuario->getEmail(), usuario);
    return true;
}

bool BDUsuarios::eliminarUsuario(const QString &email) {
    return usuarios.remove(email) > 0;
}

Usuario* BDUsuarios::obtenerUsuario(const QString &email) {
    return usuarios.value(email, nullptr);
}

bool BDUsuarios::existeUsuario(const QString &email) const {
    return usuarios.contains(email);
}
