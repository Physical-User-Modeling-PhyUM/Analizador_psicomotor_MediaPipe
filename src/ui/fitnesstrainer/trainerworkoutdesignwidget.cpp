#include "trainerworkoutdesignwidget.h"
#include "ui_trainerworkoutdesignwidget.h"
#include "ui/fitnesstrainer/assignworkoutdialog.h"
#include "pose/condition.h"
#include "ui/main/comboboxdelegate.h"
#include <QMessageBox>
#include <QDebug>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(trainerWorkoutDesignWidget, "trainerWorkoutDesignWidget")

TrainerWorkoutDesignWidget::TrainerWorkoutDesignWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::TrainerWorkoutDesignWidget),
    controller(controller)
{
    ui->setupUi(this);

    connect(ui->tableWorkouts->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &TrainerWorkoutDesignWidget::cellWorkoutSelected);

    connect(ui->tableExercises->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &TrainerWorkoutDesignWidget::cellExerciseSelected);

    connect(ui->tableStates->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &TrainerWorkoutDesignWidget::cellStateSelected);

    connect(controller->getTrainingManager()->getRepository().data(), &TrainingRepository::ui_errorMessage, this, &TrainerWorkoutDesignWidget::onUiErrorMessage);

}

TrainerWorkoutDesignWidget::~TrainerWorkoutDesignWidget()
{
    delete ui;
}

void TrainerWorkoutDesignWidget::setTrainer(QSharedPointer<FitnessTrainer> trainer)
{
    this->currentTrainer = trainer;
}



void TrainerWorkoutDesignWidget::onUiErrorMessage(const QString& msg, QtMsgType type)
{
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        QMessageBox::information(this, "Información", msg);
        break;
    case QtWarningMsg:
        QMessageBox::warning(this, "Advertencia", msg);
        break;
    case QtCriticalMsg:
    case QtFatalMsg:
        QMessageBox::critical(this, "Error Crítico", msg);
        break;
    }
}
void TrainerWorkoutDesignWidget::setupWorkoutTables()
{
    workoutModel = new UiTableModel(this);

    UiUtils::fromWorkoutSummaries(controller->getTrainingManager()->listWorkouts(DBFilter::none, {}), workoutModel);
    lockWorkoutTableColumns();

    ui->tableWorkouts->setModel(workoutModel);
    ui->tableWorkouts->resizeColumnsToContents();
    connect(workoutModel, &UiTableModel::modelModified,this, &TrainerWorkoutDesignWidget::onModelModified);

}


void TrainerWorkoutDesignWidget::loadWorkoutTable()
{
    setupWorkoutTables();

    // connect(exerciseModel, &UiTableModel::modelModified, this, &TrainerBoardWidget::onModelModified);

    exerciseModel = new UiTableModel(this);
    stateModel = new UiTableModel(this);
    constraintModel = new UiTableModel(this);
    transitionModel = new UiTableModel(this);
    //exerciseLibraryModel = new UiTableModel(this);
    loadExerciseLibrary();


    // Habilitamos ahora los filtrados en todas las tablas

    // tableExercises
    exerciseProxyModel = new QSortFilterProxyModel(this);
    exerciseProxyModel->setSourceModel(exerciseModel);
    ui->tableExercises->setModel(exerciseProxyModel);
    connect(ui->tableExercises->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutDesignWidget::onHeaderClickedExercises);

    // tableWorkouts
    workoutProxyModel = new QSortFilterProxyModel(this);
    workoutProxyModel->setSourceModel(workoutModel);
    ui->tableWorkouts->setModel(workoutProxyModel);
    connect(ui->tableWorkouts->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutDesignWidget::onHeaderClickedWorkouts);

    // tableExercisesLibrary
    exerciseLibraryProxyModel = new QSortFilterProxyModel(this);
    exerciseLibraryProxyModel->setSourceModel(exerciseLibraryModel);
    ui->tableExercisesLibrary->setModel(exerciseLibraryProxyModel);
    connect(ui->tableExercisesLibrary->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutDesignWidget::onHeaderClickedExercisesLibrary);

    // tableStates
    statesProxyModel = new QSortFilterProxyModel(this);
    statesProxyModel->setSourceModel(stateModel);
    ui->tableStates->setModel(statesProxyModel);
    connect(ui->tableStates->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutDesignWidget::onHeaderClickedStates);

    // tableConstraints
    constraintsProxyModel = new QSortFilterProxyModel(this);
    constraintsProxyModel->setSourceModel(constraintModel);
    ui->tableConstraints->setModel(constraintsProxyModel);
    connect(ui->tableConstraints->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutDesignWidget::onHeaderClickedConstraints);

    // tableTransitions
    transitionsProxyModel = new QSortFilterProxyModel(this);
    transitionsProxyModel->setSourceModel(transitionModel);
    ui->tableTransitions->setModel(transitionsProxyModel);
    connect(ui->tableTransitions->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainerWorkoutDesignWidget::onHeaderClickedTransitions);

    disconnect(ui->tableWorkouts->selectionModel(), nullptr, this, nullptr);
    connect(ui->tableWorkouts->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &TrainerWorkoutDesignWidget::cellWorkoutSelected);

    disconnect(ui->tableExercises->selectionModel(), nullptr, this, nullptr);
    connect(ui->tableExercises->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &TrainerWorkoutDesignWidget::cellExerciseSelected);

    disconnect(ui->tableStates->selectionModel(), nullptr, this, nullptr);
    connect(ui->tableStates->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &TrainerWorkoutDesignWidget::cellStateSelected);

}



