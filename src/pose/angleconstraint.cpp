/**
 * @file angleconstraint.cpp
 * @brief Implementación de la clase AngleConstraint para modelar restricciones angulares en ejercicios físicos.
 *
 * Este archivo contiene las definiciones de los métodos de la clase AngleConstraint, incluyendo constructores,
 * getters y setters. Esta clase es utilizada en el análisis biomecánico de movimientos para validar la ejecución
 * de ejercicios contra umbrales predefinidos.
 */

#include "angleconstraint.h"

/// Constructor de la clase AngleConstraint con inicialización por parámetros.
AngleConstraint::AngleConstraint(int idConstraint,int idState,int idEx,double minA, double maxA, double maxS,
                                 double minS, int fastT, int slowT,double SimtricA, Direction evo, double tol,QString linea,PoseView view)
    :idConstraint(idConstraint), idState(idState),idEx(idEx),minAngle(minA), maxAngle(maxA), maxSafeAngle(maxS), minSafeAngle(minS),
    fastThreshold(fastT), slowThreshold(slowT),symetricalAngle(SimtricA), evolution(evo), toler(tol),linea(linea),view(view) {}

// === Métodos Getters ===

/// Devuelve el ángulo mínimo permitido.
double AngleConstraint::getMinAngle() const { return minAngle; }

/// Devuelve el ángulo máximo permitido.
double AngleConstraint::getMaxAngle() const { return maxAngle; }

/// Devuelve el umbral inferior de tiempo para un movimiento rápido.
int AngleConstraint::getFastThreshold() const { return fastThreshold; }

/// Devuelve el umbral superior de tiempo para un movimiento lento.
int AngleConstraint::getSlowThreshold() const { return slowThreshold; }

/// Devuelve la evolución esperada del movimiento.
Direction AngleConstraint::getEvolution() const { return evolution; }

/// Devuelve el valor de tolerancia definido para el movimiento.
double AngleConstraint::getToler() const { return toler; }

// === Métodos Setters ===

/// Establece el ángulo mínimo permitido.
void AngleConstraint::setMinAngle(double minA) {
    if (minA >= 0) minAngle = minA;
}

/// Establece el ángulo máximo permitido.
void AngleConstraint::setMaxAngle(double maxA) {
    if (maxA >= minAngle) maxAngle = maxA;
}

/// Establece el umbral inferior de tiempo para un movimiento rápido.
void AngleConstraint::setFastThreshold(int fastT) {
    if (fastT >= 0) fastThreshold = fastT;
}

/// Establece el umbral superior de tiempo para un movimiento lento.
void AngleConstraint::setSlowThreshold(int slowT) {
    if (slowT >= 0) slowThreshold = slowT;
}

/// Establece la evolución esperada del movimiento.
void AngleConstraint::setEvolution(Direction evo) {
    evolution = evo;
}

/// Establece la tolerancia permitida.
void AngleConstraint::setToler(double tol) {
    if (tol >= 0) toler = tol;
}

// === Métodos de acceso a IDs ===

int AngleConstraint::getIdConstraint() const { return idConstraint; }
void AngleConstraint::setIdConstraint(int newIdConstraint) { idConstraint = newIdConstraint; }

int AngleConstraint::getIdState() const { return idState; }
void AngleConstraint::setIdState(int newIdState) { idState = newIdState; }

int AngleConstraint::getIdEx() const { return idEx; }
void AngleConstraint::setIdEx(int newIdEx) { idEx = newIdEx; }

// === Vista y Línea ===

/// Devuelve la vista (frontal/lateral) asociada a la restricción.
PoseView AngleConstraint::getView() const { return view; }
void AngleConstraint::setView(PoseView newView) { view = newView; }

/// Devuelve la línea de keypoints evaluada.
QString AngleConstraint::getLinea() const { return linea; }
void AngleConstraint::setLinea(const QString &newLinea) { linea = newLinea; }

// === Simetría y ángulos seguros ===

void AngleConstraint::setSymetricalAngle(double newSymetricalAngle) { symetricalAngle = newSymetricalAngle; }
double AngleConstraint::getSymetricalAngle() const { return symetricalAngle; }

/// Devuelve el ángulo superior (alias de maxAngle).
double AngleConstraint::getAngle() const { return maxAngle; }

/// Devuelve y establece el ángulo máximo seguro.
double AngleConstraint::getMaxSafeAngle() const { return maxSafeAngle; }
void AngleConstraint::setMaxSafeAngle(double newMaxSafeAngle) { maxSafeAngle = newMaxSafeAngle; }

/// Devuelve y establece el ángulo mínimo seguro.
double AngleConstraint::getMinSafeAngle() const { return minSafeAngle; }
void AngleConstraint::setMinSafeAngle(double newMinSafeAngle) { minSafeAngle = newMinSafeAngle; }

/// Establece el ángulo máximo actual si el valor recibido lo supera.
void AngleConstraint::setAngle(double angle) {
    if (angle >= maxAngle) maxAngle = angle;
}
