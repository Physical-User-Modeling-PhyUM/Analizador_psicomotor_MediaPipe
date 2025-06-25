/**
 * @file angleviewwidget.h
 * @brief Declaración de la clase AngleViewerWidget, interfaz para visualizar ángulos analizados por el sistema.
 *
 * Este widget permite representar visualmente los ángulos biomecánicos generados por la aplicación durante
 * la ejecución de ejercicios físicos. Facilita al preparador físico o usuario la selección de serie y repetición,
 * mostrando para cada una de ellas tanto la sobrecarga angular como el rango articular observado.
 */

#ifndef ANGLEVIEWERWIDGET_H
#define ANGLEVIEWERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QTableView>
#include <QVBoxLayout>
#include <QLabel>
#include "core/metricsmanager.h"
#include "ui_angleviewwidget.h"

/**
 * @class AngleViewerWidget
 * @brief Widget gráfico para la visualización detallada de ángulos en ejercicios físicos.
 *
 * Este componente de interfaz proporciona al usuario una herramienta de análisis post-ejecución que permite:
 * - Seleccionar una serie capturada.
 * - Elegir una repetición dentro de dicha serie.
 * - Visualizar en tablas separadas la información de sobrecarga angular (`OverloadsTable`) y
 *   el rango de movimiento (`RangesTable`) calculado para la repetición y estado activo.
 *
 * Utiliza un `MetricsManager` para acceder a los datos previamente procesados en la sesión de entrenamiento.
 */
class AngleViewerWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param parent Widget padre opcional.
     */
    explicit AngleViewerWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~AngleViewerWidget();

    /**
     * @brief Establece el gestor de métricas desde el cual se obtendrán los datos angulares.
     * @param manager Puntero al `MetricsManager`.
     */
    void setManager(MetricsManager* manager);

private slots:
    /**
     * @brief Slot ejecutado cuando se selecciona una nueva serie en el ComboBox.
     * @param index Índice de la serie seleccionada.
     */
    void onSerieChanged(int index);

    /**
     * @brief Slot ejecutado al seleccionar una repetición dentro de la tabla.
     * @param index Índice de la repetición seleccionada.
     */
    void onRepSelected(const QModelIndex &index);

private:
    /**
     * @brief Población del ComboBox con las series disponibles.
     */
    void populateSeries();

    /**
     * @brief Carga de las repeticiones pertenecientes a una serie específica.
     * @param serie Índice de la serie.
     */
    void populateReps(int serie);

    /**
     * @brief Muestra la tabla de sobrecarga angular para una repetición específica.
     * @param serie Índice de la serie.
     * @param rep Índice de la repetición.
     */
    void populateOverloadsTable(int serie, int rep);

    /**
     * @brief Muestra la tabla de rangos de movimiento para una repetición específica.
     * @param serie Índice de la serie.
     * @param rep Índice de la repetición.
     */
    void populateRangesTable(int serie, int rep);

    MetricsManager* metricsManager = nullptr; ///< Gestor de métricas con acceso a los datos angulares.
    Ui_AngleViewWidget* ui; ///< Interfaz gráfica generada con Qt Designer.

    int currentSerie = -1; ///< Serie actualmente seleccionada.
};

#endif // ANGLEVIEWERWIDGET_H