void TrainerWorkoutDesignWidget::cellWorkoutSelected(const QModelIndex &current, const QModelIndex &)
{
    if (!current.isValid()) return;

    isModified();

    QModelIndex sourceIndex = workoutProxyModel->mapToSource(current);
    if (!sourceIndex.isValid()) return;

    int idWorkout = workoutModel->data(sourceIndex.siblingAtColumn(0)).toInt();

    currentWorkout.clear();

    if (!buffer.isEmpty()){
        for (const auto &w:buffer){

            if (w->getIdWorkout()==idWorkout)currentWorkout=w;
        }
        if (!(currentWorkout==nullptr))qDebug(trainerWorkoutDesignWidget)<<"Workout en el buffer con id:"<<idWorkout;
    }

    if (currentWorkout==nullptr){
        currentWorkout = controller->getTrainingManager()->getWorkoutData(idWorkout);
        qDebug(trainerWorkoutDesignWidget)<<"Workout importado de BD con id:"<<idWorkout;
    }
    if (currentWorkout==nullptr){
          qWarning(trainerWorkoutDesignWidget)<<"Error importado de BD workout con id:"<<idWorkout;
    }

    buffer.append(currentWorkout);
    //limpiamos las tablas si no es la primera vez que se cargan
    if (stateModel) stateModel->setDataSet({}, {});
    if (transitionModel) transitionModel->setDataSet({}, {});
    if (constraintModel) constraintModel->setDataSet({}, {});

    currentExercise.clear();
    currentState.clear();

    if (currentWorkout->getExercises().isEmpty() && exerciseModel){
        exerciseModel->setDataSet({}, {});
        return;
    }

    exerciseModel = new UiTableModel(this);

    UiUtils::fromWorkoutExercises(currentWorkout, exerciseModel);
    exerciseProxyModel->setSourceModel(exerciseModel);
    ui->tableExercises->setModel(exerciseProxyModel);
    ui->tableExercises->resizeColumnsToContents();
    lockExercisesTableColumns();

    connect(exerciseModel, &UiTableModel::modelModified, this, &TrainerWorkoutDesignWidget::onModelModified);
    disconnect(ui->tableExercises->selectionModel(), nullptr, this, nullptr);
    connect(ui->tableExercises->selectionModel(), &QItemSelectionModel::currentRowChanged,this, &TrainerWorkoutDesignWidget::cellExerciseSelected);
    connect(currentWorkout.data(), &TrainingWorkout::ui_errorMessage,this, &TrainerWorkoutDesignWidget::onUiErrorMessage);

    workoutChange = false;
}

