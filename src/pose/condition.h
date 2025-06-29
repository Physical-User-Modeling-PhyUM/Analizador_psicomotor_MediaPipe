/**
 * @file condition.h
 * @brief Define el tipo de datos Condition utilizado para representar eventos biomecánicos detectados durante el ejercicio.
 *
 * Este archivo incluye la definición del enumerado ConditionType, la estructura Condition y funciones auxiliares
 * para conversión y comparación. Representa eventos como sobrecarga, desviación de simetría, tiempos límite superados, etc.
 * Utilizado para generar feedback al usuario en tiempo real.
 */

#ifndef CONDITION_H
#define CONDITION_H

#include "enums/PoseViewEnum.h"
#include <QHash>
#include <QString>
#include <QVariant>

/**
 * @enum ConditionType
 * @brief Enum que representa los distintos tipos de condiciones detectables durante la ejecución de un ejercicio.
 */
enum class ConditionType {
    // Generadas por el estado
    MaxAngle,              ///< Se ha alcanzado el ángulo máximo objetivo.
    MinAngle,              ///< Se ha alcanzado el ángulo mínimo objetivo.
    Increase,              ///< El ángulo está aumentando.
    Decrease,              ///< El ángulo está disminuyendo.
    OpositeDirection,      ///< El movimiento va en dirección contraria a la esperada.
    Has_Stopped,           ///< Se ha detenido el movimiento en la articulación.
    is_Steady,             ///< El ángulo se mantiene constante correctamente.
    Not_Steady,            ///< No se mantiene la posición deseada.
    MaxStateTimeout,       ///< Se ha superado el tiempo máximo en el estado.
    MinStateTimeout,       ///< Cambio de estado demasiado rápido.
    OptimalForm,           ///< Ejecución correcta.
    FastMovement,          ///< Movimiento demasiado rápido.
    SlowMovement,          ///< Movimiento demasiado lento.
    JointOverload,         ///< Exceso de carga en la articulación.
    symmetryDeviation,     ///< Asimetría entre lados del cuerpo.
    exerciseInit,          ///< Inicio del ejercicio.

    // Generadas por la máquina de estados
    InitRepetition,         ///< coimenzo de una repetición.
    EndOfRepetition,       ///< Fin de una repetición.
    InitSet,                ///< comienzo de una serie.
    EndOfSet,              ///< Fin de una serie.
    EndOfExercise,         ///< Fin del ejercicio.
    SetTime,               ///< Tiempo asignado a la serie alcanzado.
    RestTime,              ///< Tiempo de descanso concluido.
    RestOverTime,          ///< Tiempo de descanso excedido.
    EndOfMovementPhase,    ///< Fin de la fase del movimiento.
    AccelerationSpikes,    ///< Pico de aceleración detectado.
    RangeOfMotionDeficit,  ///< Rango de movimiento insuficiente.
    IncorrectExecution,    ///< Movimiento ejecutado incorrectamente.
    ExerciseOvertime       ///< Tiempo total del ejercicio superado.
};

/**
 * @struct Condition
 * @brief Representa una condición específica ocurrida durante el ejercicio.
 */
struct Condition {
    ConditionType type;     ///< Tipo de condición.
    QString keypointLine;   ///< Línea de keypoints a la que afecta.
    PoseView view;          ///< Vista (frontal/lateral) desde la cual se ha detectado.
    QVariant value;         ///< Valor cuantitativo adicional, si lo hubiera.

    /**
     * @brief Constructor de Condition.
     * @param t Tipo de condición.
     * @param line Línea de puntos clave afectada.
     * @param value Valor asociado (opcional).
     * @param view Vista desde la que se detecta (por defecto Frontal).
     */
    Condition(ConditionType t, const QString &line, QVariant value = 0, PoseView view = PoseView::Front)
        : type(t), keypointLine(line), value(value), view(view) {}
};
/**
 * @brief Convierte una cadena de texto a su correspondiente ConditionType.
 * @param str Cadena a convertir.
 * @return Valor de enumerado ConditionType.
 */

inline ConditionType conditionTypeFromString(const QString& str) {
    QString s=str.toLower();
        if (s == "maxangle") return ConditionType::MaxAngle;
        if (s == "minangle")  return ConditionType::MinAngle;
        if (s == "increase") return ConditionType::Increase;
        if (s == "decrease")   return ConditionType::Decrease;
        if (s == "opositedirection")   return ConditionType::OpositeDirection;
        if (s == "has_stopped")   return ConditionType::Has_Stopped;
        if (s == "not_steady") return ConditionType::Not_Steady;
         if (s == "is_steady") return ConditionType::is_Steady;
        if (s == "maxstatetimeout") return ConditionType::MaxStateTimeout;
        if (s == "minstatetimeout") return ConditionType::MinStateTimeout;
        if (s == "optimalform") return ConditionType::OptimalForm;
        if (s == "fastmovement")    return ConditionType::FastMovement;
        if (s == "slowmovement") return ConditionType::SlowMovement;
        if (s == "jointoverload") return ConditionType::JointOverload;
        if (s == "symmetrydeviation") return ConditionType::symmetryDeviation;
        if (s == "exerciseinit")  return ConditionType::exerciseInit;
        if (s == "endofrepetition") return ConditionType::EndOfRepetition;
         if (s == "initrepetition") return ConditionType::InitRepetition;
          if (s == "initset") return ConditionType::InitSet;
        if (s == "endofset") return ConditionType::EndOfSet;
        if (s == "endofexercise") return ConditionType::EndOfExercise;
        if (s == "settime") return ConditionType::SetTime;
        if (s == "resttime") return ConditionType::RestTime;
          if (s == "endofmovementphase") return ConditionType::EndOfMovementPhase;
        if (s == "restovertime") return ConditionType::RestOverTime;
        if (s == "accelerationspikes")    return ConditionType::AccelerationSpikes;
        if (s == "rangeofmotiondeficit") return ConditionType::RangeOfMotionDeficit;
        if (s == "incorrectexecution") return ConditionType::IncorrectExecution;
        if (s == "exerciseovertime") return ConditionType::ExerciseOvertime;


    return ConditionType::exerciseInit;
}
/**
 * @brief Convierte un ConditionType a su representación en texto.
 * @param type Tipo de condición.
 * @return Nombre en texto.
 */
