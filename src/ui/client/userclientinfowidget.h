/// @brief Declaración de la clase UserClientinfoWidget, encargada de mostrar los datos personales del cliente.

#ifndef USERCLIENTINFOWIDGET_H
#define USERCLIENTINFOWIDGET_H

#include <QWidget>
#include "core/appcontroller.h"
#include "profiles/client.h"
#include "ui_userclientinfowidget.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(useClientBoard)

/// @class UserClientinfoWidget
/// @brief Widget que muestra la información del perfil de un usuario cliente.
///
/// Incluye datos personales, plan asignado, fechas de conexión y una imagen de perfil editable.
class UserClientinfoWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructor.
    /// @param controller Controlador principal de la aplicación.
    /// @param parent Widget padre opcional.
    explicit UserClientinfoWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~UserClientinfoWidget();

    /// @brief Actualiza la información del usuario mostrada en pantalla.
    void updateUserInfo();

    /// @brief Asigna el usuario cliente que se debe mostrar.
    /// @param user Puntero compartido al cliente actual.
    void setUser(QSharedPointer<Client> user);

private slots:
    /// @brief Maneja el evento de selección de una nueva imagen de perfil.
    void on_newPhoto_clicked();

private:
    Ui_UserClientinfoWidget *ui; ///< Interfaz gráfica del widget.
    QSharedPointer<AppController> controller; ///< Controlador de la aplicación.
    QSharedPointer<Client> userClient; ///< Usuario cliente actualmente mostrado.
};

#endif // USERCLIENTINFOWIDGET_H