void TrainerWorkoutDesignWidget::cellExerciseSelected(const QModelIndex& current, const QModelIndex&)
{
    if (currentWorkout==nullptr || !current.isValid()) return;

    isModified();

    QModelIndex sourceIndex = exerciseProxyModel->mapToSource(current);
    if (!sourceIndex.isValid()) return;

    int idEx = exerciseModel->data(sourceIndex.siblingAtColumn(0)).toInt();    QSharedPointer<ExerciseEspec> exercise = currentWorkout->getExercise(idEx);

    if (exercise==nullptr) return;
    currentExercise=exercise;

    if (constraintModel) constraintModel->setDataSet({}, {});
    currentState.clear();

    if (exercise->getStatesList().isEmpty()) {
        if (stateModel) stateModel->setDataSet({}, {});
        if (transitionModel) transitionModel->setDataSet({}, {});
        return;
    }

    // si no esta vacio agregamos los estados y transiciones al modelo
    stateModel = new UiTableModel(this);

    //connect(stateModel, &UiTableModel::modelModified, this, &TrainerBoardWidget::onModelModified);

    UiUtils::fromExerciseStates(exercise, stateModel);
    statesProxyModel->setSourceModel(stateModel);
    ui->tableStates->setModel(statesProxyModel);
    ui->tableStates->resizeColumnsToContents();
    lockStatesTableColumns();

    //Conectamos con la tabla para gestionar las modificaciones de las celdas
    connect(stateModel, &UiTableModel::modelModified,this, &TrainerWorkoutDesignWidget::onModelModified);
    //Conectamos para que la selección de estado pinte las constraints
    disconnect(ui->tableStates->selectionModel(), nullptr, this, nullptr);
    connect(ui->tableStates->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &TrainerWorkoutDesignWidget::cellStateSelected);

     if (exercise->getTransitionTable().isEmpty()) {
          if (transitionModel) transitionModel->setDataSet({}, {});
         return;
     }

    transitionModel = new UiTableModel(this);
    UiUtils::fromStateTransitions(exercise->getTransitionTable(), transitionModel);
    //ui->tableTransitions->setModel(transitionModel);
    transitionsProxyModel->setSourceModel(transitionModel);
    ui->tableTransitions->setModel(transitionsProxyModel);

    ui->tableTransitions->resizeColumnsToContents();
    assignComboBoxTransitions();
    assignComboBoxTransitionLines();
    assignComboBoxTransitionViews();
    lockTransitionsTableColumns();

    //Conectamos para las modificaciones de celda
    connect(transitionModel, &UiTableModel::modelModified,this, &TrainerWorkoutDesignWidget::onModelModified);
    connect(currentExercise.data(), &ExerciseEspec::ui_errorMessage, this, &TrainerWorkoutDesignWidget::onUiErrorMessage);

}

void TrainerWorkoutDesignWidget::cellStateSelected(const QModelIndex& current, const QModelIndex&)
{
    if (currentExercise==nullptr || !current.isValid()) return;

    isModified();

    QModelIndex sourceIndex = statesProxyModel->mapToSource(current);
    if (!sourceIndex.isValid()) return;
    int idState = stateModel->data(sourceIndex.siblingAtColumn(0)).toInt();

    //int idState = ui->tableStates->model()->data(current.siblingAtColumn(0)).toInt();

    QList<State> states = currentExercise->getStatesList();

    if (states.isEmpty()) {
        if (constraintModel) constraintModel->setDataSet({}, {});
        currentState.clear();
        return;
    }
    currentState= QSharedPointer<State>::create(currentExercise->getState(idState));
    if (currentState==nullptr)return;

    constraintModel = new UiTableModel(this);
    UiUtils::fromStateConstraints(currentState->getConstraints(), constraintModel);
    lockConstraintsTableColumns();

    constraintsProxyModel->setSourceModel(constraintModel);
    ui->tableConstraints->setModel(constraintsProxyModel);
    ui->tableConstraints->resizeColumnsToContents();
    assignComboBoxConstraints();
    assignComboBoxConstraintLines();
    assignComboBoxConstraintViews();


    //conectamos para los ediciones de la tabla
    connect(constraintModel, &UiTableModel::modelModified, this, &TrainerWorkoutDesignWidget::onModelModified);

}



void TrainerWorkoutDesignWidget::on_saveButton_clicked()
{
    // Debe guardar currentworkout en la base de datos
    if (currentWorkout==nullptr) return;

    applyAllChangesToWorkout();
    int id=controller->getTrainingManager()->saveWorkout(currentWorkout);
    currentWorkout=controller->getTrainingManager()->getWorkoutData(id);


}

void TrainerWorkoutDesignWidget::on_DischardButton_clicked()
{
    if (QMessageBox::question(this, "Descartar cambios", "¿Seguro que deseas descartar los cambios?") == QMessageBox::Yes) {
        discardAllChanges();
    }
}

