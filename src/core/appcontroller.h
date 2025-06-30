#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include "core/dbmanager.h"
#include "core/metricsmanager.h"
#include "core/validationmanager.h"
#include "loginmanager.h"
//#include "ui/main/mainwindow.h"
#include "usermanager.h"
#include "trainingmanager.h"
#include "posemanager.h"
#include <QLoggingCategory>
#include "soundfeedbackmanager.h"

class MainWindow;

Q_DECLARE_LOGGING_CATEGORY(AppControllerLog)

/**
 * @class AppController
 * @brief Clase central de control de la aplicación.
 *
 * La clase AppController actúa como punto de orquestación principal de la aplicación,
 * coordinando el flujo de trabajo entre los distintos módulos: autenticación de usuarios,
 * gestión de entrenamientos, captura y análisis de movimiento, configuración del sistema,
 * generación de métricas y feedback auditivo.
 *
 * Esta clase encapsula la lógica principal de inicialización del sistema, el arranque de sesiones
 * de ejercicio, la configuración de captura de pose desde archivo, y la gestión del login y logout
 * de usuarios. También proporciona interfaces para acceder a los diferentes gestores lógicos de la aplicación.
 *
 * @see LoginManager, UserManager, TrainingManager, PoseManager, ValidationManager, SoundFeedbackManager, MetricsManager
 */
class AppController : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor explícito del controlador principal de la aplicación.
     * @param parent El objeto padre Qt (por defecto nullptr).
     */
    explicit AppController(QObject* parent = nullptr);

    /**
     * @brief Destructor del controlador.
     */
    ~AppController();

    /**
     * @brief Inicializa todos los componentes y servicios internos de la aplicación.
     *
     * Carga la configuración desde el archivo `poseConfig.json`, inicializa la base de datos,
     * crea los repositorios y gestores lógicos, y conecta las señales principales.
     */
    void initialize();

    /**
     * @brief Inicia el flujo de autenticación, normalmente mostrando la interfaz de login.
     */
    void startLoginFlow();

    /// Métodos de acceso a los distintos gestores de la aplicación.
    QSharedPointer<LoginManager> getLoginManager();
    QSharedPointer<UserManager> getUserManager();
    QSharedPointer<TrainingManager> getTrainingManager();
    QSharedPointer<PoseManager> getPoseManager();
    QSharedPointer<SoundFeedbackManager> getSoundFeedbackManager();
    QSharedPointer<ValidationManager> getValidationManager();
    QSharedPointer<MetricsManager> getMetricsManager() const;

    /**
     * @brief Devuelve el mapa de conexiones entre keypoints.
     * @return QHash con pares de keypoints conectados y sus etiquetas.
     */
    QHash<QPair<int, int>, QString> getConnections() const;

    /**
     * @brief Establece nuevas conexiones entre keypoints.
     * @param newConnections Mapa de conexiones.
     */
    void setConnections(const QHash<QPair<int, int>, QString>& newConnections);

    /**
     * @brief Devuelve la configuración de captura de pose cargada desde el archivo JSON.
     */
    QHash<QString, QVariant> getPoseCaptureConfig() const;

    /**
     * @brief Establece la configuración de captura de pose.
     * @param newPoseCaptureConfig Mapa de configuración.
     */
    void setPoseCaptureConfig(const QHash<QString, QVariant>& newPoseCaptureConfig);

    /**
     * @brief Establece el mapa de keypoints utilizados en el análisis.
     * @param newKeypoints Mapa de identificadores de keypoints y sus nombres.
     */
    void setKeypoints(const QHash<int, QString>& newKeypoints);

    /**
     * @brief Obtiene el mapa de keypoints actuales.
     * @return QHash con keypoints.
     */
    QHash<int, QString> getKeypoints() const;

    /**
     * @brief Devuelve el tamaño máximo del buffer para captura de frames.
     * @return Tamaño máximo de buffer.
     */
    int getMaxBufferSize() const;

    /**
     * @brief Inicia una sesión de ejercicio para un cliente y un ejercicio específico.
     *
     * Crea una nueva instancia de `TrainingSesion`, carga la especificación del ejercicio,
     * crea el diálogo de ejecución y configura el `PoseManager` para capturar la pose.
     *
     * @param clientId ID del cliente.
     * @param exerciseId ID del ejercicio.
     */
    void startExerciseSesion(int clientId, int exerciseId);

    /**
     * @brief Finaliza la sesión actual de ejercicio y guarda los resultados.
     * @return `true` si la sesión se guarda correctamente; `false` en caso contrario.
     */
    bool finishExerciseSesion();

    /**
     * @brief Pausa temporalmente el análisis de la sesión.
     */
    void pauseSesion();

    /**
     * @brief Inicia el análisis post-ejecución de la sesión capturada.
     */
    void startSesionAnalysis();
    /**
     * @brief Interrumpe la serie actual en ejecución.
     *
     * Este método llama a `interruptSerie()` en el `PoseManager`,
     * lo que provoca que la `StateMachine` reinicie los contadores de repetición,
     * incremente el número de serie y vuelva al estado inicial.
     *
     * Este método puede ser invocado desde la interfaz gráfica (por ejemplo, desde un botón)
     * cuando el usuario desea cortar la serie en curso.
     */
    void interruptSerie();
    void setSelf(QSharedPointer<AppController> selfController);
    /// Instancias compartidas de sesión y ejercicio en ejecución.
    QSharedPointer<TrainingSesion> sesion;
    QSharedPointer<ExerciseEspec> espec;

    void initializeAppData();
