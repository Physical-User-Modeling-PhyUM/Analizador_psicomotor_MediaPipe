#ifndef ANGLECONSTRAINT_H
#define ANGLECONSTRAINT_H

// Enumerado para la dirección del movimiento
enum class Direction {
    Increase,
    Decrease,
    Steady,
    Symetrical,
    Not_matter
};


class AngleConstraint {
public:

    // Constructor con parámetros
    AngleConstraint(double minA=-1,
                    double maxA=-1,
                    double maxS=-1,
                    double minS=-1,
                    int fastT=-1,
                    int slowT=-1,
                    double SimtricA=-1,
                    Direction evo=Direction::Not_matter,
                    double tol=-1
                    );

    // Métodos Getters
    double getMinAngle() const;
    double getMaxAngle() const ;

    int getFastThreshold() const ;
    int getSlowThreshold() const ;
    Direction getEvolution() const ;
    double getToler() const ;
    double getSymetricalAngle() const;
    double getMaxSafeAngle() const;
    void setMaxSafeAngle(double newMaxSafeAngle);
    double getMinSafeAngle() const;
    void setMinSafeAngle(double newMinSafeAngle);

    // Métodos Setters
    void setMinAngle(double minA) ;
    void setMaxAngle(double maxA) ;
    void setSymetricalAngle(double newSymetricalAngle);
    void setFastThreshold(int fastT);
    void setSlowThreshold(int slowT) ;
    void setEvolution(Direction evo) ;
    void setToler(double tol) ;





private:
    double minAngle;
    double maxAngle;
    double maxSafeAngle;
    double minSafeAngle;
    int fastThreshold;
    int slowThreshold;
    double symetricalAngle;
    Direction evolution;
    double toler;
};

#endif // ANGLECONSTRAINT_H