void TrainerWorkoutDesignWidget::applyAllChangesToWorkout()
{
    // Este método aplicará los cambios efectuados en las tablas a las clases del modelo
    //para que sean permanentes, mientras que no se pulse Save los cambios no se actualizarán
    //la base de datos, se quedarán en el buffer

    if (currentWorkout==nullptr) return;

    //Si se han modificado las constraints actualizamos el la clase del modelo
    qDebug(trainerWorkoutDesignWidget)<<"constraintsChanged"<<constraintsChanged<<"constraintModel"<<constraintModel<<"currentState"<<currentState;
    if (constraintsChanged && constraintModel && currentState){

         //qDebug(TrainerBoard)<<"Modificando constraints total antes"<<currentState->getConstraints().size();
        currentState->setConstraints(UiUtils::toStateConstraints(constraintModel));

         //qDebug(TrainerBoard)<<"Modificando constraints total despues"<<currentState->getConstraints().size();
        //qDebug(TrainerBoard)<<"Estados antes"<<currentExercise->getStates().size();
        currentExercise->updateState(*currentState);
         //qDebug(TrainerBoard)<<"Estados despues"<<currentExercise->getStates().size();
        currentWorkout->replaceExercise(currentExercise);
        //currentState = QSharedPointer<State>::create(currentExercise->getState(currentState->getId()));

    }
    //Si se han modificado las transiciones actualizamos currentExercise
    if (transitionsChanged && transitionModel && currentExercise){

        currentExercise->setTransitionTable(UiUtils::toStateTransitions(transitionModel));
        currentWorkout->replaceExercise(currentExercise);
    }
      //Si se han modificado los States actualizamos currentExercise
    if (statesChanged && stateModel && currentExercise){
        QSet<State> s=currentExercise->getStates();
       // UiUtils::toExerciseStates(s ,stateModel);
        currentExercise->setStates(UiUtils::toExerciseStates(s ,stateModel));
        currentWorkout->replaceExercise(currentExercise);
    }

    if (exercisesChanged && exerciseModel && !(currentExercise==nullptr)){
        UiUtils::updateExerciseEspecFromTable(exerciseModel, currentExercise);
        currentWorkout->replaceExercise(currentExercise);
    }

    if (workoutModel)
        UiUtils::updateWorkoutFromTable(workoutModel, currentWorkout);

    workoutChange = false;
    exercisesChanged = false;
    statesChanged = false;
    transitionsChanged = false;
    constraintsChanged = false;
}

void TrainerWorkoutDesignWidget::discardAllChanges()
{
    //Este método eliminara todos aquelos cambios que no se hayan aceptado
    //Se cargarán los ultimos aceptados que se encuentran en el buffer

    if (currentWorkout==nullptr) return;

    if (exercisesChanged && exerciseModel)
        UiUtils::fromWorkoutExercises(currentWorkout, exerciseModel);

    if (statesChanged && stateModel && currentExercise)
        UiUtils::fromExerciseStates(currentExercise, stateModel);

    if (transitionsChanged && transitionModel && currentExercise)
        UiUtils::fromStateTransitions(currentExercise->getTransitionTable(), transitionModel);

    if (constraintsChanged && constraintModel && currentState)
        UiUtils::fromStateConstraints(currentState->getConstraints(), constraintModel);

    workoutChange = false;
    exercisesChanged = false;
    statesChanged = false;
    transitionsChanged = false;
    constraintsChanged = false;
}

