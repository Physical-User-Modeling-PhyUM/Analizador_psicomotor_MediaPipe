/**
 * @file mainwindow.h
 * @brief Declaración de la clase MainWindow, ventana principal de la aplicación tras el arranque.
 *
 * Esta clase representa la ventana contenedora principal (`QMainWindow`) y gestiona el flujo inicial de la aplicación,
 * mostrando la pantalla de login y, tras la autenticación, redirigiendo a la interfaz correspondiente
 * según el tipo de usuario (Cliente, Entrenador, Administrador).
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QPointer>

#include "ui_mainwindow.h"
#include "core/appcontroller.h"
#include "loginwidget.h"
#include "ui/client/userclientmainboardwidget.h"
#include "ui/fitnesstrainer/trainerboardwidget.h"
#include "ui/admin/adminmainboardwidget.h"
#include "profiles/user.h"

/**
 * @class MainWindow
 * @brief Ventana principal que orquesta el ciclo de vida gráfico de la aplicación.
 *
 * MainWindow se encarga de:
 * - Mostrar la pantalla de login inicial (`LoginWidget`)
 * - Escuchar el evento de login exitoso (`onLoginSuccess`)
 * - Cargar dinámicamente el panel principal según el rol del usuario autenticado:
 *   - `UserClientMainBoardWidget` para clientes
 *   - `TrainerBoardWidget` para entrenadores
 *   - `AdminMainBoardWidget` para administradores
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la ventana principal.
     * @param controller Controlador principal que gestiona el modelo de dominio y navegación.
     * @param parent Widget padre opcional.
     */
    explicit MainWindow(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<AppController> appController; ///< Controlador central de la aplicación.

    LoginWidget* loginWidget; ///< Widget de autenticación de usuario.
    UserClientMainBoardWidget* clientBoard; ///< Panel principal para usuarios cliente.
    TrainerBoardWidget* trainerBoard; ///< Panel principal para entrenadores.
    AdminMainBoardWidget* adminBoard; ///< Panel principal para administradores.

private slots:
    /**
     * @brief Slot llamado tras login exitoso. Carga el panel correspondiente al rol del usuario.
     * @param user Usuario autenticado.
     */
    void onLoginSuccess(QSharedPointer<User> user);

    /**
     * @brief Muestra la pantalla de login (usado al inicio o tras cerrar sesión).
     */
    void showLoginScreen();
};

#endif // MAINWINDOW_H
