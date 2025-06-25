/**
 * @file loginwidget.h
 * @brief Declaración de la clase LoginWidget, interfaz de autenticación de usuarios en la aplicación.
 *
 * Este widget proporciona una interfaz gráfica para que el usuario pueda iniciar sesión o registrarse en el sistema.
 * Utiliza `LoginManager` como componente lógico para la validación de credenciales y autenticación segura.
 */

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/loginmanager.h"

namespace Ui {
class LoginWidget;
}

/**
 * @class LoginWidget
 * @brief Widget de autenticación de usuario que permite iniciar sesión o registrarse en la aplicación.
 *
 * Proporciona los campos básicos de usuario y contraseña, así como botones para confirmar o cancelar el inicio de sesión,
 * y para acceder al proceso de registro de nuevos usuarios. Toda la lógica de validación se delega en la clase `LoginManager`.
 */
class LoginWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param loginManager Puntero compartido al gestor de login, responsable de la autenticación.
     * @param parent Widget padre opcional.
     */
    explicit LoginWidget(QSharedPointer<LoginManager> loginManager, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~LoginWidget();

private slots:
    /**
     * @brief Acción del botón de login. Intenta autenticar al usuario.
     */
    void on_LoginButton_clicked();

    /**
     * @brief Acción del botón de cancelar. Limpia los campos o cierra el widget.
     */
    void on_CancelButton_clicked();

    /**
     * @brief Acción del botón de registro alternativo 1.
     */
    void on_RegisterButton_clicked();

    /**
     * @brief Acción del botón de registro alternativo 2 (posiblemente duplicado o de otra vista).
     */
    void on_RegisterButton_2_clicked();

    /**
     * @brief Acción del botón adicional sin nombre claro (requiere refactor).
     */
    void on_pushButton_2_clicked();

private:
    Ui::LoginWidget *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<LoginManager> loginManager; ///< Gestor responsable de la lógica de autenticación.
};

#endif // LOGINWIDGET_H