bool TrainerWorkoutDesignWidget::isModified()
{
    //Este método deberá solicitar si se quieren guardar cambios
    //si es un Workout se grabará en la base de datos, si no es así se guardará en currentWorkout
    QString tabla;
    if (workoutChange) tabla ="Workouts";
    if (exercisesChanged) tabla ="Exercise";
    if (statesChanged) tabla ="States";
    if (transitionsChanged) tabla ="Transitions";
    if (constraintsChanged) tabla ="Constraints";


    if(workoutChange||exercisesChanged||statesChanged||transitionsChanged||constraintsChanged){
    QString msg = QString("Tienes cambios sin guardar en %1. ¿Deseas guardarlos?").arg(tabla);
    auto res = QMessageBox::question(this, "Cambios sin guardar", msg, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (res == QMessageBox::Yes) {

        applyAllChangesToWorkout();
        //controller->getTrainingManager()->saveWorkout(currentWorkout);

    } else if (res == QMessageBox::No) {
        discardAllChanges();
    } else {
        return false;
    }
        }
    return true;

}

void TrainerWorkoutDesignWidget::onModelModified()
{
    //Este slot se activa cuando se editan datos de alguna fila de alguna tabla
    //para informar que esos datos han cambiado
    if (sender() == workoutModel)
        workoutChange = true;
    else if (sender() == exerciseModel)
        exercisesChanged = true;
    else if (sender() == stateModel)
        statesChanged = true;
    else if (sender() == transitionModel)
        transitionsChanged = true;
    else if (sender() == constraintModel)
        constraintsChanged = true;
}

void TrainerWorkoutDesignWidget::on_new_WorkoutButton_clicked()
{
    // Crear nuevo workout con valores por defecto
    auto newWorkout = QSharedPointer<TrainingWorkout>::create();

    newWorkout->setIdWorkout(controller->getTrainingManager()->getNextWorkoutId());
    newWorkout->setName("Nuevo Workout");
    newWorkout->setDescription("");
    newWorkout->setAuthor(currentTrainer ? currentTrainer->getUserName() : "");
    buffer.append(newWorkout);
    currentWorkout = newWorkout;
    currentExercise.clear();
    currentState.reset();

    // Agregar nueva fila al modelo existente
    QList<QVariant> newRow = {
        newWorkout->getIdWorkout(),
        newWorkout->getName(),
        newWorkout->getDescription(),
        newWorkout->getAuthor()
    };
    workoutModel->addRow(newRow);
    ui->tableWorkouts->resizeColumnsToContents();

    // Limpiar las otras tablas
    if (exerciseModel) exerciseModel->setDataSet({}, {});
    if (stateModel) stateModel->setDataSet({}, {});
    if (transitionModel) transitionModel->setDataSet({}, {});
    if (constraintModel) constraintModel->setDataSet({}, {});

    //workoutChange = true;
}


void TrainerWorkoutDesignWidget::on_new_ExerciseButton_clicked()
{
    if (currentWorkout==nullptr) return;
    QSharedPointer<ExerciseEspec> newExercise = QSharedPointer<ExerciseEspec>::create();
    newExercise->setIdEx(controller->getTrainingManager()->getNextExerciseId());
    currentWorkout->addExercise(newExercise);
    currentExercise=newExercise;
    UiUtils::fromWorkoutExercises(currentWorkout, exerciseModel);

    ui->tableExercises->resizeColumnsToContents();
    //exercisesChanged = true;
}


void TrainerWorkoutDesignWidget::on_ne_StateButton_clicked()
{
    if (currentExercise==nullptr) return;

    currentState=QSharedPointer<State>::create();
    currentExercise->addState(*currentState);

    if (constraintModel) constraintModel->setDataSet({}, {});

    //UiUtils::updateExerciseStates(currentExercise, stateModel);

    UiUtils::fromExerciseStates(currentExercise, stateModel);
    ui->tableStates->resizeColumnsToContents();
    //statesChanged = true;
    currentWorkout->replaceExercise(currentExercise);
}


void TrainerWorkoutDesignWidget::on_new_TransitionButton_clicked()
{
    if (currentExercise==nullptr) return;
    //if (currentState==nullptr) return;

    if (currentExercise->getStatesList().size()<2) {
            QMessageBox::warning(this, "Transición inválida", "Debes tener al menos dos estados para crear una transición.");
            return;
        }

    // Obtener una lista de estados disponibles
    const QList<State>& states = currentExercise->getStatesList();
    if (states.size() < 2) {
        QMessageBox::warning(this, "Transición inválida", "Debes tener al menos dos estados para crear una transición.");
        return;
    }

    int idFrom = states.first().getId();
    int idTo = states.last().getId();

    // Crear condición por defecto
    Condition defaultCond(ConditionType::exerciseInit, "");
    QPair<int, int> pair(idFrom, idTo);

    currentExercise->addTransition(pair,defaultCond) ;
    currentWorkout->replaceExercise(currentExercise);
    qDebug(trainerWorkoutDesignWidget)<<"Agregada Transicion, total: "<<currentExercise->getTransitionTable().size();

    QHash<QPair<int, int>, QSet<Condition>>  transitions=currentExercise->getTransitionTable();

    // Recargar modelo
    UiUtils::fromStateTransitions(transitions, transitionModel);

    ui->tableTransitions->resizeColumnsToContents();
    //transitionsChanged = true;
}


void TrainerWorkoutDesignWidget::on_new_ConstraintButton_clicked()
{
      if (currentState==nullptr) return;
      currentState->addAngleConstraint("NewConstraint",AngleConstraint());

      currentExercise->updateState(*currentState);
      qDebug(trainerWorkoutDesignWidget)<<"Ejercicio Id: "<<currentExercise->getIdEx()<<" con numero de estados: "<<currentExercise->getStates().size();
      currentWorkout->replaceExercise(currentExercise);

     UiUtils::fromStateConstraints(currentState->getConstraints(), constraintModel);
     ui->tableConstraints->resizeColumnsToContents();

       //constraintsChanged=true;

}


void TrainerWorkoutDesignWidget::on_delete_StateButton_clicked()
{
    if (currentExercise== nullptr || !stateModel) return;

    QModelIndex selectedIndex = ui->tableStates->selectionModel()->currentIndex();

    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Eliminar Estado", "No se ha seleccionado ningún estado válido.");
        return;
    }

    int row = selectedIndex.row();
    int stateId = stateModel->data(selectedIndex.siblingAtColumn(0)).toInt();

    // Confirmamos a eliminacion del estado
    auto reply = QMessageBox::question(this, "Eliminar Estado",
                                       QString("¿Deseas eliminar el estado con ID '%1'?").arg(stateId),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Eliminamos el estado en currentExercise
        currentExercise->delState(stateId);

        // Actualizamos el ejercicio dentro del workout
        if (currentWorkout) {
            currentWorkout->replaceExercise(currentExercise);
        }
        // Actualizamos la tabla de estados
        UiUtils::fromExerciseStates(currentExercise, stateModel);
        ui->tableStates->resizeColumnsToContents();
        //statesChanged = true;

        if (currentState && currentState->getId() == stateId) {
            currentState = nullptr;
        //Como no hay state seleccionado limpiamos la tabla de constraints
        if (constraintModel) constraintModel->setDataSet({}, {});
        }
    }
}



