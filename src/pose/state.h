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
    explicit State(int id, int minTime = 0, int maxTime = 0);
    //getters and setters
    int getId() const;
    int getMaxTime() const;
    int getMinTime() const;
    int getEntryTime() const;
    void setEntryTime(int entryTime);

    void addAngleConstraint(QString line,AngleConstraint constraint);


    QList<Condition> getReport(QHash<QString, double> detectedAngles, int currenTime) ;




private:
    int id;
    int maxTime, minTime,entryTime;
    QHash<QString, double> previousAngles;
    QHash<QString, AngleConstraint> constraints;
    double getAngle(QString line) const;
    bool isWithinToler(QString line) const;

};

#endif // STATE_H
