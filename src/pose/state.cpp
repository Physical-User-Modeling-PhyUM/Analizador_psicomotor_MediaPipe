/*!
 * \file state.cpp
 * \brief Implementación de la clase State para representar un estado dentro de una máquina de estados de un ejercicio.
 *
 * La clase State permite definir restricciones angulares específicas (por línea y vista) para un estado de ejecución,
 * evaluar las condiciones detectadas según la pose capturada, y generar un reporte detallado con desviaciones y métricas.
 */
#include "state.h"
#include <cmath>
#include <opencv2/core/hal/interface.h>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(stateLog, "stateLog")
/*!
 * \brief Constructor por defecto. Inicializa el estado con ID y ejercicio inválidos y tiempos en 0.
 */
State::State() : id(-1), idEx(-1), minTime(0), maxTime(0) {}

/*!
 * \brief Constructor con parámetros.
 * \param id ID único del estado.
 * \param idEx ID del ejercicio al que pertenece el estado.
 * \param minTime Tiempo mínimo permitido en milisegundos.
 * \param maxTime Tiempo máximo permitido en milisegundos.
 * \param name Nombre identificativo del estado.
 */
State::State(int id, int idEx, int minTime, int maxTime, QString name)
    : id(id),idEx(idEx), minTime(minTime), maxTime(maxTime),name(name)
{

    if (minTime < 0) this->minTime = 0;
    if (maxTime < minTime) this->maxTime = minTime;

}
/*!
 * \brief Devuelve el ID del estado.
 */
int State::getId() const
{
    return id;
}
/*!
 * \brief Devuelve el tiempo máximo permitido para este estado.
 */
int State::getMaxTime() const
{
    return maxTime;
}
/*!
 * \brief Devuelve el tiempo mínimo requerido en este estado.
 */
int State::getMinTime() const
{
    return minTime;
}
/*!
 * \brief Devuelve el tiempo de entrada registrado al entrar en el estado.
 */
int State::getEntryTime() const
{
    return entryTime;
}
/*!
 * \brief Establece el tiempo de entrada al estado.
 * \param entryTime Tiempo en milisegundos.
 */
void State::setEntryTime(int entryTime)
{
    this->entryTime = entryTime;
}


/*!
 * \brief Añade una nueva restricción angular para una línea concreta.
 * \param line Nombre de la línea (conexión entre keypoints).
 * \param constraint Objeto AngleConstraint con los parámetros definidos.
 */

void State::addAngleConstraint(QString line,AngleConstraint constraint)
{
    constraint.setIdState(id);
    constraint.setIdEx(idEx);
    constraint.setIdConstraint(constraints.values().size());
    constraint.setLinea(line);
    //qDebug()<<"Se ha agregado una constraint"<<line;
    constraints.insert(line, constraint);
}
/*!
 * \brief Elimina la restricción asociada a una línea.
 * \param line Nombre de la línea a eliminar.
 */
void State::delAngleConstraint(QString line){

    constraints.remove(line);
}
/*!
 * \brief Genera un reporte de condiciones sin actualizar acumuladores.
 * \param detectedAngles Ángulos detectados en la pose.
 * \param currentTime Tiempo actual en milisegundos.
 * \param view Vista (frontal/lateral).
 * \return Lista de condiciones detectadas.
 */
QList<Condition> State::getReport(QHash<QString, double> detectedAngles, int currentTime, PoseView view) {
    QHash<QString, QPair<double, double>> dummy;
    QHash<QString, double> dummyO;
    return getReport(detectedAngles, currentTime, view, dummy,dummyO);
}
/*!
 * \brief Genera un reporte detallado de condiciones, incluyendo actualización de acumuladores de rangos y sobrecargas.
 *
 * Este método evalúa todas las restricciones angulares en función de:
 * - Evolución esperada del movimiento.
 * - Umbrales de velocidad.
 * - Límites seguros (sobre o subutilización articular).
 * - Estabilidad de la ejecución (variabilidad).
 * - Simetría esperada del movimiento.
 *
 * \param detectedAngles Ángulos detectados de la pose.
 * \param currentTime Tiempo actual en milisegundos.
 * \param view Vista (frontal/lateral).
 * \param rangeAccumulator Acumulador que guarda min/max global de cada línea.
 * \param overloadsAccumulator Acumulador que guarda valores de sobrecarga.
 * \return Lista de condiciones detectadas en este estado.
 */

