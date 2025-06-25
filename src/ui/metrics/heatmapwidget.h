/**
 * @file heatmapwidget.h
 * @brief Declaración de la clase HeatmapWidget, interfaz gráfica para visualizar condiciones detectadas en forma de mapa de calor.
 *
 * Este widget permite representar visualmente la aparición de condiciones durante la ejecución de un ejercicio físico,
 * mostrando un mapa de calor donde el eje X representa el tiempo (por ejemplo, en segundos), y el eje Y representa los distintos
 * tipos de condiciones (`ConditionType`). La intensidad del color refleja la frecuencia o duración de aparición.
 */

#ifndef HEATMAPWIDGET_H
#define HEATMAPWIDGET_H

#include <QWidget>
#include <QMap>
#include <QGraphicsScene>
#include "core/metricsmanager.h"
#include "ui_heatmapwidget.h"

/**
 * @class HeatmapWidget
 * @brief Widget para visualizar un mapa de calor de condiciones biomecánicas durante el ejercicio.
 *
 * Este widget facilita el análisis de datos capturados en una serie de ejercicio, permitiendo al usuario:
 * - Seleccionar una serie disponible.
 * - Visualizar la frecuencia de aparición de cada `ConditionType` a lo largo del tiempo.
 * - Identificar visualmente zonas críticas con alta carga, errores técnicos o incumplimiento de parámetros.
 *
 * Utiliza `QGraphicsScene` para dibujar dinámicamente el mapa de calor, asignando colores en función de la magnitud de aparición
 * de cada condición en cada intervalo de tiempo.
 */
class HeatmapWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget de mapa de calor.
     * @param parent Widget padre opcional.
     */
    explicit HeatmapWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~HeatmapWidget();

    /**
     * @brief Dibuja el mapa de calor correspondiente a una serie de ejercicios seleccionada.
     * @param serie Índice de la serie a visualizar.
     */
    void drawHeatmapForSerie(int serie);

public slots:
    /**
     * @brief Asigna el gestor de métricas desde el cual se extraen los datos de condiciones.
     * @param manager Puntero al `MetricsManager`.
     */
    void setManager(MetricsManager *manager);

private slots:
    /**
     * @brief Actualiza el contenido del mapa de calor cuando hay cambios.
     */
    void updateHeatmap();

    /**
     * @brief Maneja el cambio de serie seleccionada en la interfaz.
     * @param index Índice de la nueva serie seleccionada.
     */
    void onSerieChanged(int index);

private:
    Ui_HeatMapWidget *ui; ///< Interfaz gráfica generada con Qt Designer.
    MetricsManager *metricsManager = nullptr; ///< Gestor de métricas encargado del análisis de datos.
    QGraphicsScene *scene = nullptr; ///< Escena gráfica utilizada para renderizar el mapa de calor.
    QList<int> seriesDisponibles; ///< Lista de identificadores de series disponibles para el análisis.

    /**
     * @brief Devuelve un color proporcional al valor dado, en relación al máximo observado.
     * @param value Valor actual de la condición.
     * @param max Valor máximo entre todas las condiciones, usado para normalización.
     * @return Color asignado.
     */
    QColor getValue_Color(int value, int max);
};

#endif // HEATMAPWIDGET_H
