#include "userclientsesionexecution.h"
#include "ui_userclientsesionexecution.h"
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QDateTime>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
/// @brief Constructor. Inicializa interfaz, gráficos, temporizador y configuración de feedback auditivo.
UserClientSesionExecution::UserClientSesionExecution(QSharedPointer<TrainingSesion> sesion,
                                                     AppController* controller,
                                                     QSharedPointer<SoundFeedbackManager> soundManager,
                                                     QWidget *parent)
    : QDialog(parent),
    ui(new Ui_UserClientSesionExecution),
    currentSesion(sesion),
    elapsedSeconds(0),
    inRest(false),
    restTime(0),
    inExecution(false),
    executionTime(0),
    controller(controller),
    soundManager(soundManager)
{
    ui->setupUi(this);

    sessionTimer = new QTimer(this);
    connect(sessionTimer, &QTimer::timeout, this, &UserClientSesionExecution::updateTimeLabel);
    sessionTimer->start(1000);

     ui->restTimeLabel->setText("Rest Time: 00:00");
     ui->workingTimeLabel->setText("Working Time: 00:00");

     FeedBackGraphWidget* feedbackGraph = new FeedBackGraphWidget(this);
     feedbackGraph->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

     if (ui->feedBackGraphContainer->layout()) {
         ui->feedBackGraphContainer->layout()->addWidget(feedbackGraph);
     } else {
         QVBoxLayout* layout = new QVBoxLayout(ui->feedBackGraphContainer);
         layout->addWidget(feedbackGraph);
         ui->feedBackGraphContainer->setLayout(layout);
     }
     this->graphWidget = feedbackGraph;
     sum=sesion->getExerciseSummary();

     if (sum!=nullptr){

         (sum->getRepetitions()>-1)? maxReps=sum->getRepetitions():maxReps=0;
         (sum->getSeries()>-1)? maxSets=sum->getSeries():maxSets=0;
         (sum->getRestTime()>-1)? maxRestTime=sum->getRestTime():maxRestTime=0;
         (sum->getDuration()>-1)? maxDuration=sum->getDuration():maxDuration=0;

        ui->repsBar->setMinimum(0);
        ui->setBar->setMinimum(0);
        ui->restTimeBar->setMinimum(0);
        ui->workingTimeBar->setMinimum(0);
        ui->repsBar->setMaximum(maxReps);
        ui->setBar->setMaximum(maxSets);
        ui->restTimeBar->setMaximum(maxRestTime);
        ui->workingTimeBar->setMaximum(maxDuration);
        qDebug()<<"++Widget UserClientExecution++";

        qDebug() << " Duration:" << maxDuration << "Max sets:" << maxSets;
        qDebug() << " Rest Time:" << maxRestTime << "Max reps:" << maxReps;
        qDebug()<<"++++";
     }
     QString reps = QString("Reps: 0 of %1").arg(maxReps);
     ui->RepsLabel->setText(reps);
     QString setsstr = QString("Sets: 0 of %2").arg(maxSets);
     ui->SetLabel->setText(setsstr);

     ui->criticalMsgEdit->setReadOnly(true);
     ui->alertsMsgEdit->setReadOnly(true);
     ui->infoMsgEdit->setReadOnly(true);

     //configuramos los botones
     if (!mute)ui->muteButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
     else ui->muteButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");

     if (criticaSoundEnable)ui->criticalButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
     else ui->criticalButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");

     if (alertSoundEnabled)ui->alertsButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
     else ui->alertsButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");

     if (infoSoundEnable)ui->infoButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
     else ui->infoButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");
}
/// @brief Destructor.
UserClientSesionExecution::~UserClientSesionExecution()
{
    delete ui;
}
/// @brief Muestra imagen cámara principal capturada en tiempo real.
void UserClientSesionExecution::onNewImage1(const cv::Mat& image)
{
    if (image.empty()) return;
    cv::Mat img;
    cv::cvtColor(image, img, cv::COLOR_BGR2RGB);
    QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    QSize s=ui->frontImage->size();
    QPixmap pix=QPixmap::fromImage(qimg).scaled(s,Qt::KeepAspectRatio);
    ui->frontImage->setPixmap(pix);
}
/// @brief Muestra imagen secundaria capturada en tiempo real.
void UserClientSesionExecution::onNewImage2( const cv::Mat& image)
{
    qDebug() << "[UI] Imagen secundaria recibida en sideImage, tamaño:" << image.cols << "x" << image.rows;
    if (image.empty()) return;
    cv::Mat img;
    cv::cvtColor(image, img, cv::COLOR_BGR2RGB);
    QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    QSize s=ui->sideImage->size();
    QPixmap pix=QPixmap::fromImage(qimg).scaled(s,Qt::KeepAspectRatio);
    ui->sideImage->setPixmap(pix);
}
/// @brief Procesa feedback recibido y actualiza gráficas, mensajes y sonidos si corresponde.
void UserClientSesionExecution::onFeedbackReceived( const FeedBack& feedback)
{
    if (graphWidget!=nullptr && inExecution) graphWidget->updateFromFeedback(feedback);

    for (const ConditionType &type:feedback.getEntriesList()) {

        if (fireEffect(type))
        {
            QStringList lines;
            QString message = feedback.getMessage(type);
            if (feedback.isCritical(type)){
                ui->criticalMsgEdit->append(QString("<b style='color:red;'>[CRÍTICO] " + message + "</b>"+"\n"));
                if (!mute && criticaSoundEnable && inExecution)soundManager->play(type,ConditionCategory::critical);
            }
            else if (feedback.isAlert(type)){
                 ui->alertsMsgEdit->append(QString("<span style='color:orange;'>[ALERTA] " + message + "</span>"+"\n"));
                 if (!mute && alertSoundEnabled && inExecution)soundManager->play(type,ConditionCategory::alert);
            }
            else{
                 ui->infoMsgEdit->append(QString("<span style='color:gray;'>[INFO] " + message + "</span>"+"\n"));
                 if (!mute && infoSoundEnable && inExecution)soundManager->play(type,ConditionCategory::info);
            }
        }
        switch (type){
            case ConditionType::exerciseInit: {
                    if (waitingForExecutionActivation){
                        inRest=false;
                        inExecution=true;
                        ui->restTimeBar->setValue(0);
                        ui->restTimeLabel->setStyleSheet("QLabel { color : black; }");
                        ui->restTimeBar->setStyleSheet("QProgressBar::chunk { background-color: blue; }");
                    }
                    break;
            };
            case ConditionType::EndOfSet: {
                int current = ui->setBar->value() + 1;
                if (current > maxSets) {
                    ui->SetLabel->setStyleSheet("QLabel { color : red; }");
                }
                ui->setBar->setValue(current);
                QString setsstr = QString("Sets: %1 of %2").arg(current).arg(maxSets);
                ui->SetLabel->setText(setsstr);

                ui->repsBar->setValue(0);
                ui->repsBar->setStyleSheet("QProgressBar::chunk { background-color: blue; }");
                //ui->RepsLabel->setStyleSheet("QLabel { color : black; }");
                QString reps = QString("Reps: 0 of %1").arg(maxReps);
                ui->RepsLabel->setText(reps);

                inExecution = false;
                inRest = true;
                restTime = 0;
                waitingForExecutionActivation=false;
                controller->pauseSesion();
                break;
            }
            case ConditionType::EndOfRepetition: {
                int current = ui->repsBar->value() + 1;
                if (current > maxReps) {
                    ui->repsBar->setStyleSheet("QProgressBar::chunk { background-color: red; }");
                }
                ui->repsBar->setValue(current);
                QString reps = QString("Reps: %1 of %2").arg(current).arg(maxReps);
                ui->RepsLabel->setText(reps);
                break;
            }
            case ConditionType::RestTime: {
                waitingForExecutionActivation=true;

                break;
            }
            default:
                break;
         }
    }

}