QList<Condition> State::getReport(QHash<QString, double> detectedAngles, int currentTime,PoseView view,
                                  QHash<QString, QPair<double, double>>& rangeAccumulator,
                                  QHash<QString, double>& overloadsAccumulator)
{

    QList<Condition> report={};
    double presviousAngle=-1;
    AngleConstraint line;
    double dif=0;
    bool isOptimo=true;
    if (timeLastFrame==0) timeLastFrame=currentTime;

    qDebug (stateLog)<<"ángulos detectados:"<<detectedAngles.size();
    int64_t timeDif=  currentTime-entryTime;
     if (maxTime>0 && timeDif>=maxTime){
        report.append(Condition(ConditionType::MaxStateTimeout,QString::number(id),timeDif- maxTime));
         isOptimo=false;
        qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxStateTimeout)<<"State:"<<id<<" ,currentTime:"
                 <<currentTime<<"entryTime:"<<entryTime
                 <<"maxTime:"<<maxTime<<"dif"<<currentTime-entryTime;
     }
     if (minTime>0 && timeDif>=minTime){
         report.append(Condition(ConditionType::MinStateTimeout,QString::number(id),timeDif-minTime ));
         isOptimo=false;
         qDebug(stateLog)<<conditionTypeToString(ConditionType::MinStateTimeout)<<"State:"<<id<<" ,currentTime:"
                  <<currentTime<<"entryTime:"<<entryTime
                  <<"minTime:"<<minTime<<"dif"<<currentTime-entryTime;
     }


    for (auto it = detectedAngles.constBegin(); it != detectedAngles.constEnd(); ++it) {

        QString lineName = it.key();
        double currentAngle = it.value();

        // Actualizamos los ángulos detectados de tal forma que siempre tenemos el máximo y el mínimo
        QPair<double, double>& range = rangeAccumulator[lineName];
        if (range.first !=0) range.first=currentAngle;
         if (range.second !=0) range.second=currentAngle;
        if (range.first > currentAngle) range.first = currentAngle;
        if (range.second < currentAngle) range.second = currentAngle;



        for(AngleConstraint line:constraints){

            double sym = line.getSymetricalAngle();
            double min = line.getMinAngle();
            double max = line.getMaxAngle();

                if (line.getLinea()!=lineName || line.getView() != view) continue;
                //AngleConstraint line = constraints[lineName];

                qDebug (stateLog)<<"buscando line:"<<lineName<<", currentAngle"<<currentAngle;

                if (!previousAngles.contains(lineName)) {
                    presviousAngle = -1; // Indica que no hay un valor anterior
                } else {
                    presviousAngle = previousAngles.value(lineName, -1);
                }

                if (presviousAngle!=-1){

                    dif=std::fabs(presviousAngle-currentAngle);
                    double toler=(line.getToler()==-1)? 0:line.getToler();


                    if(line.getMaxSafeAngle()!=-1 && currentAngle>line.getMaxSafeAngle()
                        && fabs(currentAngle-line.getMaxSafeAngle())>toler){
                        report.append(Condition(ConditionType::JointOverload,lineName ,currentAngle, view));
                        isOptimo=false;
                        qDebug(stateLog)<<conditionTypeToString(ConditionType::JointOverload)<<"currentAngle:"
                             <<currentAngle<<"maxSafeAngle:"<<line.getMaxSafeAngle()
                             <<"toler:"<<toler<<"dif"<<dif;
                }
                if(line.getMinSafeAngle()!=-1 && currentAngle<line.getMinSafeAngle()
                    && fabs(currentAngle-line.getMinSafeAngle())>toler)
                {
                    report.append(Condition(ConditionType::JointOverload,lineName ,currentAngle, view));
                    isOptimo=false;
                    overloadsAccumulator.insert(lineName,currentAngle);
                    qDebug(stateLog)<<conditionTypeToString(ConditionType::JointOverload)<<"currentAngle:"
                             <<currentAngle<<"minSafeAngle:"<<line.getMinSafeAngle()
                             <<"toler:"<<toler<<"dif"<<dif;
                }

                //EL fastthrreshold lo tendremos en grados por segundo
                if (line.getFastThreshold()!=-1 && (std::fabs(presviousAngle-currentAngle)/(currentTime-timeLastFrame)*1000)>line.getFastThreshold()){
                    float vel= std::fabs(presviousAngle-currentAngle)/(currentTime-timeLastFrame)*1000;
                    report.append(Condition(ConditionType::FastMovement,lineName,vel-line.getFastThreshold(),view));
                    isOptimo=false;
                    qDebug(stateLog)<<conditionTypeToString(ConditionType::FastMovement)<<"currentAngle:"
                             <<currentAngle<<"presviousAngle:"<<presviousAngle
                                     <<"FastThreshold:"<<line.getFastThreshold()<<"dif"<<presviousAngle-currentAngle<<"vel"<<vel;
                }
                if (line.getSlowThreshold()!=-1 && (std::fabs(presviousAngle-currentAngle)/(currentTime-timeLastFrame)*1000)<line.getSlowThreshold()){
                     float vel= std::fabs(presviousAngle-currentAngle)/(currentTime-timeLastFrame);
                    report.append(Condition(ConditionType::SlowMovement,lineName,line.getSlowThreshold()-vel,view));
                    isOptimo=false;
                    qDebug(stateLog)<<conditionTypeToString(ConditionType::SlowMovement)<<"currentAngle:"
                             <<currentAngle<<"presviousAngle:"<<presviousAngle
                             <<"SlowThreshold:"<<line.getSlowThreshold()<<"dif"<<presviousAngle-currentAngle<<"vel"<<vel;
                }

                switch(line.getEvolution()){

                case Direction::Increase:
                {
                        if(currentAngle>presviousAngle && dif>toler){
                            report.append(Condition(ConditionType::Increase,lineName,dif,view ));
                            //isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::Increase)<<"currentAngle:"
                                     <<currentAngle<<"presviousAngle:"<<presviousAngle
                                     <<"toler:"<<toler<<"dif"<<dif;
                        }else if(currentAngle<presviousAngle && dif>toler){
                                report.append(Condition(ConditionType::OpositeDirection,lineName, dif,view ));
                                isOptimo=false;
                                qDebug(stateLog)<<conditionTypeToString(ConditionType::OpositeDirection)<<"currentAngle:"
                                                 <<currentAngle<<"presviousAngle:"<<presviousAngle
                                                 <<"toler:"<<toler<<"dif"<<dif;
                        }else if(dif<1 && timeLastFrame-currentTime>=stallTime){
                                    report.append(Condition(ConditionType::Has_Stopped,lineName,currentAngle, view ));
                                    isOptimo=false;
                                    qDebug(stateLog)<<conditionTypeToString(ConditionType::Has_Stopped)<<"currentAngle:"
                                                     <<currentAngle<<"presviousAngle:"<<presviousAngle
                                                     <<"toler:"<<toler<<"dif"<<dif;
                        }
                        if (line.getMaxAngle() != -1 && currentAngle > max) {
                            report.append(Condition(ConditionType::MaxAngle, lineName,currentAngle, view));
                            qDebug(stateLog) << "MaxAngle (Increase)" << currentAngle << "Max Angle"<< max;
                        }
                        if (line.getMinAngle() != -1 && currentAngle > min ){
                            report.append(Condition(ConditionType::MinAngle, lineName, currentAngle, view));
                            qDebug(stateLog) << "MaxAngle (Increase)" << currentAngle << "Min Angle"<< min;
                        }


                        break;
                    }
                case Direction::Decrease:{
                        if(currentAngle<presviousAngle && dif>toler){
                            report.append(Condition(ConditionType::Decrease,lineName ,dif, view));
                            //isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::Decrease)<<"currentAngle:"
                                     <<currentAngle<<"presviousAngle:"<<presviousAngle
                                     <<"toler:"<<toler<<"dif"<<dif;
                        }else if(currentAngle>presviousAngle && dif>toler){
                                report.append(Condition(ConditionType::OpositeDirection,lineName ,dif, view));
                                isOptimo=false;
                                qDebug(stateLog)<<conditionTypeToString(ConditionType::OpositeDirection)<<"currentAngle:"
                                                 <<currentAngle<<"presviousAngle:"<<presviousAngle
                                                 <<"toler:"<<toler<<"dif"<<dif;
                         }else if(dif<1 && timeLastFrame-currentTime>=stallTime){
                        report.append(Condition(ConditionType::Has_Stopped,lineName,currentAngle, view));
                        isOptimo=false;
                        qDebug(stateLog)<<conditionTypeToString(ConditionType::Has_Stopped)<<"currentAngle:"
                                         <<currentAngle<<"presviousAngle:"<<presviousAngle
                                         <<"toler:"<<toler<<"dif"<<dif;
                        }
                         if (line.getMaxAngle() != -1 && currentAngle < max) {
                             report.append(Condition(ConditionType::MaxAngle, lineName,currentAngle, view));
                             qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxAngle)<<"currentAngle:"
                                              <<currentAngle<<"Max angle:"<<max;

                         }

                         if (line.getMinAngle() != -1 && currentAngle < min) {
                             report.append(Condition(ConditionType::MinAngle, lineName,currentAngle, view));
                             qDebug(stateLog)<<conditionTypeToString(ConditionType::MinAngle)<<"currentAngle:"
                                              <<currentAngle<<"Min angle:"<<min;
                         }

                        break;
                }
                    case Direction::Not_matter:
                        break;
                    case Direction::Steady:{
                        if(dif>toler){
                            report.append(Condition(ConditionType::Not_Steady,lineName,dif, view ));
                            isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::Not_Steady)<<"currentAngle:"
                                     <<currentAngle<<"previous angle:"<<presviousAngle
                                     <<"toler:"<<toler<<"dif"<<dif;
                        }
                        if(currentAngle<presviousAngle && dif>toler){
                            report.append(Condition(ConditionType::Decrease,lineName,dif, view ));
                            //isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::Decrease)<<"currentAngle:"
                                             <<currentAngle<<"presviousAngle:"<<presviousAngle
                                             <<"toler:"<<toler<<"dif"<<dif;
                        }else if(currentAngle>presviousAngle && dif>toler){
                            report.append(Condition(ConditionType::Increase,lineName,dif, view ));
                            //isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::Increase)<<"currentAngle:"
                                             <<currentAngle<<"presviousAngle:"<<presviousAngle
                                             <<"toler:"<<toler<<"dif"<<dif;
                        }else if(dif<=toler){
                            report.append(Condition(ConditionType::is_Steady,lineName,currentAngle, view ));
                            isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::is_Steady)<<"currentAngle:"
                                             <<currentAngle<<"previous angle:"<<presviousAngle
                                             <<"toler:"<<toler<<"dif"<<dif;
                        }

                        break;

                    }

                    case Direction::Symetrical:{
                        if(std::fabs(currentAngle-line.getSymetricalAngle())>toler){
                            report.append(Condition(ConditionType::symmetryDeviation,lineName,currentAngle-line.getSymetricalAngle(),view ));
                            isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::symmetryDeviation)<<"currentAngle:"
                                     <<currentAngle<<"angle:"<<line.getSymetricalAngle()
                                     <<"toler:"<<toler<<"dif"<<currentAngle-line.getSymetricalAngle();
                        }
                        if(currentAngle<presviousAngle && dif>toler){
                            report.append(Condition(ConditionType::Decrease,lineName ,dif, view));
                            //isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::Decrease)<<"currentAngle:"
                                             <<currentAngle<<"presviousAngle:"<<presviousAngle
                                             <<"toler:"<<toler<<"dif"<<dif;
                        }else if(currentAngle>presviousAngle && dif>toler){
                            report.append(Condition(ConditionType::Increase,lineName,dif, view ));
                            //isOptimo=false;
                            qDebug(stateLog)<<conditionTypeToString(ConditionType::Increase)<<"currentAngle:"
                                             <<currentAngle<<"presviousAngle:"<<presviousAngle
                                             <<"toler:"<<toler<<"dif"<<dif;
                        }
                         double sym = line.getSymetricalAngle();
                         double min = line.getMinAngle();
                         double max = line.getMaxAngle();

                        if (min != -1 && max != -1) {
                            if (sym < min) {
                                if (currentAngle >= min) {
                                    report.append(Condition(ConditionType::MinAngle, lineName,currentAngle, view));

                                    qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxAngle)<<"currentAngle:"
                                                      <<currentAngle<<"maxAngle:"<<max
                                                      <<"toler:"<<toler<<"dif"<<fabs(currentAngle-max);

                                }
                                if (currentAngle >= max) {
                                    report.append(Condition(ConditionType::MaxAngle, lineName,currentAngle, view));
                                    qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxAngle)<<"currentAngle:"
                                                     <<currentAngle<<"Max angle:"<<max;
                                }
                            } else if (sym > max) {
                                if (currentAngle <= min) {
                                    report.append(Condition(ConditionType::MinAngle, lineName,currentAngle, view));
                                    qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxAngle)<<"currentAngle:"
                                                     <<currentAngle<<"Min angle:"<<min;
                                }
                                if (currentAngle <= max) {
                                    report.append(Condition(ConditionType::MaxAngle, lineName,currentAngle, view));
                                    qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxAngle)<<"currentAngle:"
                                                    <<currentAngle<<"Max angle:"<<max;
                                }
                            } else {
                                if (currentAngle < min) {
                                    report.append(Condition(ConditionType::MinAngle, lineName,currentAngle, view));
                                    qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxAngle)<<"currentAngle:"
                                                     <<currentAngle<<"Min angle:"<<min;
                                }
                                if (currentAngle > max) {
                                    report.append(Condition(ConditionType::MaxAngle, lineName,currentAngle, view));
                                    qDebug(stateLog)<<conditionTypeToString(ConditionType::MaxAngle)<<"currentAngle:"
                                                    <<currentAngle<<"Max angle:"<<max;
                                }
                            }
                        }
                    }
                        break;
                    default:
                        break;

                }
        }
    }

    }

    if (isOptimo){report.append(Condition(ConditionType::OptimalForm,"",id, view));}


    timeLastFrame=currentTime;
    if (detectedAngles.size()>0) previousAngles = detectedAngles;
    return report;

}
/*!
 * \brief Devuelve todas las restricciones angulares del estado.
 * \return QHash con las restricciones organizadas por línea.
 */
