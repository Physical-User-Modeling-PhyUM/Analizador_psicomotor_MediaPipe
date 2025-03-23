#ifndef CONDITION_H
#define CONDITION_H


#include <QHash>
#include <QString>

// Enumerado para la dirección del movimiento
enum class ConditionType {

    //Condiciones generadas por State
    MaxOptAngle,              //ángulo máximo en la articulación para el ejercicio alcanzado
    MinOptAngle,              //ángulo mínimo en la articilación para el ejercicio alcanzado
    Increase,                 //ángulo está aumentando y debería disminuir
    Decrease,                 //ángulo está disminuyendo y debería aumentar
    Not_Steady,               //No se está manteniendo un ángulo constante
    MaxStateTimeout,          //Se ha superado el timepo máximo en el estado
    MinStateTimeout,          //Se ha superado el tiempo mínimo en el estado
    OptimalForm,              //Ejecución correcta
    FastMovement,             // Movimiento más rápido de lo esperado
    SlowMovement,             //Movimineto más lent de lo esperado
    JointOverload,            //Excesivo esfuerzo en la articulación
    symmetryDeviation,        //Asimetria entre lados del cuerpo
    exerciseInit,             //se inicia el ejercicio

    //Condiciones generadas por StateMachine
    EndOfRepetition,           //Fin de la repetición
    EndOfSet,                  //fin de la serie
    EndOfExercise,              //fin del Ejercicio
    SetTime,                   //Tiempo para la serie completado
    RestTime,                  //tiempo de reposo entre repeticiones no respetado
    RestOverTime,              //Tiempo de reposo excedido
    AccelerationSpikes,        //Movimiento con variaciones en la aceleración
    RangeOfMotionDeficit,      //comparando si MaxOptAngle y MinOptAngle
    ExerciseOvertime           //Se ha superado el timepo máximo para el ejercicio


};
struct Condition {
    ConditionType type;
    QString keypointLine;

    Condition(ConditionType t, const QString &line)
        : type(t), keypointLine(line) {}
};

// Operador de comparación necesario para QSet/QHash
inline bool operator==(const Condition &c1, const Condition &c2) {
    return c1.type == c2.type && c1.keypointLine == c2.keypointLine;
}

// Calcula el hash de un Condition para que Qt pueda almacenarlo correctamente
inline uint qHash(const Condition &key, uint seed = 0) {
    return qHash(static_cast<int>(key.type), seed) ^ qHash(key.keypointLine, seed);
}

#endif // CONDITION_H
