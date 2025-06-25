/// @brief Declaración de la clase FeedBackGraphWidget para la visualización gráfica del feedback de condiciones.

#ifndef FEEDBACKGRAPHWIDGET_H
#define FEEDBACKGRAPHWIDGET_H

#include <QWidget>
#include <QHash>
#include "pose/feedback.h"

/// @class FeedBackGraphWidget
/// @brief Widget gráfico que representa visualmente el resumen de condiciones de un ejercicio.
///
/// Muestra una gráfica circular tipo donut con porcentajes de condiciones óptimas, de alerta y críticas.
/// Esta visualización permite una rápida comprensión del estado general del rendimiento.
class FeedBackGraphWidget : public QWidget
{
    Q_OBJECT
public:
    /// @brief Constructor del widget.
    /// @param parent Widget padre (por defecto nullptr).
    explicit FeedBackGraphWidget(QWidget *parent = nullptr);

    /// @brief Actualiza los datos del gráfico a partir de un objeto FeedBack.
    /// @param feedback Estructura con las condiciones generadas durante el análisis del ejercicio.
    void updateFromFeedback(const FeedBack& feedback);

protected:
    /// @brief Evento de pintura del widget.
    /// Se encarga de renderizar la gráfica circular con las proporciones de condiciones.
    /// @param event Evento de pintura recibido.
    void paintEvent(QPaintEvent *event) override;

private:
    int optimalCount = 0;   ///< Número de condiciones clasificadas como óptimas.
    int alertCount = 0;     ///< Número de condiciones clasificadas como alertas.
    int criticalCount = 0;  ///< Número de condiciones clasificadas como críticas.
};

#endif // FEEDBACKGRAPHWIDGET_H
