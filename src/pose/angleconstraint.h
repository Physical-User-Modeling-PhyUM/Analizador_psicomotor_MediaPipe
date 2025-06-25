/**
 * @file angleconstraint.h
 * @brief Define la clase AngleConstraint para representar restricciones angulares aplicables a ejercicios.
 *
 * Esta clase permite modelar los rangos angulares permitidos para un movimiento, incluyendo límites mínimos y máximos,
 * tiempos de umbral, tolerancias, evolución esperada del movimiento y la línea de keypoints evaluada. Esta información
 * es utilizada por el sistema para validar biomecánicamente la ejecución de un ejercicio.
 */

#ifndef ANGLECONSTRAINT_H
#define ANGLECONSTRAINT_H
#include "enums/PoseViewEnum.h"
#include <QString>

/// Enumerado que representa la dirección esperada del movimiento durante una fase del ejercicio.
enum class Direction {
    Increase,     ///< El ángulo debe aumentar progresivamente.
    Decrease,     ///< El ángulo debe disminuir progresivamente.
    Steady,       ///< El ángulo debe mantenerse estable.
    Symetrical,   ///< El ángulo debe ser simétrico respecto a un valor de referencia.
    Not_matter    ///< La dirección no es relevante.
};

/// Convierte una cadena de texto en un valor del enumerado Direction.
inline Direction directionFromString(const QString& str) {
    if (str == "Increase") return Direction::Increase;
    if (str == "Decrease") return Direction::Decrease;
    if (str == "Steady") return Direction::Steady;
    if (str == "Symetrical") return Direction::Symetrical;
    return Direction::Not_matter;
}

/// Convierte un valor del enumerado Direction a su representación en texto.
inline QString directionToString(Direction dir) {
    switch (dir) {
    case Direction::Increase: return "Increase";
    case Direction::Decrease: return "Decrease";
    case Direction::Steady: return "Steady";
    case Direction::Symetrical: return "Symetrical";
    case Direction::Not_matter: return "Not_matter";
    }
    return "Not_matter";
}

/**
 * @class AngleConstraint
 * @brief Representa una restricción angular para un estado específico de un ejercicio.
 *
 * Define los límites y condiciones que deben cumplirse durante un movimiento para considerarse válido.
 * Estas restricciones son utilizadas por el sistema de análisis postural para generar retroalimentación en tiempo real.
 */
class AngleConstraint {
public:
    /// Constructor por parámetros con valores por defecto.
    AngleConstraint(
        int idConstraint = -1,
        int IdEx = -1,
        int idState = -1,
        double minA = -1,
        double maxA = -1,
        double maxS = -1,
        double minS = -1,
        int fastT = -1,
        int slowT = -1,
        double SimtricA = -1,
        Direction evo = Direction::Not_matter,
        double tol = -1,
        QString linea = "",
        PoseView view = PoseView::Front);

    // Métodos Getters
    double getMinAngle() const;               ///< Obtiene el ángulo mínimo permitido.
    double getMaxAngle() const;               ///< Obtiene el ángulo máximo permitido.
    int getFastThreshold() const;             ///< Tiempo mínimo para movimiento rápido.
    int getSlowThreshold() const;             ///< Tiempo máximo para movimiento lento.
    Direction getEvolution() const;           ///< Dirección esperada del movimiento.
    double getToler() const;                  ///< Tolerancia aceptable.
    double getSymetricalAngle() const;        ///< Ángulo de referencia para simetría.
    double getMaxSafeAngle() const;           ///< Ángulo máximo seguro.
    void setMaxSafeAngle(double newMaxSafeAngle); ///< Establece el ángulo máximo seguro.
    double getMinSafeAngle() const;           ///< Ángulo mínimo seguro.
    void setMinSafeAngle(double newMinSafeAngle); ///< Establece el ángulo mínimo seguro.
    QString getLinea() const;                 ///< Línea de keypoints evaluada.
    double getAngle() const;                  ///< Alias para obtener el ángulo superior.

    // Métodos Setters
    void setMinAngle(double minA);
    void setMaxAngle(double maxA);
    void setSymetricalAngle(double newSymetricalAngle);
    void setFastThreshold(int fastT);
    void setSlowThreshold(int slowT);
    void setEvolution(Direction evo);
    void setToler(double tol);
    void setLinea(const QString &newLinea);
    void setAngle(double angle);

    int getIdConstraint() const;              ///< Obtiene el identificador de la restricción.
    void setIdConstraint(int newIdConstraint);///< Establece el identificador de la restricción.

    int getIdState() const;                   ///< Obtiene el identificador del estado al que se aplica.
    void setIdState(int newIdState);          ///< Establece el identificador del estado.

    int getIdEx() const;                      ///< Obtiene el identificador del ejercicio asociado.
    void setIdEx(int newIdEx);                ///< Establece el identificador del ejercicio.

    PoseView getView() const;                 ///< Obtiene la vista (lateral, frontal) en la que se aplica la restricción.
    void setView(PoseView newView);           ///< Establece la vista relevante.

private:
    int idConstraint;        ///< ID único de la restricción.
    int idState;             ///< ID del estado del ejercicio al que se aplica.
    int idEx;                ///< ID del ejercicio asociado.
    QString linea;           ///< Línea de keypoints que define el movimiento.
    double minAngle;         ///< Ángulo mínimo permitido.
    double maxAngle;         ///< Ángulo máximo permitido.
    double maxSafeAngle;     ///< Ángulo máximo dentro del margen seguro.
    double minSafeAngle;     ///< Ángulo mínimo dentro del margen seguro.
    int fastThreshold;       ///< Umbral inferior de tiempo para movimientos rápidos.
    int slowThreshold;       ///< Umbral superior de tiempo para movimientos lentos.
    double symetricalAngle;  ///< Ángulo objetivo para movimientos simétricos.
    Direction evolution;     ///< Dirección esperada del movimiento.
    double toler;            ///< Margen de tolerancia.
    PoseView view;           ///< Vista desde la cual se evalúa (frontal, lateral, etc.).
};

#endif // ANGLECONSTRAINT_H
