/**
 * @file userclientsesionexecution.h
 * @brief Declaración de la clase UserClientSesionExecution, interfaz para la ejecución guiada de una sesión de entrenamiento con retroalimentación visual y auditiva.
 *
 * Esta clase implementa un diálogo interactivo en el que el usuario ejecuta una sesión de entrenamiento,
 * recibiendo imágenes de las cámaras, gráficos de feedback, mensajes informativos y control sonoro en tiempo real.
 * Gestiona tanto el tiempo de trabajo y descanso como el análisis de condiciones biomecánicas.
 */

#ifndef USERCLIENTSESIONEXECUTION_H
#define USERCLIENTSESIONEXECUTION_H

#include <QDialog>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <opencv2/core/mat.hpp>
#include "core/appcontroller.h"
#include "pose/feedback.h"
#include "ui/client/feedbackgraphwidget.h"
#include "workouts/trainingsesion.h"
#include "ui_userclientsesionexecution.h"
#include "core/soundfeedbackmanager.h"

/**
 * @class UserClientSesionExecution
 * @brief Diálogo de ejecución de sesión de entrenamiento con feedback audiovisual.
 *
 * Este diálogo es responsable de mostrar el flujo en tiempo real de una sesión de entrenamiento,
 * incluyendo:
 * - Imagen frontal y lateral del usuario (desde cámaras)
 * - Gráfica resumen del feedback recibido (óptimo, alerta, crítico)
 * - Progreso temporal y ejecución
 * - Mensajes detallados de condiciones biomecánicas
 * - Control del volumen, tipos de sonido y silenciamiento
 *
 * Internamente gestiona los estados de descanso, ejecución, temporización y control del feedback auditivo.
 */
class UserClientSesionExecution : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del diálogo de sesión.
     * @param sesion Objeto que representa la sesión de entrenamiento que se va a ejecutar.
     * @param controller Controlador principal de la aplicación.
     * @param soundManager Gestor de sonido para emitir feedback auditivo.
     * @param parent Widget padre (por defecto nullptr).
     */
    explicit UserClientSesionExecution(QSharedPointer<TrainingSesion> sesion,
                                       AppController* controller,
                                       QSharedPointer<SoundFeedbackManager> soundManager,
                                       QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~UserClientSesionExecution();

public slots:
    /**
     * @brief Muestra la imagen capturada por la cámara frontal.
     * @param image Imagen frontal en formato OpenCV.
     */
    void onNewImage1(const cv::Mat& image);

    /**
     * @brief Muestra la imagen capturada por la cámara lateral.
     * @param image Imagen lateral en formato OpenCV.
     */
    void onNewImage2(const cv::Mat& image);

    /**
     * @brief Procesa el feedback recibido y actualiza visualización, sonidos y mensajes.
     * @param feedback Objeto FeedBack con las condiciones detectadas.
     */
    void onFeedbackReceived(const FeedBack& feedback);

private slots:
    void on_ReadyButon_clicked();        ///< Inicia la sesión.
    void on_pauseButton_clicked();       ///< Pausa o reanuda la sesión.
    void on_EndSesionButton_clicked();   ///< Finaliza la sesión de entrenamiento.

    void on_criticalButton_clicked();    ///< Activa/desactiva el sonido para condiciones críticas.
    void on_alertsButton_clicked();      ///< Activa/desactiva el sonido para condiciones de alerta.
    void on_infoButton_clicked();        ///< Activa/desactiva el sonido para mensajes informativos.
    void on_muteButton_clicked();        ///< Activa o desactiva el modo silencio.
    void on_volumeDownButton_clicked();  ///< Disminuye el volumen.
    void on_volumeUpButton_clicked();    ///< Aumenta el volumen.

private:
    Ui_UserClientSesionExecution *ui; ///< Interfaz gráfica generada por Qt Designer.
    AppController* controller; ///< Controlador de aplicación.
    QSharedPointer<SoundFeedbackManager> soundManager; ///< Manejador de feedback sonoro.
    QSharedPointer<TrainingSesion> currentSesion; ///< Sesión de entrenamiento actual.
    QSharedPointer<ExerciseSummary> sum; ///< Resumen del ejercicio en ejecución.
    QTimer* sessionTimer; ///< Temporizador de sesión.
    FeedBackGraphWidget* graphWidget; ///< Widget de gráfica de condiciones.

    int elapsedSeconds; ///< Tiempo transcurrido desde el inicio.
    bool inRest; ///< Indica si el usuario está en descanso.
    int restTime; ///< Tiempo de descanso configurado.
    bool inExecution; ///< Indica si la sesión está en ejecución.
    bool waitingForExecutionActivation=true;///< Indica si la sesión se puede poner en ejecución o no.
    int executionTime; ///< Tiempo de ejecución actual.
    int maxReps; ///< Número máximo de repeticiones.
    int maxSets; ///< Número máximo de series.
    int maxDuration; ///< Duración máxima permitida.
    int maxRestTime; ///< Tiempo máximo de descanso.

    QHash<ConditionType, int> lastConditionTime; ///< Control de repetición de condiciones según tiempo.
    int minConditionInterval = 3; ///< Intervalo mínimo entre mensajes de una misma condición (segundos).

    bool alertSoundEnabled = true; ///< Sonido activado para alertas.
    bool infoSoundEnable = true; ///< Sonido activado para mensajes informativos.
    bool criticaSoundEnable = true; ///< Sonido activado para condiciones críticas.
    bool mute = false; ///< Modo silencio activado.
    qreal currentVolume = 0.8; ///< Volumen actual (0.0 a 1.0).

    /**
     * @brief Actualiza el texto del contador de tiempo.
     */
    void updateTimeLabel();

    /**
     * @brief Muestra mensajes de feedback textual en la interfaz.
     * @param feedback FeedBack recibido.
     */
    void appendFeedbackMessages(const FeedBack& feedback);

    /**
     * @brief Determina si se debe emitir efecto o sonido para una condición dada.
     * @param type Tipo de condición.
     * @return true si se debe emitir sonido/efecto, false si se debe suprimir.
     */
    bool fireEffect(ConditionType type);
};

#endif // USERCLIENTSESIONEXECUTION_H
