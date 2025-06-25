#ifndef TRAINERWORKOUTDESIGNWIDGET_H
#define TRAINERWORKOUTDESIGNWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QPointer>
#include <QSortFilterProxyModel>
#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "ui/main/uitablemodel.h"
#include "ui_trainerworkoutdesignwidget.h"
#include "workouts/trainingworkout.h"
#include "workouts/exerciseespec.h"
#include "pose/condition.h"
#include <QLoggingCategory>


Q_DECLARE_LOGGING_CATEGORY(trainerWorkoutDesignWidget);

class TrainerWorkoutDesignWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerWorkoutDesignWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~TrainerWorkoutDesignWidget();

    void setTrainer(QSharedPointer<FitnessTrainer> trainer);
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

    QList< QSharedPointer<TrainingWorkout>> buffer;
    QSharedPointer<AppController> controller;
    QSharedPointer<FitnessTrainer> currentTrainer;
    QSharedPointer<TrainingWorkout> currentWorkout;
    QSharedPointer<ExerciseEspec>     currentExercise;
    QSharedPointer<State>     currentState;

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





    void setupWorkoutTables();

    void applyAllChangesToWorkout();
    void discardAllChanges();
    bool isModified();

    void assignComboBoxTransitions();
    void assignComboBoxConstraints();
    void assignComboBoxTransitionLines();
     void assignComboBoxTransitionViews();
    void assignComboBoxConstraintLines();
    void assignComboBoxConstraintViews();
    void lockWorkoutTableColumns();
    void lockExercisesTableColumns();
    void lockStatesTableColumns();
    void lockTransitionsTableColumns();
    void lockConstraintsTableColumns();
    void loadAssignStackWidget();
    void setupClientsTable();

    void loadExerciseLibrary();
};

#endif // TRAINERWORKOUTDESIGNWIDGET_H
