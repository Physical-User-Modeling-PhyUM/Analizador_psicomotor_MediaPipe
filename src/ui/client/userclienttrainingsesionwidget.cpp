#include "userclienttrainingsesionwidget.h"
#include "utils/uiutils.h"
#include <QDebug>
#include <QMessageBox>

Q_LOGGING_CATEGORY(TrainigSesionBoard, "TrainigSesionBoard")

UserClientTrainingSesionWidget::UserClientTrainingSesionWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_UserClientTrainingSesionWidget)
    , controller(controller)
{
    ui->setupUi(this);

    assignedModel = new UiTableModel(this);
    exerciseModel = new UiTableModel(this);

    ui->tableAssigned->setModel(assignedModel);
    ui->tableExercises->setModel(exerciseModel);

    connect(ui->tableAssigned, &QTableView::clicked, this, &UserClientTrainingSesionWidget::onAssignedWorkoutSelected);
    connect(ui->startSesionButton, &QPushButton::clicked,
            this, &UserClientTrainingSesionWidget::onStartSesionButtonClicked);
}

UserClientTrainingSesionWidget::~UserClientTrainingSesionWidget()
{
    delete ui;
}

void UserClientTrainingSesionWidget::setUser(QSharedPointer<Client> user)
{
    currentUser = user;
    loadAssignedWorkouts();
    loadCurrentWorkoutData();
}

void UserClientTrainingSesionWidget::loadAssignedWorkouts()
{

      UiUtils::fromWorkoutCalendar(currentUser->getWorkoutCalendar(), assignedModel);
}

void UserClientTrainingSesionWidget::loadCurrentWorkoutData()
{
    int workoutId = controller->getUserManager()->getUserWorkout(QDateTime::currentDateTime());
    if (workoutId < 0) return;

    QSharedPointer<TrainingWorkout> workout = controller->getTrainingManager()->getWorkoutData(workoutId);
    if (workout==nullptr) return;

    WorkoutSummary sum=workout->toSummary();
    updateWorkoutInfo(sum);

    UiUtils::fromWorkoutExercises(workout, exerciseModel);
}

void UserClientTrainingSesionWidget::updateWorkoutInfo(const WorkoutSummary& summary)
{
    ui->workout_label->setText(summary.getTitle());
    ui->description_label->setText(summary.getDescription());
    ui->Author_label->setText(summary.getAuthor());
    QString tot=QString::number(summary.getTotalExercises());
    ui->total_ex_label->setText(tot);
}

void UserClientTrainingSesionWidget::populateExerciseTable(const QList<ExerciseSummary>& exercises)
{

}

void UserClientTrainingSesionWidget::onAssignedWorkoutSelected(const QModelIndex& index)
{
    int row = index.row();

    if (row < 0 || row >= assignedModel->rowCount()) return;
    //QList<QList<QVariant>> d=assignedModel->getDataSet();
    QVariant idVar = assignedModel->data(assignedModel->index(row, 0));
    int workoutId = idVar.toInt();

    QSharedPointer<TrainingWorkout> workout = controller->getTrainingManager()->getWorkoutData(workoutId);


    if (workout==nullptr) return;

    WorkoutSummary sum=workout->toSummary();
    updateWorkoutInfo(sum);
    UiUtils::fromWorkoutExercises(workout, exerciseModel);
}


void UserClientTrainingSesionWidget::onStartSesionButtonClicked()
{
    QModelIndex index = ui->tableExercises->currentIndex();

    if (!index.isValid()) {
        QMessageBox::warning(this, "Atención", "Seleccione un ejercicio para iniciar la sesión.");
        return;
    }

    int row = index.row();
    QVariant idEx = exerciseModel->data(assignedModel->index(row, 0));
    selectedExerciseId = idEx.toInt();

    controller->startExerciseSesion(currentUser->getId(), selectedExerciseId);

    qDebug() << "Sesión iniciada para el ejercicio ID:" << selectedExerciseId;
}