QHash<QString, AngleConstraint> State::getConstraints() const
{
    return constraints;
}
/*!
 * \brief Actualiza o reemplaza una restricción existente para una línea dada.
 * \param key Línea a modificar.
 * \param constraint Nueva restricción a aplicar.
 */
void State::updateConstraint( QString key,  AngleConstraint& constraint)
{
    constraints[key] = constraint;
}


/*!
 * \brief Devuelve el ID del ejercicio asociado a este estado.
 */
int State::getIdEx() const
{
    return idEx;
}
/*!
 * \brief Asigna el ID del ejercicio para este estado.
 * \param newIdEx Nuevo identificador.
 */
void State::setIdEx(int newIdEx)
{
    idEx = newIdEx;
}
/*!
 * \brief Asigna el tiempo máximo permitido en el estado.
 * \param newMaxTime Tiempo máximo en milisegundos.
 */
void State::setMaxTime(int newMaxTime)
{
    maxTime = newMaxTime;
}
/*!
 * \brief Asigna el tiempo mínimo requerido en el estado.
 * \param newMinTime Tiempo mínimo en milisegundos.
 */
void State::setMinTime(int newMinTime)
{
    minTime = newMinTime;
}
/*!
 * \brief Devuelve el nombre identificativo del estado.
 */
QString State::getName() const
{
    return name;
}
/*!
 * \brief Asigna un nuevo nombre descriptivo al estado.
 * \param newName Nombre a establecer.
 */
void State::setName(const QString &newName)
{
    name = newName;
}
/*!
 * \brief Asigna un nuevo ID al estado.
 * \param newId Identificador numérico.
 */
void State::setId(int newId)
{
    id = newId;
}
/*!
 * \brief Establece un nuevo conjunto de restricciones angulares para el estado.
 * \param newConstraints QHash con las restricciones por línea.
 */
void State::setConstraints(const QHash<QString, AngleConstraint>& newConstraints)
{
    constraints = newConstraints;
}





