#include "state.h"
#include <cmath>

State::State(int id, int minTime, int maxTime)
    : id(id), minTime(minTime), maxTime(maxTime)
{
    // Opcional: Validar valores
    if (minTime < 0) this->minTime = 0;
    if (maxTime < minTime) this->maxTime = minTime;

}

int State::getId() const
{
    return id;
}

int State::getMaxTime() const
{
    return maxTime;
}

int State::getMinTime() const
{
    return minTime;
}

int State::getEntryTime() const
{
    return entryTime;
}

void State::setEntryTime(int entryTime)
{
    this->entryTime = entryTime;
}

void State::addAngleConstraint(QString line,AngleConstraint constraint)
{

    constraints.insert(line, constraint);
}



QList<Condition> State::getReport(QHash<QString, double> detectedAngles, int currentTime)
{
    QList<Condition> report={};
    double presviousAngle=-1;
    AngleConstraint line;
    double dif=0;
    bool isOptimo=true;

     if (currentTime-entryTime>maxTime){report.append(Condition(ConditionType::MaxStateTimeout,"" ));isOptimo=false;}
     if (currentTime-entryTime>minTime){report.append(Condition(ConditionType::MinStateTimeout,"" ));isOptimo=false;}

    for (auto it = detectedAngles.constBegin(); it != detectedAngles.constEnd(); ++it) {

        QString lineName = it.key();
        double currentAngle = it.value();

        if (!previousAngles.contains(lineName)) {
            presviousAngle = -1; // Indica que no hay un valor anterior
        } else {
            presviousAngle = previousAngles.value(lineName, -1);
        }

        if (presviousAngle!=-1){

            dif=std::fabs(presviousAngle-currentAngle);
            if(line.getMaxAngle()!=-1 && currentAngle>line.getMaxAngle() && dif>line.getToler()){report.append(Condition(ConditionType::MaxOptAngle,lineName ));}
            if(line.getMinAngle()!=-1 && currentAngle<line.getMinAngle() && dif>line.getToler()){report.append(Condition(ConditionType::MinOptAngle,lineName ));}
            if(line.getMaxSafeAngle()!=-1 && currentAngle>line.getMaxSafeAngle() && dif>line.getToler()){report.append(Condition(ConditionType::JointOverload,lineName ));isOptimo=false;}
            if(line.getMinSafeAngle()!=-1 && currentAngle<line.getMinSafeAngle() && dif>line.getToler()){report.append(Condition(ConditionType::JointOverload,lineName ));isOptimo=false;}


            if (line.getFastThreshold()!=-1 && std::fabs(presviousAngle-currentAngle)>line.getFastThreshold()){report.append(Condition(ConditionType::FastMovement,lineName ));isOptimo=false;}
            if (line.getSlowThreshold()!=-1 && std::fabs(presviousAngle-currentAngle)<line.getSlowThreshold()){report.append(Condition(ConditionType::FastMovement,lineName ));isOptimo=false;}

            switch(line.getEvolution()){

                case Direction::Increase:
                    if(currentAngle<=presviousAngle && dif>line.getToler()){report.append(Condition(ConditionType::Decrease,lineName ));isOptimo=false;}
                    break;
                case Direction::Decrease:
                    if(currentAngle>=presviousAngle && dif>line.getToler()){report.append(Condition(ConditionType::Increase,lineName ));isOptimo=false;}
                    break;
                case Direction::Not_matter:
                    break;
                case Direction::Steady:
                    if(std::fabs(currentAngle-line.getMaxAngle())>line.getToler()){report.append(Condition(ConditionType::Not_Steady,lineName ));isOptimo=false;}
                    break;
                case Direction::Symetrical:
                    if(std::fabs(currentAngle-line.getSymetricalAngle())>line.getToler()){report.append(Condition(ConditionType::symmetryDeviation,lineName ));isOptimo=false;}
                    break;
                default:
                    break;

            }
        }
    }
    if (isOptimo){report.append(Condition(ConditionType::OptimalForm,""));}
    previousAngles = detectedAngles;
    return report;
}

