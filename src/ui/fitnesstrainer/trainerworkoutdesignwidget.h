/**
 * @file trainerworkoutdesignwidget.h
 * @brief Declaración de la clase TrainerWorkoutDesignWidget, interfaz de diseño de planes de entrenamiento para entrenadores.
 *
 * Este widget permite al entrenador crear, editar y gestionar workouts (entrenamientos) personalizados.
 * Proporciona control total sobre la definición de ejercicios, estados, transiciones y restricciones,
 * permitiendo componer entrenamientos detallados, reutilizar ejercicios desde la biblioteca y asignarlos a clientes.
 */

#ifndef TRAINERWORKOUTDESIGNWIDGET_H
#define TRAINERWORKOUTDESIGNWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QLoggingCategory>

#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "ui/main/uitablemodel.h"
#include "ui_trainerworkoutdesignwidget.h"
#include "workouts/trainingworkout.h"
#include "workouts/exerciseespec.h"
#include "pose/condition.h"

Q_DECLARE_LOGGING_CATEGORY(trainerWorkoutDesignWidget)

/**
 * @class TrainerWorkoutDesignWidget
 * @brief Widget principal para el diseño de entrenamientos por parte del entrenador.
 *
 * Este widget organiza el diseño y edición de workouts personalizados, gestionando:
 * - Creación y eliminación de entrenamientos
 * - Composición de ejercicios y edición de sus propiedades
 * - Edición de estados del ejercicio
 * - Definición de transiciones y restricciones
 * - Asignación de entrenamientos a clientes
 *
 * También ofrece funciones para importar desde biblioteca, bloquear columnas editables
 * y controlar los cambios realizados para permitir guardar o descartar de forma agrupada.
 */
class TrainerWorkoutDesignWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param controller Controlador principal de la aplicación.
     * @param parent Widget padre opcional.
     */
    explicit TrainerWorkoutDesignWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~TrainerWorkoutDesignWidget();

    /**
     * @brief Asigna el entrenador actual.
     * @param trainer Entrenador autenticado.
     */
    void setTrainer(QSharedPointer<FitnessTrainer> trainer);

    /**
     * @brief Carga los entrenamientos disponibles en la tabla principal.
     */
    void loadWorkoutTable();

private slots:
    void cellWorkoutSelected(const QModelIndex &current, const QModelIndex &previous);
    void on_saveButton_clicked();
    void on_DischardButton_clicked();
    void cellExerciseSelected(const QModelIndex& current, const QModelIndex&);
    void cellStateSelected(const QModelIndex& current, const QModelIndex&);
    void onModelModified();

    void on_new_WorkoutButton_clicked();
    void on_new_ExerciseButton_clicked();
    void on_ne_StateButton_clicked();
    void on_new_TransitionButton_clicked();
    void on_new_ConstraintButton_clicked();
    void on_delete_StateButton_clicked();
    void on_delete_ConstraintButton_clicked();
    void on_delete_TransitionButton_clicked();
    void on_delete_ExerciseButton_clicked();
    void on_delete_WorkoutButton_clicked();
    void on_AddToWorkout_Button_clicked();

    void onHeaderClickedExercises(int logicalIndex);
    void onHeaderClickedWorkouts(int logicalIndex);
    void onHeaderClickedExercisesLibrary(int logicalIndex);
    void onHeaderClickedStates(int logicalIndex);
    void onHeaderClickedConstraints(int logicalIndex);
    void onHeaderClickedTransitions(int logicalIndex);

    void on_clearFilters_Button_clicked();
    void onUiErrorMessage(const QString& msg, QtMsgType type = QtWarningMsg);
    void on_AssignButton_clicked();

private:
    Ui_TrainerWorkoutDesignWidget *ui;

    QList<QSharedPointer<TrainingWorkout>> buffer; ///< Buffer temporal de workouts.
    QSharedPointer<AppController> controller;
    QSharedPointer<FitnessTrainer> currentTrainer;
    QSharedPointer<TrainingWorkout> currentWorkout;
    QSharedPointer<ExerciseEspec> currentExercise;
    QSharedPointer<State> currentState;

    QPointer<UiTableModel> workoutModel;
    QPointer<UiTableModel> exerciseModel;
    QPointer<UiTableModel> stateModel;
    QPointer<UiTableModel> constraintModel;
    QPointer<UiTableModel> transitionModel;
    QPointer<UiTableModel> exerciseLibraryModel;

    QSortFilterProxyModel* exerciseProxyModel = nullptr;
    QSortFilterProxyModel* workoutProxyModel = nullptr;
    QSortFilterProxyModel* exerciseLibraryProxyModel = nullptr;
    QSortFilterProxyModel* statesProxyModel = nullptr;
    QSortFilterProxyModel* constraintsProxyModel = nullptr;
    QSortFilterProxyModel* transitionsProxyModel = nullptr;

    bool workoutChange = false;
    bool exercisesChanged = false;
    bool statesChanged = false;
    bool transitionsChanged = false;
    bool constraintsChanged = false;

    /**
     * @brief Configura las tablas de trabajo del widget.
     */
    void setupWorkoutTables();

    /**
     * @brief Aplica todos los cambios al workout actual.
     */
    void applyAllChangesToWorkout();

    /**
     * @brief Descarta todos los cambios realizados sin guardar.
     */
    void discardAllChanges();

    /**
     * @brief Indica si existen cambios sin guardar.
     * @return true si hay cambios; false en caso contrario.
     */
    bool isModified();

    // Métodos auxiliares para asignar ComboBox en tablas
    void assignComboBoxTransitions();
    void assignComboBoxConstraints();
    void assignComboBoxTransitionLines();
    void assignComboBoxTransitionViews();
    void assignComboBoxConstraintLines();
    void assignComboBoxConstraintViews();

    // Métodos auxiliares para bloquear columnas no editables
    void lockWorkoutTableColumns();
    void lockExercisesTableColumns();
    void lockStatesTableColumns();
    void lockTransitionsTableColumns();
    void lockConstraintsTableColumns();

    /**
     * @brief Inicializa la sección de asignación de workouts a clientes.
     */
    void loadAssignStackWidget();

    /**
     * @brief Configura la tabla de clientes para la asignación.
     */
    void setupClientsTable();

    /**
     * @brief Carga los ejercicios disponibles en la biblioteca.
     */
    void loadExerciseLibrary();
};

#endif // TRAINERWORKOUTDESIGNWIDGET_H
