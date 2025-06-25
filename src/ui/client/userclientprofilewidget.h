
/// @brief Declaración de la clase UserClientprofileWidget, encargada de mostrar el perfil detallado del cliente.

#ifndef USERCLIENTPROFILEWIDGET_H
#define USERCLIENTPROFILEWIDGET_H

#include <QWidget>
#include "core/appcontroller.h"
#include "ui_userclientprofilewidget.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(UserClientProfileBoard)

/// @class UserClientprofileWidget
/// @brief Widget para mostrar y eventualmente editar el perfil completo de un cliente.
///
/// Este widget está diseñado para desplegar los datos del perfil del usuario en la interfaz cliente.
/// En versiones futuras podrá incorporar funcionalidades de edición o actualización de los datos.
class UserClientprofileWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructor.
    /// @param controller Controlador principal de la aplicación.
    /// @param parent Widget padre opcional.
    explicit UserClientprofileWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~UserClientprofileWidget();

    /// @brief Asigna el usuario cliente cuyos datos se mostrarán en el perfil.
    /// @param user Usuario de tipo `Client`.
    void setUser(QSharedPointer<Client> user);

private:
    Ui_UserClientprofileWidget *ui; ///< Interfaz gráfica asociada al widget.
    QSharedPointer<Client> currentUser; ///< Cliente actualmente visualizado.
    QSharedPointer<AppController> controller; ///< Controlador principal para acceder a datos y lógica.
};

#endif // USERCLIENTPROFILEWIDGET_H
