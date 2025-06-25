/**
 * @file PoseViewEnum.h
 * @brief Enumerado que define las vistas posibles de una pose capturada.
 *
 * Este archivo proporciona el enum `PoseView` que permite identificar la orientación
 * de la cámara o perspectiva desde la cual se ha capturado la imagen de la pose,
 * junto con funciones auxiliares para su conversión entre enum y cadena.
 */

#ifndef POSEVIEWENUM_H
#define POSEVIEWENUM_H

#include <QString>

/**
 * @enum PoseView
 * @brief Representa la perspectiva desde la que se observa la pose.
 *
 * Este enum permite especificar la vista desde la cual se ha capturado la información
 * biomecánica del usuario, lo cual es útil para análisis de movimientos desde diferentes ángulos.
 */
enum class PoseView {
    Front,       ///< Vista frontal
    Left,        ///< Vista lateral izquierda
    Right,       ///< Vista lateral derecha
    top_down,    ///< Vista desde arriba (cenital)
    bottom_up    ///< Vista desde abajo (nadir)
};

/**
 * @brief Convierte un valor `PoseView` a su representación textual.
 * @param view Valor del enum.
 * @return Cadena con el nombre de la vista.
 */
inline QString PoseViewToString(PoseView view) {
    switch (view) {
    case PoseView::Front: return "Front";
    case PoseView::Left: return "Left";
    case PoseView::Right: return "Right";
    case PoseView::top_down: return "top_down";
    case PoseView::bottom_up: return "bottom_up";
    default: return "Front";
    }
}

/**
 * @brief Convierte una cadena textual en un valor del enum `PoseView`.
 * @param str Cadena con el nombre de la vista (case insensitive).
 * @return Valor correspondiente del enum, o `Front` si no se reconoce.
 */
inline PoseView PoseViewFromString(const QString& str) {
    QString s = str.toLower();

    if (s == "front") return PoseView::Front;
    if (s == "left") return PoseView::Left;
    if (s == "right") return PoseView::Right;
    if (s == "top_down") return PoseView::top_down;
    if (s == "bottom_up") return PoseView::bottom_up;

    return PoseView::Front;
}

#endif // POSEVIEWENUM_H
