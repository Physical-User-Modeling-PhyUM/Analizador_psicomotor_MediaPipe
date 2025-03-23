#include "feedback.h"

FeedBack::FeedBack(QList<Condition> conds) {}

QList<QString> FeedBack::getCriticalMessages() const
{
    return criticalMessages;
}

QList<QString> FeedBack::getInfoMessages() const
{
    return infoMessages;
}

QList<QString> FeedBack::getAlerts() const
{
    return alerts;
}


void FeedBack::processConditions(QList<Condition> &conds){

    for (const Condition &cond : conds) {



        switch (cond.type) {
            case ConditionType::MaxOptAngle:
                 criticalMessages.append(QString("El ángulo de %1 es demasiado grande. Reduce la amplitud.").arg(cond.keypointLine));
                break;
            case ConditionType::MinOptAngle:
                 criticalMessages.append(QString("El ángulo de %1 es demasiado pequeño. Aumenta la amplitud.").arg(cond.keypointLine));
                 break;
            case ConditionType::Increase:
                infoMessages.append( QString("El movimiento en %1 debe ser más fluido.").arg(cond.keypointLine));
                 break;
            case ConditionType::Decrease:
                infoMessages.append( QString("No dejes que el movimiento en %1 se acelere demasiado.").arg(cond.keypointLine));
                 break;
            case ConditionType::Not_Steady:
                infoMessages.append( QString("Debes mantener la posición en %1 más estable.").arg(cond.keypointLine));
                 break;
            case ConditionType::MaxStateTimeout:
                infoMessages.append( "Has estado demasiado tiempo en esta posición.");
                alerts.append("overtime");
                break;
            case ConditionType::MinStateTimeout:
                infoMessages.append( "Has cambiado de posición demasiado rápido.");
                alerts.append("time");
                 break;
            case ConditionType::OptimalForm:
                infoMessages.append( "¡Buena técnica! Sigue así.");
                alerts.append("good");
                 break;
            case ConditionType::FastMovement:
                infoMessages.append( "El movimiento es demasiado rápido. Controla la velocidad.");
                 break;
            case ConditionType::SlowMovement:
                infoMessages.append( "El movimiento es demasiado lento. Acelera un poco.");
                 break;
            case ConditionType::JointOverload:
                criticalMessages.append( QString("Estas forzando la articulación %1").arg(cond.keypointLine));
                break;
            case ConditionType::AccelerationSpikes:
                criticalMessages.append( QString("Aceleraciones no controladas en la articulación %1").arg(cond.keypointLine));
                break;
            case ConditionType::symmetryDeviation:
                infoMessages.append(QString( "Manten la simetria en el eje %1.").arg(cond.keypointLine));
                break;
            default:
                 break;
            }
        }

}
