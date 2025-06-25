/**
 * @file mainboardwidget.h
 * @brief Declaración de la clase MainBoardWidget, contenedor principal de interfaz tras la autenticación.
 *
 * Esta clase representa el widget principal que se muestra tras el login exitoso del usuario.
 * Se encarga de cargar dinámicamente el contenido correspondiente según el tipo de usuario (Cliente, Entrenador o Administrador).
 * Actúa como punto de entrada visual para la navegación general dentro de la aplicación.
 */

#ifndef MAINBOARDWIDGET_H
#define MAINBOARDWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/appcontroller.h"
#include "ui_mainboardwidget.h"

/**
 * @class MainBoardWidget
 * @brief Widget principal de la aplicación que contiene y gestiona la vista según el rol del usuario autenticado.
 *
 * Este widget se muestra justo después del proceso de login. Carga la interfaz adecuada (cliente, entrenador, administrador)
 * según el rol del usuario activo, utilizando el `AppController` como punto de acceso al modelo de dominio.
 */
class MainBoardWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param controller Controlador principal de la aplicación, encargado de gestionar el flujo general.
     * @param parent Widget padre opcional.
     */
    explicit MainBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MainBoardWidget();

private:
    Ui_MainBoardwidget *ui; ///< Interfaz gráfica generada automáticamente por Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador central de la lógica y navegación de la aplicación.
};

#endif // MAINBOARDWIDGET_H