private:
    QSharedPointer<DBManager> dbManager;  ///< Gestor de acceso a base de datos SQLite.
    QSharedPointer<LoginManager> loginManager;  ///< Gestor de autenticación de usuarios.
    QSharedPointer<UserManager> userManager;  ///< Gestor de lógica relacionada con usuarios y perfiles.
    QSharedPointer<TrainingManager> trainingManager;  ///< Gestor de entrenamientos y ejercicios.
    QSharedPointer<PoseManager> poseManager;  ///< Gestor de captura y análisis de pose.
    QSharedPointer<SoundFeedbackManager> soundFeedbackManager; ///< Gestor de feedback auditivo.
    QSharedPointer<ValidationManager> validationManager; ///< Módulo de validación de condiciones de ejecución.
    QSharedPointer<MetricsManager> metricsManager; ///< Módulo de cálculo de métricas de rendimiento.
    QScopedPointer<MainWindow> mainWindow; ///< Gestor del interfaz de usuario
    QSharedPointer<AppController> self;  ///<  Para que pueda pasarse a MainWindow correctamente
    QHash<QPair<int, int>, QString> connections; ///< Conexiones entre keypoints usados para análisis biomecánico.
    QHash<QString, QVariant> poseCaptureConfig;  ///< Configuración de captura cargada desde JSON.
    QHash<int, QString> keypoints; ///< Nombres de los keypoints utilizados en los análisis.

    int maxBufferSize = 100; ///< Tamaño máximo del buffer de frames.
    bool dualmode = false; ///< Indica si la aplicación está en modo de captura dual.

    /**
     * @brief Carga la configuración inicial desde el archivo `poseConfig.json`.
     */
    void loadConfig();

private slots:
    /**
     * @brief Slot invocado al completarse con éxito el login de un usuario.
     * @param user Puntero compartido al usuario autenticado.
     */
    void onUserLoggedIn(QSharedPointer<User> user);

    /**
     * @brief Slot invocado cuando se completa el registro de un nuevo cliente.
     * @param data Mapa con los campos del nuevo cliente.
     */
    void newClientRegistration(QHash<UserField, QVariant> data);

    /**
     * @brief Slot que inicia la captura de poses con los parámetros actuales.
     */
    void startPoseCapture();

    /**
     * @brief Slot invocado cuando el usuario realiza logout del sistema.
     */
    void onUserLoggedOut();
};


#endif // APPCONTROLLER_H
