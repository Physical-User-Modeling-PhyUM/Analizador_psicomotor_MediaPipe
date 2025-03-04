#ifndef GESTORDEUSUARIOS_H
#define GESTORDEUSUARIOS_H

#include <QObject>
#include "bd_usuarios.h"
#include "factoría_usuarios.h"

class GestorDeUsuarios : public QObject {
    Q_OBJECT

public:
    explicit GestorDeUsuarios(QObject *parent = nullptr);
    Usuario* crearUsuario(const QString &tipo, const QString &nombre, const QString &email);
    bool eliminarUsuario(const QString &email);
    Usuario* obtenerUsuario(const QString &email);

private:
    BDUsuarios bdUsuarios;
};

#endif // GESTORDEUSUARIOS_H
