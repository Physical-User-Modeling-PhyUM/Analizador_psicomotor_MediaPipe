/*!
 * \file trainingrepository.h
 * \brief Declaración de la clase TrainingRepository.
 *
 * Esta clase proporciona una interfaz de alto nivel para la gestión de datos relacionados con entrenamientos,
 * incluyendo ejercicios, workouts, sesiones y sus reportes. Centraliza el acceso a la base de datos
 * a través de un `DBManager`, permitiendo persistencia y recuperación eficiente.
 */

#ifndef TRAININGREPOSITORY_H
#define TRAININGREPOSITORY_H

#include "core/dbmanager.h"
#include <QPointer>
#include "workouts/exerciseespec.h"
#include "workouts/trainingworkout.h"
#include "workouts/trainingsesion.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(TrainingRepo)

/*!
 * \class TrainingRepository
 * \brief Repositorio responsable de la gestión de datos de entrenamiento en la base de datos.
 *
 * Encapsula las operaciones CRUD para objetos como `ExerciseEspec`, `TrainingWorkout` y `TrainingSesion`,
 * además de funciones auxiliares para listar elementos y convertir datos a estructuras de persistencia (`DBTable`).
 */
class TrainingRepository: public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor que inicializa el repositorio con el manejador de base de datos.
     * \param dbManager Puntero al objeto `DBManager` que gestiona la conexión y las operaciones SQL.
     * \param parent Objeto padre en el árbol de objetos de Qt.
     */
    explicit TrainingRepository(DBManager* dbManager, QObject* parent = nullptr);

    /*!
     * \brief Recupera una especificación de ejercicio desde la base de datos a partir de su ID.
     * \param id Identificador único del ejercicio.
     * \return Puntero compartido a `ExerciseEspec`, o `nullptr` si no se encuentra.
     */
    QSharedPointer<ExerciseEspec> getEspec(int id);

    /*!
     * \brief Recupera un workout completo (incluyendo ejercicios) desde la base de datos.
     * \param id Identificador único del workout.
     * \return Puntero compartido a `TrainingWorkout`, o `nullptr` si no se encuentra.
     */
    QSharedPointer<TrainingWorkout> getWorkout(int id);

    // Métodos para listar entidades desde la base de datos
    DBTable listWorkouts(DBFilter filtro, QString valor);
    DBTable listExercises(DBFilter filtro, QString valor);
    DBTable listStates(DBFilter filtro, QString valor);
    DBTable listConstraints(DBFilter filtro, QString valor);

    /*!
     * \brief Guarda o reemplaza una especificación de ejercicio completa en la base de datos.
     * \param espec Objeto `ExerciseEspec` a persistir.
     * \return ID asignado del ejercicio, o -1 si falla.
     */
    int saveEspec(QSharedPointer<ExerciseEspec> espec);

    /*!
     * \brief Elimina una especificación de ejercicio y todos sus estados, constraints y transiciones asociadas.
     * \param idEx ID del ejercicio a eliminar.
     * \return `true` si la eliminación fue exitosa.
     */
    bool deleteEspec(int idEx);

    /*!
     * \brief Guarda un workout completo, incluyendo ejercicios y relaciones.
     * \param workout Objeto `TrainingWorkout` a guardar.
     * \return ID del workout guardado, o -1 si ocurre un error.
     */
    int saveWorkout(QSharedPointer<TrainingWorkout> workout);

    /*!
     * \brief Elimina un workout y todas las especificaciones de ejercicios asociadas.
     * \param idWorkout ID del workout a eliminar.
     * \return `true` si se eliminó correctamente.
     */
    bool deleteWorkoutfull(int idWorkout);

    /*!
     * \brief Elimina solo la entrada principal del workout sin tocar los ejercicios.
     * \param idWorkout ID del workout a eliminar.
     * \return `true` si se eliminó correctamente.
     */
    bool deleteWorkout(int idWorkout);

    /*!
     * \brief Guarda una sesión de entrenamiento, incluyendo resumen de ejercicio y reporte.
     * \param sesion Puntero compartido a `TrainingSesion`.
     * \return `true` si se guardó con éxito.
     */
    bool SaveTrainingSesion(QSharedPointer<TrainingSesion> sesion);

    /*!
     * \brief Recupera una sesión de entrenamiento completa (con resumen y reporte).
     * \param idSesion Identificador de la sesión.
     * \return Puntero compartido a `TrainingSesion`, o `nullptr` si falla.
     */
    QSharedPointer<TrainingSesion> getTrainingSesion(int idSesion);

    /*!
     * \brief Genera el siguiente ID disponible para un workout.
     * \return ID único generado.
     */
    int getNextWorkoutId();

    /*!
     * \brief Genera el siguiente ID disponible para una especificación de ejercicio.
     * \return ID único generado.
     */
    int getNextExerciseId();

    /*!
     * \brief Lista las fechas asociadas a sesiones previas de un cliente sobre un ejercicio y workout específicos.
     * \param idClient ID del cliente.
     * \param idWorkout ID del workout.
     * \param idEx ID del ejercicio.
     * \return Mapa de ID de sesión → fecha en `QDateTime`.
     */
    QMap<int, QDateTime> listSesionSummaries(int idClient, int idWorkout, int idEx);

signals:
    /*!
     * \brief Señal utilizada para notificar errores en la interfaz.
     * \param msg Mensaje descriptivo del error.
     * \param type Nivel de severidad (`QtMsgType`).
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);

private:
    DBManager* dbManager;  //!< Puntero al gestor de base de datos.

    /*!
     * \brief Convierte un objeto `ExerciseEspec` a un `DBTable` para guardado.
     * \param espec Especificación de ejercicio a convertir.
     * \return Tabla con los datos listos para persistencia.
     */
    DBTable toDBTable_ExerciseEspec(QSharedPointer<ExerciseEspec> espec);

    /*!
     * \brief Convierte un objeto `TrainingWorkout` a un `DBTable` para guardado.
     * \param workout Objeto de entrenamiento a convertir.
     * \return Tabla con los datos del workout.
     */
    DBTable toDBTable_TrainingWorkout(QSharedPointer<TrainingWorkout> workout);
};

#endif // TRAININGREPOSITORY_H
