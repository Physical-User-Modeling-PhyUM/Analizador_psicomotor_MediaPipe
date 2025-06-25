/// @brief Declaración de la clase UserClientConfigWidget, encargada de gestionar la configuración personal del usuario cliente.

#ifndef USERCLIENTCONFIGWIDGET_H
#define USERCLIENTCONFIGWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include "core/appcontroller.h"
#include "ui_userclientconfigwidget.h"

/// @class UserClientConfigWidget
/// @brief Widget para que el usuario cliente personalice su experiencia.
///
/// Permite configurar opciones como el modo dual, sonidos informativos y de alerta, y conexiones personalizadas entre keypoints.
/// Los cambios se guardan como preferencias del usuario.
class UserClientConfigWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructor del widget.
    /// @param controller Puntero al controlador de la aplicación.
    /// @param parent Widget padre opcional.
    explicit UserClientConfigWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~UserClientConfigWidget();

    /// @brief Carga la configuración actual del usuario y la refleja en el formulario.
    void loadConfig();

private:
    Ui_UserClientConfigWidget* ui; ///< Interfaz gráfica del widget.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.

    /// @brief Guarda las preferencias del usuario basadas en el estado actual del formulario.
    void saveUserPreferences();
};

#endif // USERCLIENTCONFIGWIDGET_H
