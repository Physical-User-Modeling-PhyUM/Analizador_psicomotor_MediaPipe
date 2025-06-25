/**
 * @file exerciseespec.cpp
 * @brief Implementación de la clase ExerciseEspec.
 */

#include "exerciseespec.h"


// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(ExerciseSpec, "ExerciseSpec")
/**
 * @brief Constructor por defecto. Inicializa los atributos con valores predeterminados.
 */
ExerciseEspec::ExerciseEspec(QObject *parent){
    idEx =  -1;

    name = "not defined";
    description ="";
    exersiseType = "not defined";
    targetMuscle = "not defined";
    Equipment = "not defined";
    series = 0;
    repetitions =0;
    duration = 0;
    weightPercentage = 0;
    restTime = 0;

}
/**
 * @brief Constructor que inicializa todos los atributos desde un QHash de campos.
 *
 * @param data Mapa con claves de tipo ExEspecField y sus valores asociados.
 * @param parent Objeto padre de Qt.
 */
ExerciseEspec::ExerciseEspec(QHash<ExEspecField, QVariant> data, QObject* parent) {

    idEx = data.value(ExEspecField::IdEx, -1).toInt();

    name = data.value(ExEspecField::Name).toString();
    description = data.value(ExEspecField::Description).toString();
    exersiseType = data.value(ExEspecField::ExersiseType).toString();
    targetMuscle = data.value(ExEspecField::TargetMuscle).toString();
    Equipment = data.value(ExEspecField::Equipment).toString();

    // Garantizar que los valores numéricos sean no negativos
    series = qMax(0, data.value(ExEspecField::Series, 0).toInt());
    repetitions = qMax(0, data.value(ExEspecField::Repetitions, 0).toInt());
    duration = qMax(0, data.value(ExEspecField::Duration, 0).toInt());
    weightPercentage = qMax(0.0, data.value(ExEspecField::Weight, 0.0).toFloat());
    restTime = qMax(0, data.value(ExEspecField::RestTime, 0).toInt());
}


/**
 * @brief Devuelve la lista de estados del ejercicio, ordenada por ID.
 */
QList<State> ExerciseEspec::getStatesList() const {
    QList<State> sortedList = states.values();  // Convertir QSet a QList
    std::sort(sortedList.begin(), sortedList.end(), [](const State& a, const State& b) {
        return a.getId() < b.getId();  // Ordenar por ID
    });
    return sortedList;
}
/**
 * @brief Recupera un estado específico por ID.
 */
State ExerciseEspec::getState (int idState) const{
    for (const State& s :states ){
        if (s.getId()==idState) return s;
    };
    return State(-1);
}
/**
 * @brief Actualiza un estado existente en el conjunto, si coincide por ID.
 * @return true si se realizó la actualización.
 */
bool ExerciseEspec::updateState(State state){
    for (const State& s :states ){
        if (s.getId()==state.getId()){
            states.remove(s);
            addState(state);
            return true;
        }
    }
    return false;
}
/**
 * @brief Devuelve la tabla de transiciones actuales entre estados.
 */
QHash<QPair<int, int>, QSet<Condition>> ExerciseEspec::getTransitionTable() const {
    return transitionTable;
}
/**
 * @brief Añade un nuevo estado al conjunto. Si no tiene ID asignado, se autogenera.
 */
void ExerciseEspec::addState( State state) {
    state.setIdEx(idEx);
    if (state.getId() == -1) {
        state.setId(states.size());
    }
    states.insert(state);
}

/**
 * @brief Elimina un estado del conjunto por su ID.
 */
void ExerciseEspec::delState(int id){

    for (auto it = states.begin(); it != states.end(); ++it) {
        if (it->getId() == id) {
            states.erase(it);
            break;
        }
    }
}

/**
 * @brief Añade un conjunto de condiciones para una transición entre dos estados.
 */
void ExerciseEspec::addTransition(QPair<int, int> estados, QSet<Condition> condiciones) {
    if (transitionTable.contains(estados)) {
        transitionTable[estados].unite(condiciones);
    } else {
        transitionTable.insert(estados, condiciones);
    }
}
/**
 * @brief Añade una única condición a una transición específica.
 */
void ExerciseEspec::addTransition(QPair<int, int> estados, const Condition& condicion) {
    addTransition(estados, QSet<Condition> { condicion });
}
/**
 * @brief Elimina una condición específica de una transición entre estados.
 */
void ExerciseEspec::delTransition(QPair<int, int> estados, const Condition& condicion) {
    // Verificar si el par de estados existe
    if (!transitionTable.contains(estados))
        emit ui_errorMessage("No existe una transición entre esos estados",QtMsgType::QtInfoMsg);
        return;

    QSet<Condition>& conditions = transitionTable[estados];

    for (auto it = conditions.begin(); it != conditions.end(); ++it) {
        QString line= it->keypointLine;
        if (it->type == condicion.type && line == condicion.keypointLine) {

            conditions.erase(it);
            qDebug(ExerciseSpec) << "Eliminada transición:"
                                 << conditionTypeToString(condicion.type)
                                 << " line:" << line
                                 << " total transiciones ahora:" << conditions.size();
            break;
        }
    }

    // Si ya no quedan condiciones para ese par de estados, eliminamos la entrada
    if (conditions.isEmpty()) {
        transitionTable.remove(estados);
    }
}
/**
 * @brief Convierte los datos del ejercicio en un resumen tipo `ExerciseSummary`.
 */
