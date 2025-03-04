#ifndef BDUSUARIOS_H
#define BDUSUARIOS_H

#include <QObject>
#include <QMap>
#include "usuario.h"

class BDUsuarios : public QObject {
    Q_OBJECT

public:
    explicit BDUsuarios(QObject *parent = nullptr);
    bool insertarUsuario(Usuario *usuario);
    bool eliminarUsuario(const QString &email);
    Usuario* obtenerUsuario(const QString &email);
    bool existeUsuario(const QString &email) const;

private:
    QMap<QString, Usuario*> usuarios;
};

#endif // BDUSUARIOS_H
