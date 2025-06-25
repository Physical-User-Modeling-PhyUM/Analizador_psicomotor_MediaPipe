/**
 * @file feedback.cpp
 * @brief Implementación de la clase FeedBack para clasificar condiciones y generar mensajes personalizados.
 *
 * Este archivo define cómo se procesan las condiciones detectadas durante un ejercicio físico, generando
 * mensajes clasificados por severidad (crítica, alerta, informativa) que serán usados en tiempo real
 * para proporcionar retroalimentación al usuario.
 */

#include "feedback.h"
/// Constructor que recibe una lista de condiciones y las procesa para generar entradas clasificadas.

FeedBack::FeedBack(QList<Condition> conds) {

    conditionsMap.resize(static_cast<int>(ConditionType::ExerciseOvertime) + 1);
    conditionsMap.fill(false);
    processConditions(conds);
}

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

/// Procesa cada condición recibida, clasificándola y generando su mensaje asociado.

void FeedBack::processConditions(QList<Condition> &conds) {
    for (const Condition &cond : conds) {
        switch (cond.type) {
        case ConditionType::MaxAngle:
            entries[ConditionType::MaxAngle] = {
                QString("ángulo óptimo máximo alcanzado(%1)").arg(cond.keypointLine),
                ConditionCategory::info
            };
            break;
        case ConditionType::MinAngle:
            entries[ConditionType::MinAngle] = {
                QString("ángulo óptimo mínimo alcanzado(%1)").arg(cond.keypointLine),
                 ConditionCategory::info
            };
            break;
        case ConditionType::Increase:
            entries[ConditionType::Increase] = {
                QString("Has cambiado el sentido del movimiento en el eje %1.").arg(cond.keypointLine),
                 ConditionCategory::alert
            };
            break;
        case ConditionType::Decrease:
            entries[ConditionType::Decrease] = {
                QString("Has cambiado el sentido del movimiento en el eje %1.").arg(cond.keypointLine),
                 ConditionCategory::alert
            };
            break;
        case ConditionType::Has_Stopped:
            entries[ConditionType::Has_Stopped] = {
                QString("Has interrumpido el movimiento en el eje %1.").arg(cond.keypointLine),
                ConditionCategory::alert
            };
            break;
        case ConditionType::OpositeDirection:
            entries[ConditionType::OpositeDirection] = {
                QString("Has cambiado el sentido del movimiento en el eje %1.").arg(cond.keypointLine),
                ConditionCategory::alert
            };
            break;
        case ConditionType::Not_Steady:
            entries[ConditionType::Not_Steady] = {
                QString("Debes mantener la posición en %1 más estable.").arg(cond.keypointLine),
                 ConditionCategory::alert
            };
            break;
        case ConditionType::is_Steady:
            entries[ConditionType::is_Steady] = {
                QString("Estas manteniendo la posicion en %1 correctamente.").arg(cond.keypointLine),
                ConditionCategory::info
            };
            break;
        case ConditionType::MaxStateTimeout:
            entries[ConditionType::MaxStateTimeout] = {
                "Has estado demasiado tiempo en esta posición.",
                 ConditionCategory::alert
            };
            break;
        case ConditionType::MinStateTimeout:
            entries[ConditionType::MinStateTimeout] = {
                "Has cambiado de posición demasiado rápido.",
               ConditionCategory::alert
            };
            break;
        case ConditionType::OptimalForm:
            entries[ConditionType::OptimalForm] = {
                "¡Buena técnica! Sigue así.",
                ConditionCategory::info
            };
            break;
        case ConditionType::FastMovement:
            entries[ConditionType::FastMovement] = {
                "El movimiento es demasiado rápido. Controla la velocidad.",
                ConditionCategory::alert
            };
            break;
        case ConditionType::SlowMovement:
            entries[ConditionType::SlowMovement] = {
                "El movimiento es demasiado lento. Acelera un poco.",
                 ConditionCategory::alert
            };
            break;
        case ConditionType::JointOverload:
            entries[ConditionType::JointOverload] = {
                QString("Estas forzando la articulación en la línea %1").arg(cond.keypointLine),
                 ConditionCategory::critical
            };
            break;
        case ConditionType::symmetryDeviation:
            entries[ConditionType::symmetryDeviation] = {
                QString("Mantén la simetría en el eje %1.").arg(cond.keypointLine),
                ConditionCategory::alert
            };
            break;
        case ConditionType::exerciseInit:
            entries[ConditionType::exerciseInit] = {
                "Ejercicio iniciado!!",
               ConditionCategory::info
            };
            break;
        case ConditionType::EndOfRepetition:
            entries[ConditionType::EndOfRepetition] = {
                "Repetición Completada!!",
                ConditionCategory::info
            };
            break;
        case ConditionType::EndOfSet:
            entries[ConditionType::EndOfSet] = {
                "Serie Completada!!",
                ConditionCategory::info
            };
            break;
        case ConditionType::EndOfExercise:
            entries[ConditionType::EndOfExercise] = {
                "Ejercicio Completado!!",
                ConditionCategory::info
            };
        case ConditionType::EndOfMovementPhase:
            entries[ConditionType::EndOfMovementPhase] = {
                "Fase del movimiento completada!!",
                ConditionCategory::info
            };
            break;
        case ConditionType::SetTime:
            entries[ConditionType::SetTime] = {
                "Se ha alcanzado el tiempo máximo para realizar la serie!!",
                ConditionCategory::info
            };
            break;
        case ConditionType::RestTime:
            entries[ConditionType::RestTime] = {
                "Tiempo de descanso finalizado. ¡Prepárate!!",
                ConditionCategory::info
            };
            break;
        case ConditionType::RestOverTime:
            entries[ConditionType::RestOverTime] = {
                "Tiempo de descanso sobrepasado. ¡Prepárate!!",
                ConditionCategory::alert
            };
            break;
        case ConditionType::AccelerationSpikes:
            entries[ConditionType::AccelerationSpikes] = {
                QString("Aceleraciones no controladas en la articulación %1").arg(cond.keypointLine),
                ConditionCategory::alert
            };
            break;
        case ConditionType::RangeOfMotionDeficit:
            entries[ConditionType::RangeOfMotionDeficit] = {
                QString("Rango de movimiento insuficiente en la articulación %1").arg(cond.keypointLine),
               ConditionCategory::alert
            };
            break;
        case ConditionType::IncorrectExecution:
            entries[ConditionType::IncorrectExecution] = {
                "No se ha realizado el  movimiento de forma correcta.",
                ConditionCategory::alert
            };
        case ConditionType::ExerciseOvertime:
            entries[ConditionType::ExerciseOvertime] = {
                "Se ha superado el tiempo máximo para realizar el ejercicio.",
                ConditionCategory::alert
            };
            break;
        default:
            break;
        }
    }
}

QString FeedBack::getMessage(ConditionType type) const {
    return entries.contains(type) ? entries[type].message : "";
}

ConditionCategory FeedBack::getCategory(ConditionType type) const {
    return entries.contains(type) ? entries[type].category :ConditionCategory::none;
}

bool FeedBack::isCritical(ConditionType type)const
{
    return entries[type].category==ConditionCategory::critical;
}

bool FeedBack::isAlert(ConditionType type) const
{
     return entries[type].category==ConditionCategory::alert;
}

bool FeedBack::isInfo(ConditionType type) const
{
     return entries[type].category==ConditionCategory::info;
}

bool FeedBack::isOptimal(ConditionType type) const
{
    return (type==ConditionType::OptimalForm);
}

QList<ConditionType> FeedBack::getEntriesList() const
{
    return entries.keys();
}



bool FeedBack::contains(ConditionType cond) const {
    return entries.contains(cond);
}
