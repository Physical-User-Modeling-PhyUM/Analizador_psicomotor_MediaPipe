/**
 * @file logviewerwidget.h
 * @brief Declaración de la clase LogViewerWidget, interfaz para visualizar el log estructurado de una sesión de entrenamiento.
 *
 * Este widget permite mostrar en forma tabular todos los eventos registrados durante una sesión de entrenamiento,
 * como condiciones biomecánicas, transiciones de estados, errores y observaciones del sistema.
 * La tabla es interactiva y permite ordenar por columnas.
 */

#ifndef LOGVIEWERWIDGET_H
#define LOGVIEWERWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QSharedPointer>
#include "ui/main/uitablemodel.h"
#include "workouts/trainingsesion.h"
#include "core/appcontroller.h"
#include "ui_logviewerwidget.h"

/**
 * @class LogViewerWidget
 * @brief Widget para explorar visualmente el log completo de una sesión de entrenamiento.
 *
 * LogViewerWidget proporciona una tabla ordenable y filtrable que muestra en detalle los registros generados
 * durante la ejecución de un ejercicio. Incluye:
 * - Tipo de evento o condición detectada.
 * - Línea de keypoints implicada.
 * - Estado actual de la máquina de estados.
 * - Marca temporal del evento.
 * - Otros metadatos relevantes.
 *
 * Está conectado a un `AppController` y puede integrarse fácilmente con la interfaz principal del entrenador.
 */
class LogViewerWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param controller Controlador principal de la aplicación.
     * @param parent Widget padre opcional.
     */
    explicit LogViewerWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~LogViewerWidget();

    /**
     * @brief Carga los datos del log asociados a una sesión de entrenamiento.
     * @param sesion Puntero compartido a la sesión (`TrainingSesion`) cuyos eventos se desean visualizar.
     */
    void loadSesion(QSharedPointer<TrainingSesion> sesion);

private slots:
    /**
     * @brief Slot que gestiona el reordenamiento al hacer clic en un encabezado de columna.
     * @param column Índice de la columna clicada.
     */
    void onHeaderClicked(int column);

private:
    Ui_LogViewerWidget *ui; ///< Interfaz gráfica generada con Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.
    QSharedPointer<TrainingSesion> currentSesion; ///< Sesión actualmente cargada.

    UiTableModel* logModel = nullptr; ///< Modelo de datos para la tabla de eventos.
    QSortFilterProxyModel* proxyModel = nullptr; ///< Proxy para permitir ordenación y filtrado dinámico.
};

#endif // LOGVIEWERWIDGET_H
