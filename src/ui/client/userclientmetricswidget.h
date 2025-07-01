#ifndef USERCLIENTMETRICSWIDGET_H
#define USERCLIENTMETRICSWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QLoggingCategory>
#include <QSharedPointer>
#include <QModelIndex>

#include "core/appcontroller.h"
#include "core/metricsmanager.h"
#include "pose/sesionreport.h"
#include "ui/metrics/heatmapwidget.h"
#include "ui/metrics/timewidget.h"
#include "ui/metrics/angleviewwidget.h"
#include "ui/metrics/logviewerwidget.h"
#include "ui_userclientmetricswidget.h"




class UserClientMetricsWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase.
     * @param ctrl Controlador principal de la aplicación.
     * @param parent Widget padre opcional.
     */
    explicit UserClientMetricsWidget(QSharedPointer<AppController> controller, QWidget* parent);
    /**
     * @brief Destructor.
     */
    ~UserClientMetricsWidget();
    /**
     * @brief Establece el informe de sesión a analizar en los widgets visuales.
     * @param report Instancia de SesionReport con los datos de análisis.
     */
    void setReport(const SesionReport &report);

    /**
     * @brief Actualiza todos los widgets con la información cargada.
     */
    void updateWidgets();

    /**
     * @brief Rellena el widget temporal con los datos de ejecución.
     */
    void populateTimeWidget();

    /**
     * @brief Carga el gráfico de distribución de condiciones (pendiente de implementar).
     */
    void populateConditionDistributionChart();

    /**
     * @brief Carga el widget heatmap con los datos de condiciones temporales por estado.
     */
    void populateHeatmapWidget();
    void setUser(QSharedPointer<Client> user);
private slots:
    /**
     * @brief Acción al seleccionar un cliente desde la tabla.
     * @param row Fila seleccionada.
     */
    void onClienteSelected(int row);

    /**
     * @brief Acción al seleccionar un entrenamiento desde la tabla.
     * @param row Fila seleccionada.
     */
    void onWorkoutSelected(int row);

    /**
     * @brief Acción al seleccionar una sesión desde la tabla.
     * @param row Fila seleccionada.
     */
    void onSesionSelected(int row);

    /**
     * @brief Acción al seleccionar un ejercicio desde la tabla.
     * @param row Fila seleccionada.
     */
    void onExerciseSelected(int row);

    /**
     * @brief Clic en la tabla de ejercicios.
     * @param index Índice de celda clicado.
     */
    void onExercisesTableClicked(const QModelIndex &index);

    /**
     * @brief Clic en la tabla de clientes.
     * @param index Índice de celda clicado.
     */
    void onClienteTableClicked(const QModelIndex &index);

    /**
     * @brief Clic en la tabla de entrenamientos.
     * @param index Índice de celda clicado.
     */
    void onWorkoutTableClicked(const QModelIndex &index);

    /**
     * @brief Clic en la tabla de sesiones.
     * @param index Índice de celda clicado.
     */
    void onSesionTableClicked(const QModelIndex &index);

    /**
     * @brief Cambio de serie seleccionada desde combo box.
     * @param index Índice seleccionado.
     */
    void onSerieChanged(int index);

private:
    Ui_UserClientMetricsWidget *ui;///< Interfaz de usuario generada por Qt Designer.
    QSharedPointer<MetricsManager> metricsManager; ///< Gestor de métricas encargado del procesamiento de datos.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.

    TimeWidget* timeWidget = nullptr; ///< Widget de representación temporal.
    HeatmapWidget* heatmapWidget = nullptr; ///< Widget de condiciones tipo heatmap.
    //ConditionDistributionChart* conditionChart = nullptr; ///< Gráfico de distribución de condiciones (comentado).
    AngleViewerWidget* angleViewer = nullptr; ///< Widget para visualización de ángulos.
    LogViewerWidget* logViewer = nullptr; ///< Widget de visualización del log de la sesión.

    int currentClientId   = -1; ///< ID del cliente actualmente seleccionado.
    int currentWorkoutId  = -1; ///< ID del entrenamiento seleccionado.
    int currentExerciseId = -1; ///< ID del ejercicio seleccionado.
     QSharedPointer<Client> currentUser;
    /**
     * @brief Carga los clientes disponibles y los muestra en la tabla.
     */
    void loadClientes();

    /**
     * @brief Carga los entrenamientos asignados a un cliente.
     * @param idClient ID del cliente.
     */
    void loadWorkoutsForClient(int idClient);

    /**
     * @brief Carga las sesiones disponibles según cliente, entrenamiento y ejercicio.
     * @param idWorkout ID del entrenamiento.
     * @param idClient ID del cliente.
     * @param idEx ID del ejercicio.
     */
    void loadAvailableSesions(int idWorkout, int idClient, int idEx);

    /**
     * @brief Carga los ejercicios asociados a un entrenamiento.
     * @param idWorkout ID del entrenamiento.
     */
    void loadExercisesForWorkout(int idWorkout);
};

#endif // USERCLIENTMETRICSWIDGET_H
