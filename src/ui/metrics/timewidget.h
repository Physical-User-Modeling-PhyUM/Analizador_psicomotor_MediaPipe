/**
 * @file timewidget.h
 * @brief Declaración de la clase TimeWidget, interfaz para visualizar métricas temporales de una sesión de entrenamiento.
 *
 * Este widget ofrece una vista detallada de los tiempos asociados a una serie de ejercicios, incluyendo
 * duración activa, descansos, tiempos por estado y posibles sobretiempos. Está diseñado para facilitar
 * el análisis temporal de la ejecución mediante gráficos y tablas.
 */

#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QChartView>
#include <QTableWidget>
#include <QSharedPointer>
#include "pose/sesionreport.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(timeWidget)

/**
 * @class TimeWidget
 * @brief Widget gráfico para visualizar la duración de series, descansos y tiempos de estado en una sesión.
 *
 * Este componente permite:
 * - Seleccionar una serie de entrenamiento.
 * - Visualizar gráficamente los tiempos de ejecución y descanso.
 * - Mostrar sobretiempos en la ejecución o en los descansos.
 * - Consultar los tiempos dedicados a cada estado del ejercicio.
 *
 * Utiliza `SesionReport` para obtener la información y actualiza tanto una tabla de estados como una visualización con `QChart`.
 */
class TimeWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param parent Widget padre opcional.
     */
    explicit TimeWidget(QWidget *parent = nullptr);

    /**
     * @brief Asigna el informe de sesión desde el cual se obtendrán los datos de análisis temporal.
     * @param rep Referencia al objeto `SesionReport` con los datos de la sesión.
     */
    void setReport(const SesionReport& rep);

    /**
     * @brief Establece los segmentos temporales de cada repetición, incluyendo los estados por los que pasa.
     * @param segments Lista de listas de pares (nombre del estado, duración en milisegundos) por repetición.
     */
    void setRepetitionSegments(const QList<QList<QPair<QString, int>>>& segments);

private slots:
    /**
     * @brief Maneja el cambio de serie seleccionada.
     * @param index Índice de la serie seleccionada.
     */
    void onSerieChanged(int index);

private:
    /**
     * @brief Rellena el ComboBox con los nombres de las series disponibles.
     * @param seriesNames Lista de nombres de series.
     */
    void setSeriesList(const QStringList& seriesNames);

    /**
     * @brief Actualiza las métricas visuales y textuales de la serie seleccionada.
     */
    void setSerieMetrics();

    /**
     * @brief Genera o actualiza el gráfico de la serie actual.
     */
    void updateChart();

    /**
     * @brief Rellena la tabla con los tiempos por estado dentro de la repetición actual.
     */
    void updateTable();

    QComboBox* serieSelector; ///< ComboBox para seleccionar la serie a analizar.
    QLabel* labelDuration; ///< Etiqueta que muestra la duración activa de la serie.
    QLabel* labelRest; ///< Etiqueta que muestra el tiempo de descanso.
    QLabel* labelOverSerie; ///< Etiqueta para sobretiempos en ejecución.
    QLabel* labelOverRest; ///< Etiqueta para sobretiempos en descanso.
    QChart* chart; ///< Gráfico de barras o áreas representando los segmentos temporales.
    QChartView* chartView; ///< Vista del gráfico incrustada en el widget.
    QTableWidget* stateTimes; ///< Tabla que muestra el tiempo dedicado a cada estado.
    QList<QList<QPair<QString, int>>> currentSegments; ///< Segmentos por repetición, con nombre de estado y duración.
    QSharedPointer<SesionReport> report; ///< Reporte de sesión con toda la información estructurada.
    QHash<int, QString> stateNames; ///< Asociación de IDs de estados con nombres descriptivos.
};

#endif // TIMEWIDGET_H
