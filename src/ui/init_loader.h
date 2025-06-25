/**
 * @file init_loader.h
 * @brief Declaración de la clase Init_Loader, responsable de la carga inicial de datos en la base de datos del sistema.
 *
 * Esta clase permite poblar la base de datos con datos estructurados de ejemplo para propósitos de desarrollo,
 * pruebas o despliegues iniciales. Inserta usuarios (clientes, administrador, entrenador), perfiles, ejercicios,
 * entrenamientos, sesiones de entrenamiento y relaciones entre ellos, garantizando consistencia de IDs.
 */

#ifndef INIT_LOADER_H
#define INIT_LOADER_H

// Perfiles de usuario
#include "profiles/client.h"
#include "profiles/admin.h"
#include "profiles/fitnesstrainer.h"
#include "profiles/clientprofile.h"

// Entrenamiento
#include "workouts/exerciseespec.h"
#include "workouts/trainingsesion.h"
#include "workouts/trainingworkout.h"
#include "workouts/exercisesummary.h"
#include "pose/sesionreport.h"

// Estados y restricciones
#include "pose/state.h"
#include "pose/condition.h"
#include "pose/angleconstraint.h"

// Enums de campos y tipos de base de datos
#include "enums/ExEspecFieldEnum.h"
#include "enums/UserFieldEnum.h"
#include "enums/WorkoutFieldEnum.h"
#include "enums/TypeBDEnum.h"
#include "enums/profileEnums.h"

// Qt
#include <QSharedPointer>
#include <QHash>
#include <QVariant>
#include <QDateTime>
#include <QPair>
#include <QSet>
#include <QSharedDataPointer>

#include "core/dbmanager.h"

/**
 * @class Init_Loader
 * @brief Clase encargada de inicializar la base de datos del sistema con datos de prueba o despliegue.
 *
 * Init_Loader permite:
 * - Crear entradas iniciales de clientes con perfiles completos.
 * - Cargar entrenadores y un administrador del sistema.
 * - Definir ejercicios y entrenamientos preconfigurados con sus especificaciones, estados y restricciones.
 * - Asignar sesiones históricas con reportes para análisis.
 *
 * Utiliza `DBManager` para persistencia y `DBTable` para estructuración de los datos por entidad.
 */
class Init_Loader {

public:
    /**
     * @brief Constructor por defecto.
     */
    Init_Loader();

    /**
     * @brief Ejecuta el proceso de carga completa en la base de datos.
     * @param db Instancia del gestor de base de datos a utilizar.
     */
    void load(QSharedPointer<DBManager> db);

    /**
     * @brief Destructor.
     */
    ~Init_Loader();

private:
    /**
     * @brief Carga los ejercicios con sus especificaciones y restricciones asociadas.
     * @param db Instancia de la base de datos.
     */
    void loadExercises(QSharedPointer<DBManager> db);

    /**
     * @brief Carga entrenamientos predefinidos compuestos por los ejercicios cargados.
     * @param db Instancia de la base de datos.
     */
    void loadWorkouts(QSharedPointer<DBManager> db);

    /**
     * @brief Carga los clientes con sus perfiles físicos y preferencias.
     * @param db Instancia de la base de datos.
     */
    void loadClients(QSharedPointer<DBManager> db);

    /**
     * @brief Inserta en la base de datos un administrador y un entrenador físico.
     * @param db Instancia de la base de datos.
     */
    void loadTrainerAndAdmin(QSharedPointer<DBManager> db);

    /**
     * @brief Carga sesiones históricas de entrenamiento asociadas a los clientes cargados.
     * @param db Instancia de la base de datos.
     */
    void loadTrainingSessions(QSharedPointer<DBManager> db);

    /**
     * @brief Crea estructuras vacías por tipo de entidad en la base de datos para poblarlas posteriormente.
     * @param tables Hash que asocia cada tipo con su tabla correspondiente (`DBTable`).
     */
    void createEmptyTables(QHash<TypeBDEnum, DBTable>& tables);
};

#endif // INIT_LOADER_H