void TrainerWorkoutDesignWidget::on_delete_ConstraintButton_clicked()
{
    if (currentState == nullptr || !constraintModel) return;

    QModelIndex selectedIndex = ui->tableConstraints->selectionModel()->currentIndex();

    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Eliminar Constraint", "No se ha seleccionado ninguna constraint válida.");
        return;
    }

    int row = selectedIndex.row();
    QString line = constraintModel->data(selectedIndex.siblingAtColumn(0)).toString();


    auto reply = QMessageBox::question(this, "Eliminar Constraint",
                                       QString("¿Deseas eliminar la constraint '%1'?").arg(line),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        currentState->delAngleConstraint(line);

        // Actualizamos  también el ejercicio y el workout
        if (currentExercise) {
            currentExercise->updateState(*currentState);
            currentWorkout->replaceExercise(currentExercise);
        }

        // Actualizamos la tabla de constraints
        UiUtils::fromStateConstraints(currentState->getConstraints(), constraintModel);
        ui->tableConstraints->resizeColumnsToContents();

        //constraintsChanged = true;
    }
}


void TrainerWorkoutDesignWidget::on_delete_TransitionButton_clicked()
{
    if (currentExercise == nullptr|| !transitionModel) return;

    QModelIndex selectedIndex = ui->tableTransitions->selectionModel()->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Eliminar Transición", "No se ha seleccionado ninguna transición válida.");
        return;
    }

    int row = selectedIndex.row();
    int origen = transitionModel->data(selectedIndex.siblingAtColumn(0)).toInt();
    int destino = transitionModel->data(selectedIndex.siblingAtColumn(1)).toInt();
    QString type=transitionModel->data(selectedIndex.siblingAtColumn(2)).toString();
    QString line=transitionModel->data(selectedIndex.siblingAtColumn(3)).toString();

    // Confirmamos la  eliminación
    auto reply = QMessageBox::question(this, "Eliminar Transición",
                                       QString("¿Deseas eliminar la transición en: %1 de:%2 a: %3, con condicion:%4?").arg(line).arg(origen).arg(destino).arg(type),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Eliminar la transición en currentState
        Condition cond= Condition(conditionTypeFromString(type),line);
        currentExercise->delTransition(QPair(origen,destino), cond);

        // Actualizar el modelo de transiciones
        UiUtils::fromStateTransitions(currentExercise->getTransitionTable(), transitionModel);
        ui->tableTransitions->resizeColumnsToContents();

        //transitionsChanged = true;
    }
}


void TrainerWorkoutDesignWidget::on_delete_ExerciseButton_clicked()
{
    if (!exerciseModel) return;

    QModelIndex selectedIndex = ui->tableExercises->selectionModel()->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Eliminar Ejercicio", "No se ha seleccionado ningun ejercicio válido.");
        return;
    }

    int row = selectedIndex.row();
    int idExercise = exerciseModel->data(selectedIndex.siblingAtColumn(0)).toInt();
    QString exerciseName = exerciseModel->data(selectedIndex.siblingAtColumn(2)).toString();

    // Preguntamos si queremos seguir conla  eliminación
    auto reply = QMessageBox::question(this, "Eliminar Ejercicio",
                                       QString("¿Deseas eliminar el ejercicio '%1' (ID: %2)?")
                                           .arg(exerciseName)
                                           .arg(idExercise),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Eliminamos el ejercicio del workout
        currentWorkout->removeExercise(idExercise);
        // Actualizamos el modelo
         UiUtils::fromWorkoutExercises(currentWorkout, exerciseModel);
         ui->tableExercises->resizeColumnsToContents();
        exercisesChanged = true;
         //Como no hay exercise seleccionado limpiamos las tablas inferiores
         if (stateModel) stateModel->setDataSet({}, {});
         if (transitionModel) transitionModel->setDataSet({}, {});
         if (constraintModel) constraintModel->setDataSet({}, {});
    }


}


