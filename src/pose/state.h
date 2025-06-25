#ifndef STATE_H
#define STATE_H

#include <QMap>
#include <QLoggingCategory>
#include <QPair>
#include "condition.h"
#include "angleconstraint.h"


Q_DECLARE_LOGGING_CATEGORY(EstateLog);



class State
{

public:
    State();
    explicit State(int id,int idEx=-1, int minTime = 0, int maxTime = 0, QString name ="none");
    //getters and setters
    int getId() const;
    void setId(int newId);
    int getMaxTime() const;
    int getMinTime() const;
    int getEntryTime() const;
    void setEntryTime(int entryTime);

    void addAngleConstraint(QString line,AngleConstraint constraint);
    void delAngleConstraint(QString line);

    QList<Condition> getReport(QHash<QString,
                               double> detectedAngles,
                               int currentTime,
                               PoseView view=PoseView::Front);

    QList<Condition> getReport(QHash<QString, double> detectedAngles,
                               int currentTime,
                               PoseView view,
                               QHash<QString, QPair<double, double>>& rangeAccumulator,
                               QHash<QString, double>& overloadsAccumulator);



    void setConstraints(const QHash<QString, AngleConstraint> &newConstraints);
    void updateConstraint( QString key,  AngleConstraint& constraint);
    QHash<QString, AngleConstraint> getConstraints() const;



    int getIdEx() const;
    void setIdEx(int newIdEx);

    void setMaxTime(int newMaxTime);

    void setMinTime(int newMinTime);


    QString getName() const;
    void setName(const QString &newName);

private:
    int id;
    int idEx;
    QString name="none";
    int stallTime=500;
    int timeLastFrame=0;
    int maxTime, minTime,entryTime;
    QHash<QString, double> previousAngles;
    QHash<QString, AngleConstraint> constraints;
    double getAngle(QString line) const;
    bool isWithinToler(QString line) const;

};

/**
 * @brief Operador de igualdad para QSet/QHash: compara por ID del estado.
 */
inline bool operator==(const State& s1, const State& s2) {
    return (s1.getId() == s2.getId()) && (s1.getIdEx()==s2.getIdEx());
}

/**
 * @brief Función de hash para usar State en QSet/QHash, basada en su ID.
 */
inline uint qHash(const State& s, uint seed = 0) {
    return qHash(s.getId(), seed);
}

inline bool operator<(const State& s1, const State& s2) {
    return s1.getId() < s2.getId();
}

#endif // STATE_H
