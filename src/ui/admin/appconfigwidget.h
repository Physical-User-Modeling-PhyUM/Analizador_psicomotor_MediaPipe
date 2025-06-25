/// @file appconfigwidget.h
/// @brief Declaración de la clase AppConfigWidget, encargada de la edición de configuraciones del sistema relacionadas con la captura de pose.

#ifndef APPCONFIGWIDGET_H
#define APPCONFIGWIDGET_H

#include <QWidget>
#include "ui_appconfigwidget.h"
#include "core/appcontroller.h"
#include <QSharedPointer>
#include "ui/main/uitablemodel.h"

namespace Ui {
class AppConfigWidget;
}

/// @class AppConfigWidget
/// @brief Widget de configuración de parámetros relacionados con la captura de pose.
///
/// Esta clase permite visualizar y modificar parámetros del sistema en una tabla editable,
/// utilizando un modelo de datos personalizado. Los cambios se aplican al pulsar el botón de guardar.
class AppConfigWidget : public QWidget {
    Q_OBJECT

public:
    /// @brief Constructor del widget.
    /// @param controller Puntero compartido al controlador de la aplicación.
    /// @param parent Widget padre (por defecto nullptr).
    explicit AppConfigWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~AppConfigWidget();

private slots:
    /// @brief Slot invocado al hacer clic en el botón de guardado.
    /// Extrae los datos del modelo y los almacena en la configuración de la aplicación.
    void onSaveClicked();

private:
    Ui_AppConfigWidget *ui; ///< Puntero a la interfaz generada por Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.
    UiTableModel* configModel; ///< Modelo de datos para mostrar las claves y valores de configuración.

    /// @brief Carga la configuración actual desde el controlador y la inserta en el modelo de tabla.
    void loadConfigToForm();

    /// @brief Recolecta los datos de la tabla como un hash clave-valor.
    /// @return Hash con las claves y valores modificados por el usuario.
    QHash<QString, QVariant> collectConfigData();
};

#endif // APPCONFIGWIDGET_H