void TrainerWorkoutDesignWidget::on_delete_WorkoutButton_clicked()
{
    if (!workoutModel) return;

    QModelIndex selectedIndex = ui->tableWorkouts->selectionModel()->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Eliminar Workout", "No se ha seleccionado ningún workout válido.");
        return;
    }

    int row = selectedIndex.row();
    int idWorkout = workoutModel->data(selectedIndex.siblingAtColumn(0)).toInt();
    QString workoutName = workoutModel->data(selectedIndex.siblingAtColumn(1)).toString();

    // Confirmamos si quiere eliminar el workout
    auto reply = QMessageBox::warning(this, "Eliminar Workout",
                                       QString("¿Deseas eliminar el workout '%1' (ID: %2)?\
                                                Se borrará de la base de datos")
                                           .arg(workoutName)
                                           .arg(idWorkout),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Eliminamos el workout del buffer y de la BD
        controller->getTrainingManager()->deleteWorkout(idWorkout);

        for (int i = 0; i < buffer.size(); ++i) {
            if (buffer[i]->getIdWorkout() == idWorkout) {
                buffer.removeAt(i);
            }
        }

        // Actualizamos el modelo
        UiUtils::fromWorkoutSummaries(controller->getTrainingManager()->listWorkouts(DBFilter::none, {}), workoutModel);
        ui->tableWorkouts->setModel(workoutModel);
        ui->tableWorkouts->resizeColumnsToContents();
        workoutChange = true;

        //Como no hay workout seleccionado limpiamos las tablas inferiores
        if (exerciseModel) exerciseModel->setDataSet({}, {});
        if (stateModel) stateModel->setDataSet({}, {});
        if (transitionModel) transitionModel->setDataSet({}, {});
        if (constraintModel) constraintModel->setDataSet({}, {});
    }
}

void TrainerWorkoutDesignWidget::loadExerciseLibrary()
{
    if (!controller) return;

    auto exerciseSummaries = controller->getTrainingManager()->listExercises();

    exerciseLibraryModel = new UiTableModel(this);
    UiUtils::fromExerciseSummaries(exerciseSummaries, exerciseLibraryModel);

    ui->tableExercisesLibrary->setModel(exerciseLibraryModel);
    ui->tableExercisesLibrary->resizeColumnsToContents();
}


void TrainerWorkoutDesignWidget::on_AddToWorkout_Button_clicked()
{
    if (!currentWorkout || !exerciseLibraryModel) return;

    QModelIndex selectedIndex = ui->tableExercisesLibrary->currentIndex();
    if (!selectedIndex.isValid()) return;

    int row = selectedIndex.row();
    if (row < 0 || row >= exerciseLibraryModel->rowCount()) return;

    auto rows = exerciseLibraryModel->getDataSet();
    int idEx = rows[row][0].toInt();
    if (idEx <= 0) return;

    auto espec = controller->getTrainingManager()->getExercise(idEx);
    if (!espec) return;

    currentWorkout->addExercise(espec);

    if (exerciseModel) {
        UiUtils::fromWorkoutExercises(currentWorkout, exerciseModel);
        ui->tableExercises->resizeColumnsToContents();
    }

    workoutChange = true;
}


void TrainerWorkoutDesignWidget::onHeaderClickedExercises(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->tableExercises, exerciseProxyModel, logicalIndex);
}

void TrainerWorkoutDesignWidget::onHeaderClickedWorkouts(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->tableWorkouts, workoutProxyModel, logicalIndex);
}

void TrainerWorkoutDesignWidget::onHeaderClickedExercisesLibrary(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->tableExercisesLibrary, exerciseLibraryProxyModel, logicalIndex);
}

void TrainerWorkoutDesignWidget::onHeaderClickedStates(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->tableStates, statesProxyModel, logicalIndex);
}

void TrainerWorkoutDesignWidget::onHeaderClickedConstraints(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->tableConstraints, constraintsProxyModel, logicalIndex);
}

void TrainerWorkoutDesignWidget::onHeaderClickedTransitions(int logicalIndex)
{
    UiUtils::showFilterPopup(this, ui->tableTransitions, transitionsProxyModel, logicalIndex);
}


void TrainerWorkoutDesignWidget::on_clearFilters_Button_clicked()
{
    if (exerciseProxyModel) exerciseProxyModel->setFilterFixedString("");
    if (workoutProxyModel) workoutProxyModel->setFilterFixedString("");
    if (exerciseLibraryProxyModel) exerciseLibraryProxyModel->setFilterFixedString("");
    if (statesProxyModel) statesProxyModel->setFilterFixedString("");
    if (constraintsProxyModel) constraintsProxyModel->setFilterFixedString("");
    if (transitionsProxyModel) transitionsProxyModel->setFilterFixedString("");
}

void TrainerWorkoutDesignWidget::assignComboBoxTransitions()
{
    QStringList conditionTypesList;
    for (int i = static_cast<int>(ConditionType::MaxAngle); i <= static_cast<int>(ConditionType::ExerciseOvertime); ++i) {
        conditionTypesList << conditionTypeToString(static_cast<ConditionType>(i));
    }
    ComboBoxDelegate* conditionTypeDelegate = new ComboBoxDelegate(this, conditionTypesList);
    ui->tableTransitions->setItemDelegateForColumn(2, conditionTypeDelegate);
}