ExerciseSummary ExerciseEspec::toSummary() const {
    return ExerciseSummary(
        getIdEx(),
        getName(),
        getDescription(),
        getExersiseType(),
        getTargetMuscle(),
        getSeries(),
        getRepetitions(),
        getDuration(),
        getWeightPercentage(),
        getRestTime()

        );
}

/**

@brief Obtiene el identificador único del ejercicio.
@return ID del ejercicio (int).
*/
int ExerciseEspec::getIdEx() const
{
    return idEx;
}
/**

@brief Establece el identificador único del ejercicio.
@param newIdEx Nuevo ID a asignar.
*/
void ExerciseEspec::setIdEx(int newIdEx)
{
    idEx = newIdEx;
}
/**

@brief Obtiene el nombre del ejercicio.
@return Nombre del ejercicio (QString).
*/
QString ExerciseEspec::getName() const
{
    return name;
}
/**

@brief Establece el nombre del ejercicio.
@param newName Nuevo nombre a asignar.
*/
void ExerciseEspec::setName(const QString &newName)
{
    name = newName;
}
/**

@brief Devuelve la descripción del ejercicio.
@return Texto descriptivo del ejercicio.
*/
QString ExerciseEspec::getDescription() const
{
    return description;
}
/**

@brief Establece la descripción del ejercicio.
@param newDescription Nueva descripción.
*/
void ExerciseEspec::setDescription(const QString &newDescription)
{
    description = newDescription;
}
/**

@brief Devuelve el tipo de ejercicio (por ejemplo, "fuerza", "movilidad", etc.).
@return Tipo de ejercicio (QString).
*/
QString ExerciseEspec::getExersiseType() const
{
    return exersiseType;
}
/**

@brief Establece el tipo de ejercicio.
@param newExersiseType Nuevo tipo.
*/
void ExerciseEspec::setExersiseType(const QString &newExersiseType)
{
    exersiseType = newExersiseType;
}
/**

@brief Devuelve el grupo muscular objetivo del ejercicio.
@return Grupo muscular (QString).
*/
QString ExerciseEspec::getTargetMuscle() const
{
    return targetMuscle;
}
/**

@brief Establece el grupo muscular objetivo del ejercicio.
@param newTargetMuscle Nombre del grupo muscular.
*/
void ExerciseEspec::setTargetMuscle(const QString &newTargetMuscle)
{
    targetMuscle = newTargetMuscle;
}
/**

@brief Obtiene el tipo de equipamiento utilizado.
@return Nombre del equipamiento (QString).
*/
QString ExerciseEspec::getEquipment() const
{
    return Equipment;
}
/**

@brief Establece el tipo de equipamiento.
@param newEquipment Nombre del equipamiento.
*/
void ExerciseEspec::setEquipment(const QString &newEquipment)
{
    Equipment = newEquipment;
}
/**

@brief Devuelve la cantidad de series del ejercicio.
@return Número de series (int).
*/
int ExerciseEspec::getSeries() const
{
    return series;
}
/**

@brief Establece la cantidad de series del ejercicio.
@param newSeries Nueva cantidad de series.
*/
void ExerciseEspec::setSeries(int newSeries)
{
    series = newSeries;
}
/**

@brief Devuelve el número de repeticiones por serie.
@return Número de repeticiones (int).
*/
int ExerciseEspec::getRepetitions() const
{
    return repetitions;
}
/**

@brief Establece el número de repeticiones por serie.
@param newRepetitions Valor a asignar.
*/
void ExerciseEspec::setRepetitions(int newRepetitions)
{
    repetitions = newRepetitions;
}
/**

@brief Devuelve la duración estimada del ejercicio en segundos.
@return Duración (int).
*/
int ExerciseEspec::getDuration() const
{
    return duration;
}
/**

@brief Establece la duración estimada del ejercicio.
@param newDuration Duración en segundos.
*/
void ExerciseEspec::setDuration(int newDuration)
{
    duration = newDuration;
}
/**

@brief Devuelve el porcentaje de peso respecto al máximo del usuario.
@return Porcentaje (float).
*/
float ExerciseEspec::getWeightPercentage() const
{
    return weightPercentage;
}
/**

@brief Establece el porcentaje de peso del ejercicio.
@param newWeight Porcentaje (0.0–100.0).
*/
void ExerciseEspec::setWeightPercentage(float newWeight)
{
    weightPercentage = newWeight;
}
/**

@brief Devuelve el tiempo de descanso entre series.
@return Tiempo de descanso en segundos (int).
*/
int ExerciseEspec::getRestTime() const
{
    return restTime;
}
/**

@brief Establece el tiempo de descanso entre series.
@param newRestTime Tiempo en segundos.
*/
void ExerciseEspec::setRestTime(int newRestTime)
{
    restTime = newRestTime;
}
/**

@brief Establece el conjunto de transiciones del ejercicio.
@param newTransitionTable Nueva tabla de transiciones.
*/
void ExerciseEspec::setTransitionTable(const QHash<QPair<int, int>, QSet<Condition> > &newTransitionTable)
{
    transitionTable = newTransitionTable;
}
/**

@brief Devuelve el conjunto de estados del ejercicio.
@return Conjunto de objetos State.
*/
QSet<State> ExerciseEspec::getStates() const
{
    return states;
}
/**

@brief Establece un nuevo conjunto de estados.
@param newStates Conjunto actualizado de estados.
*/
void ExerciseEspec::setStates(const QSet<State> &newStates)
{
    states = newStates;
}
