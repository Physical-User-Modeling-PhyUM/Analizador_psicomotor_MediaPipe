
/// @brief Declaración de la clase AdminMainBoardWidget, encargada de gestionar la interfaz principal del rol administrador.

#ifndef ADMINMAINBOARDWIDGET_H
#define ADMINMAINBOARDWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/appcontroller.h"
#include "appconfigwidget.h"
#include "usersadminwidget.h"
#include "ui_adminmainboardwidget.h"

/// @class AdminMainBoardWidget
/// @brief Widget principal de la interfaz para el usuario con rol de administrador.
///
/// Este widget proporciona acceso a las funcionalidades de configuración de la aplicación y gestión de usuarios.
/// Utiliza un `QStackedWidget` para alternar entre secciones, y una lista lateral para navegar entre ellas.
class AdminMainBoardWidget : public QWidget {
    Q_OBJECT

public:
    /// @brief Constructor explícito.
    /// @param controller Puntero compartido al controlador principal de la aplicación.
    /// @param parent Puntero al widget padre, por defecto nullptr.
    explicit AdminMainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~AdminMainBoardWidget();

signals:
    /// @brief Señal emitida cuando se solicita cerrar sesión.
    void logoutRequested();

private slots:
    /// @brief Cambia la sección visible en el `QStackedWidget` según la selección de la lista lateral.
    /// @param index Índice de la sección seleccionada.
    void sectionChanged(int index);

private:
    Ui_AdminMainBoardWidget *ui; ///< Puntero al objeto de interfaz generado por Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.
    AppConfigWidget* configWidget; ///< Widget de configuración de la aplicación.
    UsersAdminWidget* usersWidget; ///< Widget de administración de usuarios.
};

#endif // ADMINMAINBOARDWIDGET_H
