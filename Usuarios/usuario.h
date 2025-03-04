#ifndef USUARIO_H
#define USUARIO_H

#include <QString>

class Usuario {
protected:
    QString nombre;
    QString email;

public:
    Usuario(const QString& nombre, const QString& email);
    virtual ~Usuario() = default;

    QString getNombre() const;
    QString getEmail() const;

    virtual QString tipoUsuario() const = 0; // Método virtual puro
};

#endif // USUARIO_H