/// @brief Procesa feedback recibido y actualiza gráficas, mensajes y sonidos si corresponde.
void UserClientSesionExecution::appendFeedbackMessages( const FeedBack& feedback)
{

}
/// @brief Lógica de temporizador que actualiza la interfaz de tiempo y progreso.
void UserClientSesionExecution::updateTimeLabel()
{
    elapsedSeconds++;
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;
    QString Sesiontime = QString("Sesion Time: %1:%2").arg(minutes, 2, 10, QChar('0'))
                             .arg(seconds, 2, 10, QChar('0'));
    ui->SesionTimeLabel->setText(Sesiontime);


    if (inRest) {
        restTime++;

        // Mostrar tiempo
        int minRest = restTime / 60;
        int secRest = restTime % 60;
        QString rest = QString("Rest Time: %1:%2").arg(minRest, 2, 10, QChar('0'))
                           .arg(secRest, 2, 10, QChar('0'));
        ui->restTimeLabel->setText(rest);

        // Avanzar barra proporcional o marcar exceso
        if (restTime <= maxRestTime) {
            ui->restTimeBar->setValue(restTime);
            ui->restTimeBar->setStyleSheet("QProgressBar::chunk { background-color: blue; }");
            ui->restTimeLabel->setStyleSheet("QLabel { color : green; }");
        } else {
            ui->restTimeBar->setValue(maxRestTime);
            ui->restTimeBar->setStyleSheet("QProgressBar::chunk { background-color: red; }");
            ui->restTimeLabel->setStyleSheet("QLabel { color : red; }");
        }

        // RESET de ejecución
        ui->workingTimeLabel->setText("Working Time: 00:00");
        ui->workingTimeLabel->setStyleSheet("QLabel { color : black; }");
        ui->workingTimeBar->setValue(0);
        ui->workingTimeBar->setStyleSheet("QProgressBar::chunk { background-color: black; }");
    }


    else if (inExecution) {
        executionTime++;

        int minExec = executionTime / 60;
        int secExec = executionTime % 60;
        QString execTime = QString("Working Time: %1:%2").arg(minExec, 2, 10, QChar('0'))
                               .arg(secExec, 2, 10, QChar('0'));
        ui->workingTimeLabel->setText(execTime);

        if (executionTime <= maxDuration) {
            ui->workingTimeBar->setValue(executionTime);
            ui->workingTimeBar->setStyleSheet("QProgressBar::chunk { background-color: blue; }");
            ui->workingTimeLabel->setStyleSheet("QLabel { color : green; }");
        } else {
            ui->workingTimeBar->setValue(maxDuration);
            ui->workingTimeBar->setStyleSheet("QProgressBar::chunk { background-color: red; }");
            ui->workingTimeLabel->setStyleSheet("QLabel { color : red; }");
        }

        // RESET de descanso
        ui->restTimeLabel->setText("Rest Time: 00:00");
        ui->restTimeLabel->setStyleSheet("QLabel { color : black; }");
        ui->restTimeBar->setValue(0);
        ui->restTimeBar->setStyleSheet("QProgressBar::chunk { background-color: black; }");
    }

}

