/**
 * @file posemanager.h
 * @brief Declaración de la clase PoseManager, encargada de la captura, sincronización y análisis de poses.
 *
 * Esta clase gestiona la conexión con procesos externos en Python encargados de detectar keypoints
 * usando modelos de visión por computadora. Recoge los resultados desde memoria compartida,
 * los sincroniza si hay múltiples vistas, y ejecuta el análisis biomecánico con una máquina de estados.
 */

#ifndef POSEMANAGER_H
#define POSEMANAGER_H

#include <QMainWindow>
#include <QLoggingCategory>
#include <QProcess>
#include <QVector>
#include <QPair>
#include <QQueue>
#include <QTimer>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include "pose/pose.h"
#include "pose/statemachine.h"
#include "pose/feedback.h"
#include <QDateTime>
#include <qthread.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <QCoreApplication>
#include <qfile.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "workouts/trainingsesion.h"

Q_DECLARE_LOGGING_CATEGORY(PoseManagerLog)

/**
 * @class PoseManager
 * @brief Controlador de alto nivel que captura y analiza poses del usuario en tiempo real.
 *
 * Coordina la ejecución de scripts Python para capturar datos de cámaras,
 * lee keypoints desde memoria compartida, crea objetos `Pose`, y los analiza usando
 * una máquina de estados para generar feedback durante el ejercicio.
 */
class PoseManager : public QObject {
    friend class TestPoseManager;
    Q_OBJECT

public:
    /**
     * @brief Constructor por defecto.
     */
    explicit PoseManager(QObject *parent = nullptr);

    /**
     * @brief Destructor. Libera recursos compartidos y detiene procesos.
     */
    ~PoseManager();

    /**
     * @brief Inicializa la sesión y el análisis biomecánico.
     * @param sesion Objeto `TrainingSesion` en curso.
     * @param espec Especificación del ejercicio.
     * @param dual Indica si se usa modo con dos cámaras.
     */
    void init(QSharedPointer<TrainingSesion> sesion,QSharedPointer<ExerciseEspec> espec, bool dual=false);

    /**
     * @brief Inicia el análisis biomecánico sobre las poses recibidas.
     */
    void runAnalysis();

    /**
     * @brief Pausa el análisis biomecánico.
     */
    void pauseAnalysis();

    /**
     * @brief Aplica configuración externa de cámaras, keypoints y conexiones.
     */
    void applyConfiguration(QHash<QString, QVariant>& config,QHash<QPair<int, int>, QString>& conn,
                            QHash<int, QString>& kpts);

    /**
     * @brief Libera recursos compartidos como memoria y semáforos.
     */
    void resetMemory();
    /**
     * @brief Interrumpe la serie actual y reinicia la máquina de estados.
     *
     * Este método permite reiniciar la serie activa sin finalizar el ejercicio completo.
     * Se reinician los contadores de repetición, se incrementa el número de serie
     * y se restablece el estado inicial de la máquina de estados para comenzar la siguiente serie.
     */
    void newSerie();
signals:
    void newImage1(cv::Mat image);          ///< Imagen de la cámara 1 con keypoints.
    void newImage2(cv::Mat image);          ///< Imagen de la cámara 2 con keypoints.
    void feedbackGenerated(FeedBack feedback); ///< Feedback biomecánico generado.
    void exerciseCompleted();               ///< Señal emitida al finalizar el ejercicio.
    void exerciseInit();                    ///< Inicio del ejercicio.
    void repetitionComplete();              ///< Final de una repetición.
    void setComplete();                     ///< Final de una serie.
    void timeout(QString msg);              ///< Timeout detectado durante la captura.

public slots:
    /**
     * @brief Detiene la captura de poses y limpia recursos.
     */
    void stopCapture();

    void PythonProccesLogOutput1();   ///< Captura salida estándar del proceso Python 1.
    void PythonProccesErrorOutput1(); ///< Captura errores del proceso Python 1.
    void PythonProccesLogOutput2();   ///< Captura salida estándar del proceso Python 2.
    void PythonProccesErrorOutput2(); ///< Captura errores del proceso Python 2.

    /**
     * @brief Activa el modo test y carga las poses desde disco.
     * @param folderPath Carpeta con imágenes y JSONs.
     */
    void enableTestMode(const QString& folderPath);

private slots:
    /**
     * @brief Función ejecutada periódicamente para procesar el siguiente frame.
     */
    void processNextFrame();

private:
    QProcess *process_cam1, *process_cam2;
    bool infoMessage = true;
    bool alerts = true;
    bool critical = true;

    QTimer* frameTimer = nullptr;
    int64_t lastTimestamp1 = -1;
    int64_t lastTimestamp2 = -1;
    bool configured = false;
    char* shm_1;
    char* shm_2;
    bool dualMode;
    bool running;
    bool runningAnalysis=false;
    QTimer* timer = nullptr;
    int WIDTH, HEIGHT, FRAME_SIZE, JSON_SIZE, FLAG_SIZE, TOTAL_SIZE;
    QString CAM_1, CAM_2, SEM_SHM1, SEM_SHM2;
    QString pythonScript;
    QString PythonEnv;
    sem_t* sem1, *sem2;
    int shm_fd1, shm_fd2;

    int initFrameCount = 0;
    int MAX_ALLOWED_MISSES = 5;
    int STARTING_MISSES_FRAMES = 30;
    PoseView view1 = PoseView::Front;
    PoseView view2 = PoseView::Right;

    QQueue<QPair<int64_t, QSharedPointer<Pose>>> poseQueue;
    QSharedPointer<Pose> lastValidSidePose;
    int view1MissCount = 0;
    int view2MissCount = 0;

    QQueue<QPair<int64_t, QHash<QString, double>>> angleQueueView2;
    int SYNC_TOLERANCE_MS = 200;

    QSharedPointer<TrainingSesion> runningSesion;
    QSharedPointer<StateMachine> poseAnalyzer;

    // --- Test Mode ---
    bool testMode = false;
    QString testInputFolder;
    QStringList testFrames;
    int currentTestFrameIndex = 0;

    /**
     * @brief Lanza los procesos Python para captura de cámara.
     */
    void startPythonProcesses();

    /**
     * @brief Conecta con la memoria compartida y semáforos.
     * @return true si la conexión fue exitosa.
     */
    bool connectSharedMemory();

    /**
     * @brief Detiene los procesos Python de captura.
     */
    void stopPythonProcesses();

    /**
     * @brief Obtiene una nueva pose desde la memoria compartida o disco.
     * @param cam_name Nombre de recurso compartido.
     * @param sem Semáforo asociado.
     * @param shm_pose Memoria compartida.
     * @param view Vista correspondiente (frontal/lateral).
     * @return Puntero a Pose o nullptr si falla.
     */
    Pose* getNewPose(QString cam_name, sem_t* sem, uchar* shm_pose, PoseView view = PoseView::Front);


    /**
     * @brief Método auxiliar para pruebas de memoria compartida.
     */
    void testShareMemory();

    QHash<QPair<int, int>, QString> connections; ///< Conexiones entre keypoints.
};

#endif // POSEMANAGER_H
