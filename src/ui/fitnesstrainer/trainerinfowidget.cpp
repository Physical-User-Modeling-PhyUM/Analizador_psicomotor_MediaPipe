#include "trainerinfowidget.h"
#include "ui_trainerInfoWidget.h"
#include <QFileDialog>
#include <QMessageBox>

#include <QPixmap>

TrainerInfoWidget::TrainerInfoWidget(QSharedPointer<AppController> controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui_TrainerInfoWidget),
    controller(controller)
{
    ui->setupUi(this);
    connect(ui->newPhoto, &QPushButton::clicked, this, &TrainerInfoWidget::on_newPhoto_clicked);

}


TrainerInfoWidget::~TrainerInfoWidget()
{
    delete ui;
}

void TrainerInfoWidget::setTrainer(QSharedPointer<FitnessTrainer> trainer)
{
    this->trainer = trainer;
    updateTrainerInfo();
}

void TrainerInfoWidget::updateTrainerInfo()
{
    if (!trainer) return;

    ui->IdUserLabel->setText(QString::number(trainer->getId()));
    ui->userNameLabel->setText(trainer->getUserName());
    ui->emailLabel->setText(trainer->getEmail());
    ui->logLabel->setText(trainer->getLast_login().toString("dd/MM/yyyy hh:mm"));
    ui->sinceLabel->setText(trainer->getJoin_up_date().toString("dd/MM/yyyy hh:mm"));

    QPixmap pixmap = QPixmap::fromImage(trainer->getProfile_Picture());
    ui->pictureLabel->setPixmap(pixmap);
    ui->pictureLabel->setScaledContents(true);
}
void TrainerInfoWidget::on_newPhoto_clicked()
{
    if (!trainer) return;

    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Selecciona una imagen de perfil"),
                                                    QString(),
                                                    tr("Imágenes (*.png *.jpg *.jpeg *.bmp)"));

    if (filePath.isEmpty())
        return;

    QImage image;
    if (!image.load(filePath)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo cargar la imagen."));
        return;
    }

    trainer->setProfile_Picture(image);
    controller->getUserManager()->updateUser(trainer);

    QPixmap pixmap = QPixmap::fromImage(image);
    ui->pictureLabel->setPixmap(pixmap);
    ui->pictureLabel->setScaledContents(true);

    QMessageBox::information(this, tr("Éxito"), tr("Imagen actualizada correctamente."));

}