/// @brief Activa el análisis de la sesión desde el controlador.
void UserClientSesionExecution::on_ReadyButon_clicked()
{
    controller->startSesionAnalysis();
    waitingForExecutionActivation=true;
}

/// @brief Pausa o reanuda la sesión.
void UserClientSesionExecution::on_pauseButton_clicked()
{
    if (sessionTimer->isActive()) {
        sessionTimer->stop();
        ui->pauseButton->setText("Resume");
        controller->pauseSesion();

    } else {
        sessionTimer->start();
        ui->pauseButton->setText("Pause");
        controller->startSesionAnalysis();
    }
}

/// @brief Finaliza la sesión y cierra el diálogo.
void UserClientSesionExecution::on_EndSesionButton_clicked()
{
    controller->finishExerciseSesion();
    this->accept();
}
/// @brief Controla si se debe mostrar un efecto o reproducir sonido según el tipo de condición.
bool UserClientSesionExecution::fireEffect(ConditionType type)
{
    int now = elapsedSeconds;
    if (!lastConditionTime.contains(type) || now - lastConditionTime[type] >= minConditionInterval) {
        lastConditionTime[type] = now;
        return true;
    }
    return false;
}
/// @brief Alterna el sonido para condiciones críticas.
void UserClientSesionExecution::on_criticalButton_clicked()
{

    criticaSoundEnable = !criticaSoundEnable;
    soundManager->setCriticaSoundEnable(criticaSoundEnable);
    if (criticaSoundEnable)ui->criticalButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
    else ui->criticalButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");
}

/// @brief Alterna el sonido para alertas.
void UserClientSesionExecution::on_alertsButton_clicked()
{
    alertSoundEnabled = !alertSoundEnabled;
     soundManager->setAlertSoundEnabled(alertSoundEnabled);
    if (alertSoundEnabled)ui->alertsButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
    else ui->alertsButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");

}

/// @brief Alterna el sonido para mensajes informativos.
void UserClientSesionExecution::on_infoButton_clicked()
{
    infoSoundEnable = !infoSoundEnable;
   soundManager->setInfoSoundEnable(infoSoundEnable);
    if (infoSoundEnable)ui->infoButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
    else ui->infoButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");
}

/// @brief Alterna el estado de silencio global.
void UserClientSesionExecution::on_muteButton_clicked()
{
    mute = !mute;
   soundManager->mute(mute);
    if (!mute)ui->muteButton->setStyleSheet("QPushButton { background-color: green; color: white; font-weight: bold; }");
    else ui->muteButton->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");
}

/// @brief Disminuye el volumen de salida.
void UserClientSesionExecution::on_volumeDownButton_clicked()
{
    if (currentVolume > 0.0) {
        currentVolume -= 0.1;
        soundManager->setVolume(currentVolume);
    }
}

/// @brief Aumenta el volumen de salida.
void UserClientSesionExecution::on_volumeUpButton_clicked()
{
    if (currentVolume < 1.0) {
        currentVolume += 0.1;
        soundManager->setVolume(currentVolume);
    }
}

