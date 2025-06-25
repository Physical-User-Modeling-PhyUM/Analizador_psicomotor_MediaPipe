
/// @brief Declaración de la clase UserClientMainBoardWidget, que gestiona la vista principal del cliente en la aplicación.

#ifndef USERCLIENTMAINBOARDWIDGET_H
#define USERCLIENTMAINBOARDWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include "profiles/client.h"
#include "profiles/user.h"
#include "core/appcontroller.h"
#include "ui/client/userclientconfigwidget.h"
#include "ui/client/userclienttrainingsesionwidget.h"
#include "userclientinfowidget.h"
#include "userclientprofilewidget.h"
#include "ui_userclientmainboardwidget.h"

/// @class UserClientMainBoardWidget
/// @brief Widget principal del cliente, que actúa como panel de navegación entre secciones.
///
/// Este widget contiene el `stackedWidget` con las vistas:
/// - Información del cliente
/// - Selección de sesiones de entrenamiento
/// - Perfil del cliente
/// - Configuración personal
class UserClientMainBoardWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructor.
    /// @param controller Controlador principal de la aplicación.
    /// @param parent Widget padre opcional.
    explicit UserClientMainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~UserClientMainBoardWidget();

    /// @brief Asigna el usuario cliente activo al widget.
    /// @param user Instancia de `Client` actualmente autenticada.
    void setUser(QSharedPointer<Client> user);

public slots:
    /// @brief Muestra mensajes de error o información proveniente de otras partes del sistema.
    /// @param msg Texto del mensaje.
    /// @param type Tipo de mensaje Qt.
    void onUiErrorMessage(const QString &msg, QtMsgType type);

private slots:
    /// @brief Cambia la sección activa del stackedWidget al recibir un nuevo índice.
    /// @param index Índice seleccionado desde el menú lateral.
    void onSectionChanged(int index);

signals:
    /// @brief Señal emitida cuando el usuario solicita cerrar sesión.
    void logoutRequested();

private:
    Ui_UserClientMainBoardWidget *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<Client> currentUser; ///< Usuario cliente actualmente autenticado.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.

    QPointer<UserClientinfoWidget> infoWidget; ///< Widget de información general del usuario.
    QPointer<UserClientTrainingSesionWidget> trainingSelectionWidget; ///< Widget de selección de entrenamientos.
    QPointer<UserClientprofileWidget> profileWidget; ///< Widget de perfil personal.
    QPointer<UserClientConfigWidget> configWidget; ///< Widget de configuración personal.
};

#endif // USERCLIENTMAINBOARDWIDGET_H
