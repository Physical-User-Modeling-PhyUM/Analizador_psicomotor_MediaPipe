/**
 * @file trainingworkout.cpp
 * @brief Implementación de la clase TrainingWorkout.
 */

#include "trainingworkout.h"

/**
 * @brief Constructor por defecto. Inicializa los campos con valores básicos.
 */

TrainingWorkout::TrainingWorkout(QObject *parent) {
    idWorkout = -1;
    name = "Nuevo Workout";
    description = "";
    estimatedDuration = 0;
    author = "";
    exercises = {};
}



/**
 * @brief Constructor desde un mapa de campos `WorkoutField`.
 */

TrainingWorkout::TrainingWorkout(QHash<WorkoutField, QVariant>& data,QObject* parent) {
    int id = data.value(WorkoutField::IdWorkout, -1).toInt();
    idWorkout = (id <= 0) ? -1 : id;

    QString n = data.value(WorkoutField::Name).toString().trimmed();
    name = (n.isEmpty()) ? "Sin nombre" : n;

    description = data.value(WorkoutField::Description).toString().trimmed();

    int dur = data.value(WorkoutField::duration, -1).toInt();
    estimatedDuration = (dur <= 0) ? 0 : dur;

    exercises = {};
}

// --- Getters y setters simples ---

int TrainingWorkout::getIdWorkout() const {
    return idWorkout;
}

QString TrainingWorkout::getName() const {
    return name;
}

void TrainingWorkout::setName( QString newName) {
    name = newName;
}

QString TrainingWorkout::getDescription() const {
    return description;
}

void TrainingWorkout::setDescription( QString newDescription) {
    description = newDescription;
}

int TrainingWorkout::getEstimatedDuration() const {
    return estimatedDuration;
}

void TrainingWorkout::setEstimatedDuration(int newEstimatedDuration) {
    estimatedDuration = newEstimatedDuration;
}


QString TrainingWorkout::getAuthor() const
{
  return author;
}

void TrainingWorkout::setAuthor(const QString &newAuthor)
{
  author = newAuthor;
}
// --- Gestión de ejercicios ---

/**
 * @brief Devuelve la lista completa de ejercicios.
 */
QList<QSharedPointer<ExerciseEspec>>TrainingWorkout::getExercises() const {
    return exercises;
}
/**
 * @brief Recupera un ejercicio por su ID.
 */
QSharedPointer<ExerciseEspec> TrainingWorkout::getExercise(int idEx)
{
    for ( QSharedPointer<ExerciseEspec>& ex : exercises) {
        if (ex->getIdEx() == idEx) {
            return ex;
        }
    }
    return nullptr;
}
/**
 * @brief Reemplaza un ejercicio existente con el mismo ID.
 * @param updated Ejercicio actualizado.
 * @return true si se reemplazó correctamente.
 */
bool TrainingWorkout::replaceExercise(QSharedPointer<ExerciseEspec> updated)
{
    for (int i = 0; i < exercises.size(); ++i) {
        if (exercises[i]->getIdEx() == updated->getIdEx()) {
            exercises[i] = updated;
            return true;
        }
    }
    return false;
}
/**
 * @brief Añade un nuevo ejercicio al entrenamiento.
 * @param exercise Puntero al ejercicio.
 * @param order Posición donde insertarlo. Si es -1, se agrega al final.
 * @return true si se añadió correctamente.
 */
bool TrainingWorkout::addExercise(QSharedPointer<ExerciseEspec> exercise, int order) {

    if (exercise== nullptr) {
         qWarning() << "Ejercicio nulo:";
         emit ui_errorMessage("No se puede realizar esa operación");
        return false;
    }
    // Verificar si ya existe un ejercicio con el mismo ID
    for ( QSharedPointer<ExerciseEspec> ex : exercises) {

        if (ex->getIdEx() == exercise->getIdEx()) {
            qWarning() << "Ejercicio con idEx duplicado:" << exercise->getIdEx();
            emit ui_errorMessage("Ese ejercicio ya se encuentra incluido en el plan de entrenamiento");
            return false;
        }
    }
    // si el valor es el valor por defecto lo agragamos al final de la lista
    if (order == -1 || order < 0 || order > exercises.size()) {
        exercises.append(exercise);
    } else {

        exercises.insert(order, exercise);
    }

    return true;
}
/**
 * @brief Elimina un ejercicio del entrenamiento por su ID.
 * @param idEx ID del ejercicio a eliminar.
 * @return true si se eliminó; false si no se encontró.
 */
bool TrainingWorkout::removeExercise(int idEx)
{
    for (int i = 0; i < exercises.size(); ++i) {
        if (exercises[i]->getIdEx() == idEx) {
            exercises.removeAt(i);
            return true;
        }
    }
    emit ui_errorMessage("No se ha eliminado ningún ejercicio");
    return false;
}


/**
 * @brief Genera un resumen del entrenamiento incluyendo los resúmenes de todos sus ejercicios.
 */
WorkoutSummary TrainingWorkout::toSummary()
{
    WorkoutSummary w(idWorkout, name, description, author,estimatedDuration);
    for ( QSharedPointer<ExerciseEspec>& ex : exercises) {
        w.addExerciseSummary(ex->toSummary());
    }
    return w;
}

void TrainingWorkout::setIdWorkout(int newIdWorkout)
{
    idWorkout = newIdWorkout;
}