void TrainerWorkoutDesignWidget::assignComboBoxConstraints()
{
    QStringList directionsList;
    directionsList << directionToString(Direction::Increase)
                   << directionToString(Direction::Decrease)
                   << directionToString(Direction::Steady)
                   << directionToString(Direction::Symetrical)
                   << directionToString(Direction::Not_matter);
    ComboBoxDelegate* directionDelegate = new ComboBoxDelegate(this, directionsList);
    ui->tableConstraints->setItemDelegateForColumn(9, directionDelegate);
}

void TrainerWorkoutDesignWidget::assignComboBoxTransitionLines()
{
    QStringList connectionNames = controller->getConnections().values();
    connectionNames.removeDuplicates();
    connectionNames.sort();

    ComboBoxDelegate* lineDelegate = new ComboBoxDelegate(this, connectionNames);
    ui->tableTransitions->setItemDelegateForColumn(3, lineDelegate);
}

void TrainerWorkoutDesignWidget::assignComboBoxTransitionViews()
{
    QStringList poseViewList;
    poseViewList << PoseViewToString(PoseView::Front)
                 << PoseViewToString(PoseView::Left)
                 << PoseViewToString(PoseView::Right)
                 << PoseViewToString(PoseView::bottom_up)
                 << PoseViewToString(PoseView::top_down);
    ComboBoxDelegate* poseViewDelegate = new ComboBoxDelegate(this, poseViewList);
    ui->tableTransitions->setItemDelegateForColumn(4, poseViewDelegate);
}

void TrainerWorkoutDesignWidget::assignComboBoxConstraintLines()
{
    QStringList connectionNames = controller->getConnections().values();
    connectionNames.removeDuplicates();
    connectionNames.sort();

    ComboBoxDelegate* lineDelegate = new ComboBoxDelegate(this, connectionNames);
    ui->tableConstraints->setItemDelegateForColumn(3, lineDelegate);
}

void TrainerWorkoutDesignWidget::assignComboBoxConstraintViews()
{
    QStringList poseViewList;
    poseViewList << PoseViewToString(PoseView::Front)
                   << PoseViewToString(PoseView::Left)
                   << PoseViewToString(PoseView::Right)
                   << PoseViewToString(PoseView::bottom_up)
                   << PoseViewToString(PoseView::top_down);
    ComboBoxDelegate* poseViewDelegate = new ComboBoxDelegate(this, poseViewList);
    ui->tableConstraints->setItemDelegateForColumn(13, poseViewDelegate);
}

void TrainerWorkoutDesignWidget::lockWorkoutTableColumns()
{
    if (workoutModel)
        workoutModel->setColumnReadOnly(0);
}

void TrainerWorkoutDesignWidget::lockExercisesTableColumns()
{
    if (exerciseModel)
        exerciseModel->setColumnReadOnly(0);
}

void TrainerWorkoutDesignWidget::lockStatesTableColumns()
{
    if (stateModel)
        stateModel->setColumnReadOnly(0);
        stateModel->setColumnReadOnly(1);
        //stateModel->setColumnReadOnly(2);
}

void TrainerWorkoutDesignWidget::lockTransitionsTableColumns()
{
    if (transitionModel) {
        //transitionModel->setColumnReadOnly(0);
        //transitionModel->setColumnReadOnly(3);
    }
}

void TrainerWorkoutDesignWidget::lockConstraintsTableColumns()
{
    if (constraintModel)
        constraintModel->setColumnReadOnly(0);
        constraintModel->setColumnReadOnly(1);
        constraintModel->setColumnReadOnly(2);
        //constraintModel->setColumnReadOnly(3);
        //constraintModel->setColumnReadOnly(9);
}


void TrainerWorkoutDesignWidget::on_AssignButton_clicked()
{
    QModelIndex idx = ui->tableWorkouts->currentIndex();
    if (!idx.isValid()) {
        QMessageBox::warning(this, "Asignar Plan", "Seleccione un Plan de Entrenamiento.");
        return;
    }

    QModelIndex sourceIndex = workoutProxyModel->mapToSource(idx);
    if (!sourceIndex.isValid()) {
        QMessageBox::warning(this, "Asignar Plan", "Seleccione un Plan de Entrenamiento.");
        return;
    }
    int idWorkout = workoutModel->data(sourceIndex.siblingAtColumn(0)).toInt();

    AssignWorkoutDialog d(controller, idWorkout,this);
    if (d.exec() == QDialog::Accepted) {
            QMessageBox::information(this, "Éxito", "Workout asignado correctamente.");

    }
}




















