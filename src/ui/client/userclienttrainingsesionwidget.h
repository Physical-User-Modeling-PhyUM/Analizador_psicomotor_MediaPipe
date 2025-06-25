/**
 * @file userclienttrainingsesionwidget.h
 * @brief Declaración de la clase UserClientTrainingSesionWidget, interfaz cliente para la consulta y ejecución de entrenamientos asignados.
 *
 * Esta clase define un widget que permite al cliente visualizar sus entrenamientos planificados,
 * explorar los ejercicios de cada entrenamiento y comenzar la ejecución de sesiones individuales.
 * Carga la información desde el calendario del cliente y utiliza modelos tabulares personalizados para mostrarla.
 */

#ifndef USERCLIENTTRAININGSESIONWIDGET_H
#define USERCLIENTTRAININGSESIONWIDGET_H

#include <QWidget>
#include <QLoggingCategory>
#include "ui_userclienttrainingsesionwidget.h"
#include "core/appcontroller.h"
#include "ui/main/uitablemodel.h"
#include "workouts/workoutsummary.h"
#include "workouts/exercisesummary.h"
#include "profiles/client.h"

Q_DECLARE_LOGGING_CATEGORY(TrainigSesionBoard)

/**
 * @class UserClientTrainingSesionWidget
 * @brief Widget cliente para consultar y ejecutar sesiones de entrenamiento asignadas.
 *
 * Esta interfaz muestra al usuario los entrenamientos asignados desde su calendario,
 * permite seleccionar uno y visualizar su lista de ejercicios, y ejecutar una sesión
 * para un ejercicio concreto.
 *
 * Internamente utiliza dos modelos: uno para los entrenamientos asignados y otro para los ejercicios.
 */
class UserClientTrainingSesionWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param controller Controlador principal de la aplicación.
     * @param parent Widget padre opcional.
     */
    explicit UserClientTrainingSesionWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~UserClientTrainingSesionWidget();

    /**
     * @brief Establece el cliente autenticado y carga sus entrenamientos.
     * @param user Instancia de `Client` autenticado.
     */
    void setUser(QSharedPointer<Client> user);

    /**
     * @brief Muestra la información general del workout seleccionado.
     * @param summary Estructura resumen del entrenamiento.
     */
    void updateWorkoutInfo(const WorkoutSummary& summary);

    /**
     * @brief Rellena la tabla de ejercicios con los datos de un entrenamiento.
     * @param exercises Lista de ejercicios pertenecientes al workout.
     */
    void populateExerciseTable(const QList<ExerciseSummary>& exercises);

private slots:
    /**
     * @brief Acción al seleccionar un entrenamiento de la tabla de asignados.
     * Carga la lista de ejercicios correspondiente.
     * @param index Índice seleccionado.
     */
    void onAssignedWorkoutSelected(const QModelIndex& index);

    /**
     * @brief Inicia una sesión de entrenamiento para el ejercicio actualmente seleccionado.
     */
    void onStartSesionButtonClicked();

private:
    /**
     * @brief Carga los entrenamientos asignados al cliente desde su calendario.
     */
    void loadAssignedWorkouts();

    /**
     * @brief Carga el entrenamiento actual según la fecha del sistema.
     */
    void loadCurrentWorkoutData();

    Ui_UserClientTrainingSesionWidget *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<Client> currentUser; ///< Cliente actualmente autenticado.
    QSharedPointer<AppController> controller; ///< Controlador central de la aplicación.

    UiTableModel* assignedModel; ///< Modelo de datos para los entrenamientos asignados.
    UiTableModel* exerciseModel; ///< Modelo de datos para los ejercicios del entrenamiento.

    int selectedExerciseId = -1; ///< ID del ejercicio actualmente seleccionado.
};

#endif // USERCLIENTTRAININGSESIONWIDGET_H
