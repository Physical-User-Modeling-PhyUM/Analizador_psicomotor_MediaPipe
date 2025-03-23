#include "angleconstraint.h"



AngleConstraint::AngleConstraint(double minA, double maxA, double maxS, double minS, int fastT, int slowT,double SimtricA, Direction evo, double tol)
    : minAngle(minA), maxAngle(maxA), maxSafeAngle(maxS), minSafeAngle(minS),
    fastThreshold(fastT), slowThreshold(slowT),symetricalAngle(SimtricA), evolution(evo), toler(tol) {}

// Métodos Getters
double AngleConstraint::getMinAngle() const { return minAngle; }
double AngleConstraint::getMaxAngle() const { return maxAngle; }

int AngleConstraint::getFastThreshold() const { return fastThreshold; }
int AngleConstraint::getSlowThreshold() const { return slowThreshold; }
Direction AngleConstraint::getEvolution() const { return evolution; }
double AngleConstraint::getToler() const { return toler; }

// Métodos Setters con validaciones
void AngleConstraint::setMinAngle(double minA) {
    if (minA >= 0) minAngle = minA;
}

void AngleConstraint::setMaxAngle(double maxA) {
    if (maxA >= minAngle) maxAngle = maxA;
}



void AngleConstraint::setFastThreshold(int fastT) {
    if (fastT >= 0) fastThreshold = fastT;
}

void AngleConstraint::setSlowThreshold(int slowT) {
    if (slowT >= 0) slowThreshold = slowT;
}

void AngleConstraint::setEvolution(Direction evo) {
    evolution = evo;
}

void AngleConstraint::setToler(double tol) {
    if (tol >= 0) toler = tol;
}

void AngleConstraint::setSymetricalAngle(double newSymetricalAngle)
{
    symetricalAngle = newSymetricalAngle;
}

double AngleConstraint::getSymetricalAngle() const
{
    return symetricalAngle;
}

double AngleConstraint::getMaxSafeAngle() const
{
    return maxSafeAngle;
}

void AngleConstraint::setMaxSafeAngle(double newMaxSafeAngle)
{
    maxSafeAngle = newMaxSafeAngle;
}

double AngleConstraint::getMinSafeAngle() const
{
    return minSafeAngle;
}

void AngleConstraint::setMinSafeAngle(double newMinSafeAngle)
{
    minSafeAngle = newMinSafeAngle;
}