inline QString conditionTypeToString(ConditionType type) {
    switch (type) {

        case ConditionType::MaxAngle: return "MaxAngle";
        case ConditionType::MinAngle:  return "MinAngle";
        case ConditionType::Increase:    return "Increase";
        case ConditionType::Decrease:    return "Decrease";
        case ConditionType::OpositeDirection:    return "OpositeDirection";
        case ConditionType::Has_Stopped:    return "Has_Stopped";
        case ConditionType::is_Steady: return "is_Steady";
        case ConditionType::Not_Steady: return "Not_Steady";
        case ConditionType::MaxStateTimeout: return "MaxStateTimeout";
        case ConditionType::MinStateTimeout: return "MinStateTimeout";
        case ConditionType::OptimalForm: return "OptimalForm";
        case ConditionType::FastMovement: return "FastMovement";
        case ConditionType::SlowMovement: return "SlowMovement";
        case ConditionType::JointOverload: return "JointOverload";
        case ConditionType::symmetryDeviation: return "symmetryDeviation";
        case ConditionType::exerciseInit:  return "exerciseInit";
        case ConditionType::EndOfRepetition: return "EndOfRepetition";
        case ConditionType::EndOfSet: return "EndOfSet";
        case ConditionType::InitSet: return "InitSet";
        case ConditionType::InitRepetition: return "InitRepetition";
        case ConditionType::EndOfExercise: return "EndOfExercise";
        case ConditionType::SetTime:    return "SetTime";
        case ConditionType::RestTime: return "RestTime";
        case ConditionType::EndOfMovementPhase: return "EndOfMovementPhase";
        case ConditionType::RestOverTime: return "RestOverTime";
        case ConditionType::AccelerationSpikes: return "AccelerationSpikes";
        case ConditionType::RangeOfMotionDeficit: return "RangeOfMotionDeficit";
        case ConditionType::ExerciseOvertime: return "ExerciseOvertime";
        case ConditionType::IncorrectExecution: return "IncorrectExecution";
    }
    return "exerciseInit";
}


// inline bool operator==(const Condition &c1, const Condition &c2) {

//     return c1.type == c2.type
//            && c1.keypointLine == c2.keypointLine
//         && c1.view==c2.view;
// }
/**
 * @brief Comparador de igualdad para condiciones.
 * Ignora línea y vista en condiciones globales.
 */
inline bool operator==(const Condition &c1, const Condition &c2) {
    static const QSet<ConditionType> ignoreViewAndLine = {
        ConditionType::MaxStateTimeout,
        ConditionType::exerciseInit,
        ConditionType::EndOfRepetition,
        ConditionType::EndOfSet,
        ConditionType::EndOfExercise,
        ConditionType::EndOfMovementPhase,
        ConditionType::RestOverTime,
        ConditionType::SetTime
    };

    if (c1.type != c2.type)
        return false;

    if (ignoreViewAndLine.contains(c1.type))
        return true;

    return c1.keypointLine == c2.keypointLine && c1.view == c2.view;
}
/**
 * @brief Hash para Condition, teniendo en cuenta tipo, línea y vista según el tipo.
 * @param key Objeto Condition.
 * @param seed Semilla de hash.
 * @return Valor hash calculado.
 */
inline uint qHash(const Condition &key, uint seed = 0) {
    static const QSet<ConditionType> ignoreViewAndLine = {
        ConditionType::MaxStateTimeout,
        ConditionType::exerciseInit,
        ConditionType::EndOfRepetition,
        ConditionType::EndOfSet,
        ConditionType::EndOfExercise,
        ConditionType::EndOfMovementPhase,
        ConditionType::RestOverTime,
        ConditionType::SetTime
    };

    uint baseHash = qHash(static_cast<int>(key.type), seed);

    if (ignoreViewAndLine.contains(key.type))
        return baseHash;

    baseHash ^= qHash(key.keypointLine, seed);
    baseHash ^= qHash(static_cast<int>(key.view), seed);
    return baseHash;
}

// inline uint qHash(const Condition &key, uint seed = 0) {

//     return qHash(static_cast<int>(key.type), seed) ^ qHash(key.keypointLine, seed);
// }
/**
 * @enum ConditionCategory
 * @brief Clasificación de severidad para las condiciones.
 */
enum class ConditionCategory {
    critical,   ///< Condición crítica que puede causar lesión o error grave.
    alert,      ///< Alerta de desviación no crítica pero relevante.
    info,       ///< Mensaje informativo o de refuerzo positivo.
    none        ///< Sin categoría asignada.
};

#endif // CONDITION_H
