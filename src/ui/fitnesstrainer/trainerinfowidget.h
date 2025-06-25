#ifndef TRAINERINFOWIDGET_H
#define TRAINERINFOWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "ui_trainerInfoWidget.h"

class TrainerInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerInfoWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~TrainerInfoWidget();

    void setTrainer(QSharedPointer<FitnessTrainer> trainer);
    void updateTrainerInfo();
public slots:
    void on_newPhoto_clicked();
private:
    Ui_TrainerInfoWidget *ui;
    QSharedPointer<AppController> controller;
    QSharedPointer<FitnessTrainer> trainer;


};
#endif // TRAINERINFOWIDGET_H
