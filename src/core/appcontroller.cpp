#include "appcontroller.h"
#include "ui/init_loader.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "ui/client/userclientsesionexecution.h"
#include "ui/main/mainwindow.h"
#include <QStandardPaths>
// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(AppControllerLog, "Controller")


/**
 * @file appcontroller.cpp
 * @brief Implementación de la clase AppController, que coordina los módulos de la aplicación.
 */

////////////////////////////////////////////////////////////////////////////////
/// @brief Constructor por defecto del controlador de la aplicación.
/// @param parent Objeto padre dentro del sistema Qt (opcional).
////////////////////////////////////////////////////////////////////////////////
AppController::AppController(QObject* parent)
    : QObject(parent)
{ }

////////////////////////////////////////////////////////////////////////////////
/// @brief Destructor del AppController.
/// Libera recursos, aunque en este caso los QSharedPointer aseguran liberación automática.
////////////////////////////////////////////////////////////////////////////////
AppController::~AppController() {}

////////////////////////////////////////////////////////////////////////////////
/// @brief Inicializa todos los componentes centrales de la aplicación.
///
/// Esta función carga la configuración desde JSON, crea la instancia de DBManager,
/// y los gestores de usuario, login, entrenamiento, captura de pose, feedback auditivo,
/// validación y métricas. También conecta señales clave entre módulos.
///
/// Si la base de datos no existe, puede lanzarse Init_Loader (comentado por ahora).
////////////////////////////////////////////////////////////////////////////////
void AppController::initialize() {


    loadConfig();
    //QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/data.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "/data.db";

    bool dbExists = QFile::exists(dbPath);
    dbManager = QSharedPointer<DBManager>::create(dbPath,this);

    QSharedPointer<UserRepository> userRepo = QSharedPointer<UserRepository>(new UserRepository(dbManager.data()));
    userManager = QSharedPointer<UserManager>::create(userRepo,this);

    loginManager = QSharedPointer<LoginManager>::create(dbManager, userManager,this);

    auto trainingRepo = QSharedPointer<TrainingRepository>::create(dbManager.data());
    trainingManager = QSharedPointer<TrainingManager>::create(trainingRepo);

    poseManager = QSharedPointer<PoseManager>::create(this);
    soundFeedbackManager = QSharedPointer<SoundFeedbackManager>::create(this);
    validationManager = QSharedPointer<ValidationManager>::create(this);
    metricsManager=QSharedPointer<MetricsManager>::create();


    connect(loginManager.data(), &LoginManager::loginSuccess, this, &AppController::onUserLoggedIn);
    connect(loginManager.data(), &LoginManager::newClientReg, this, &AppController::newClientRegistration);
    connect(userManager.data(), &UserManager::userLoggedOut, this, &AppController::onUserLoggedOut);

    // Crear y mostrar MainWindow

    mainWindow.reset(new MainWindow(self));  // Usas el QSharedPointer real, no QSharedPointer(this)
    mainWindow->show();

    //if (!dbExists) {
        //Init_Loader u;
       // u.load(dbManager);
   // }
    if (!dbExists) {
        if (!dbManager->initializeSchema()) {
            qCritical(AppControllerLog) << "Error inicializando esquema.";
            exit(1);
        }
        Init_Loader loader;
        loader.load(dbManager);
    }

        // dbManager->get(TypeBDEnum::User).print();
        //  dbManager->get(TypeBDEnum::Client).print();
        //  dbManager->get(TypeBDEnum::ClientWorkoutCalendar).print();
        //  dbManager->get(TypeBDEnum::Trainer).print();
        //   dbManager->get(TypeBDEnum::Admin).print();
        // dbManager->get(TypeBDEnum::TrainingWorkout).print();
        // dbManager->get(TypeBDEnum::ExerciseEspec).print();
        // dbManager->get(TypeBDEnum::WorkoutExercises).print();
        // dbManager->get(TypeBDEnum::State).print();
        // dbManager->get(TypeBDEnum::AngleConstraint).print();
        // dbManager->get(TypeBDEnum::Transition).print();
        // dbManager->get(TypeBDEnum::ExerciseSummary).print();
}
void AppController::setSelf(QSharedPointer<AppController> selfController) {
    if (!self.isNull()) {
        qWarning(AppControllerLog) << "AppController::setSelf() ya fue llamado. Ignorado.";
        return;
    }
    self = selfController;
}


void AppController::initializeAppData() {
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);  // ~/Library/Application Support/pfg/
    QDir dir(basePath);

    if (!dir.exists()) {
        dir.mkpath(".");
        qDebug() << "Creando carpeta base:" << basePath;
    }

    // Crear subdirectorios
    dir.mkpath("sounds");
    dir.mkpath("config");

    // Lista de archivos a copiar
    struct CopyItem { QString src; QString dst; };
    QList<CopyItem> filesToCopy = {
                                   {":/mediaPipe/VideoCapture.py", basePath + "/VideoCapture.py"},
                                   {":/config/poseConfig.json", basePath + "/config/poseConfig.json"},
                                   {":/config/schema.sql", basePath + "/config/schema.sql"},
                                   {":/sounds/tadaa.wav", basePath + "/sounds/tadaa.wav"},
                                   {":/sounds/camera_qt.wav", basePath + "/sounds/camera_qt.wav"},
                                   {":/sounds/good_job.wav", basePath + "/sounds/good_job.wav"},
                                   {":/sounds/overload.wav", basePath + "/sounds/overload.wav"},
                                   {":/sounds/rest.wav", basePath + "/sounds/rest.wav"},
                                   {":/sounds/set.wav", basePath + "/sounds/set.wav"},
                                   {":/sounds/rep.wav", basePath + "/sounds/rep.wav"},
                                   {":/sounds/slow.wav", basePath + "/sounds/slow.wav"},
                                   {":/sounds/fast.wav", basePath + "/sounds/fast.wav"},
                                   {":/sounds/time.wav", basePath + "/sounds/time.wav"},
                                   {":/sounds/No_stop.wav", basePath + "/sounds/No_stop.wav"},
                                   {":/sounds/symetry.wav", basePath + "/sounds/symetry.wav"},
                                   {":/sounds/Wrong_exec.wav", basePath + "/sounds/Wrong_exec.wav"},
                                   {":/sounds/Not_steady.wav", basePath + "/sounds/Not_steady.wav"},
                                   {":/sounds/go.wav", basePath + "/sounds/go.wav"},
                                   };

    // Copiar archivos si no existen
    for (const auto& item : filesToCopy) {
        if (!QFile::exists(item.dst)) {
            if (QFile::copy(item.src, item.dst)) {
                qDebug() << "Copiado:" << item.dst;
            } else {
                qWarning() << "Error al copiar:" << item.src << "->" << item.dst;
            }
        }
    }

    // Base de datos (solo si no existe ya)
    QString dbPath = basePath + "/data.db";
    if (!QFile::exists(dbPath)) {
        // Copiar o generar desde schema
        QString schemaSrc = basePath + "/config/schema.sql";
        QFile schemaFile(schemaSrc);
        if (schemaFile.exists()) {
            // Aquí podrías ejecutar el SQL para crear la BD vacía, si deseas.
            // Por ahora, solo te deja la SQL en el sistema.
            qDebug() << "Base de datos aún no implementada desde schema.";
        }
    }
}

/**
 * @brief Devuelve el puntero compartido al LoginManager.
 */
QSharedPointer<LoginManager> AppController::getLoginManager() { return loginManager; }

/**
 * @brief Devuelve el puntero compartido al UserManager.
 */
QSharedPointer<UserManager> AppController::getUserManager() { return userManager; }

/**
 * @brief Devuelve el puntero compartido al TrainingManager.
 */
QSharedPointer<TrainingManager> AppController::getTrainingManager() { return trainingManager; }

/**
 * @brief Devuelve el puntero compartido al PoseManager.
 */
QSharedPointer<PoseManager> AppController::getPoseManager() { return poseManager; }

/**
 * @brief Devuelve el puntero compartido al ValidationManager.
 */
QSharedPointer<ValidationManager> AppController::getValidationManager() {
    return validationManager;
}

/**
 * @brief Devuelve el mapa actual de conexiones entre keypoints.
 */
QHash<QPair<int, int>, QString> AppController::getConnections() const {
    return connections;
}

/**
 * @brief Devuelve el tamaño máximo del buffer para capturas de pose.
 */
int AppController::getMaxBufferSize() const {
    return maxBufferSize;
}

/**
 * @brief Inicia el flujo de login. Actualmente sin implementación.
 */
void AppController::startLoginFlow() {

}
/**
 * @brief Slot que se ejecuta cuando un usuario inicia sesión con éxito.
 *
 * Establece el usuario activo, carga las preferencias y conexiones,
 * y emite la señal correspondiente.
 *
 * @param user Usuario autenticado.
 */
void AppController::onUserLoggedIn(QSharedPointer<User> user) {
    if (!user.isNull()) {
        userManager->setCurrentUser(user);
        // Cargar y aplicar preferencias
        UserPreferences prefs = userManager->getUserPreferences(user->getId());
        //QHash<QString, QVariant> config;
        poseCaptureConfig["DualMode"] = prefs.getDualMode();
        poseCaptureConfig["InfoSound"] = prefs.getInfoSound();
        poseCaptureConfig["AlertSound"] = prefs.getAlertSound();
        poseCaptureConfig["Mute"] = prefs.getMute();
        //setPoseCaptureConfig(config);

        if (!prefs.getConnectionsJson().isEmpty()) {
            nlohmann::json json = nlohmann::json::parse(prefs.getConnectionsJson().toStdString());
            QHash<QPair<int, int>, QString> conns;
            for (const auto& item : json) {
                int from = item[0];
                int to = item[1];
                conns.insert({from, to}, QString("%1-%2").arg(from).arg(to));
            }
            setConnections(conns);
        }

        emit userManager->userLoggedIn(user);

    }

}
/**
 * @brief Slot que se ejecuta al registrarse un nuevo cliente.
 *
 * Crea un nuevo cliente a partir de los datos recibidos.
 *
 * @param data Hash de campos definidos en UserField con sus valores.
 */
void AppController::newClientRegistration(QHash<UserField, QVariant>  data){

    if (!data.isEmpty()) {
        userManager->createClient(data,QSharedPointer<ClientProfile> (new ClientProfile));
    }

}
/**
 * @brief Slot que se ejecuta cuando el usuario cierra sesión.
 *
 * Actualmente vacío, se puede usar para realizar limpieza adicional.
 */
void AppController::onUserLoggedOut() {

}

/**
 * @brief Carga la configuración desde el archivo poseConfig.json.
 *
 * Valida las claves requeridas, carga parámetros de captura,
 * conexiones y keypoints. La configuración se almacena en poseCaptureConfig.
 */
void AppController::loadConfig() {

    //QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config/poseConfig.json";
    QString configPath = QCoreApplication::applicationDirPath() + "/config/poseConfig.json";
    qDebug(AppControllerLog) << "Buscando config.json en:" << configPath;

    std::ifstream file(configPath.toStdString());
    if (!file.is_open()) {
        qCritical(AppControllerLog) << "No se pudo abrir config.json";
        return;
    }
    nlohmann::json config;
    file >> config;
    file.close();

    QStringList requiredKeys = {"WIDTH", "HEIGHT", "JSON_SIZE", "FLAG_SIZE", "PYTHON_SCRIPT", "CAM1", "CAM2", "SEM_SHM1", "SEM_SHM2","TEST_MODE", "TEST_FOLDER"};

    for (const QString& key : requiredKeys) {
        if (!config.contains(key)) {
            qCritical(AppControllerLog) << "Falta la clave en poseConfig.json:" << key;
            exit(1);
        }

    }


    poseCaptureConfig.insert("WIDTH", config["WIDTH"].get<int>());
    poseCaptureConfig.insert("HEIGHT", config["HEIGHT"].get<int>());
    poseCaptureConfig.insert("JSON_SIZE", config["JSON_SIZE"].get<int>());
    poseCaptureConfig.insert("FLAG_SIZE", config["FLAG_SIZE"].get<int>());
    poseCaptureConfig.insert("PYTHON_SCRIPT", QString::fromStdString(config["PYTHON_SCRIPT"]));
     poseCaptureConfig.insert("PYTHON_ENV", QString::fromStdString(config["PYTHON_ENV"]));
    poseCaptureConfig.insert("CAM1", QString::fromStdString(config["CAM1"]));
    poseCaptureConfig.insert("CAM2", QString::fromStdString(config["CAM2"]));
    poseCaptureConfig.insert("SEM_SHM1", QString::fromStdString(config["SEM_SHM1"]));
    poseCaptureConfig.insert("SEM_SHM2", QString::fromStdString(config["SEM_SHM2"]));
    //QString testMode= QString::fromStdString(config["TEST_MODE"]).toLower();
    //poseCaptureConfig.insert("TEST_MODE",(testMode=="true")? true:false);
    poseCaptureConfig.insert("TEST_MODE", config["TEST_MODE"].get<bool>());
    poseCaptureConfig.insert("TEST_FOLDER", QString::fromStdString(config["TEST_FOLDER"]));

    if (config.contains("DUALMODE")) {
        //QString mode= QString::fromStdString(config["DUALMODE"]).toLower();
        //if (mode=="true") dualmode=true;
        //               else dualmode=false;
         poseCaptureConfig.insert("DUALMODE", config["DUALMODE"].get<bool>());
    }

    if (config.contains("VIEW1")) poseCaptureConfig.insert("VIEW1", QString::fromStdString(config["VIEW1"]));
    if (config.contains("VIEW2")) poseCaptureConfig.insert("VIEW2", QString::fromStdString(config["VIEW2"]));
    if (config.contains("STARTING_MISSING_FRAMES")) poseCaptureConfig.insert("STARTING_MISSING_FRAMES", config["STARTING_MISSING_FRAMES"].get<int>());
    if (config.contains("MAX_ALLOWED_MISSES")) poseCaptureConfig.insert("MAX_ALLOWED_MISSES", config["MAX_ALLOWED_MISSES"].get<int>());

    if (config.contains("BUFFER_SIZE")) {
        maxBufferSize= (config["BUFFER_SIZE"].get<int>()>0)?config["BUFFER_SIZE"].get<int>():100;

    }

    if (config.contains("KEYPOINTS")) {
       nlohmann:: json keys = config["KEYPOINTS"];
        for (auto it = keys.begin(); it != keys.end(); ++it) {
            int key = std::stoi(it.key());
            QString name = QString::fromStdString(it.value());
            keypoints.insert(key, name);
        }
    }


    if (config.contains("CONNECTIONS")) {
       nlohmann:: json connArray = config["CONNECTIONS"];
        for (const auto& conn : connArray) {
            int from = conn[0];
            int to = conn[1];
            QString fromName = keypoints.value(from, QString::number(from));
            QString toName = keypoints.value(to, QString::number(to));
            QString name = fromName + "<->" + toName;
            connections.insert(qMakePair(from, to), name);
        }
    }

    // Mostrar valores cargados
    qDebug(AppControllerLog) << "PYTHON_SCRIPT:" << poseCaptureConfig["PYTHON_SCRIPT"].toString();
      qDebug(AppControllerLog) << "PYTHON_ENV:" << poseCaptureConfig["PYTHON_ENV"].toString();
    qDebug(AppControllerLog) << "CAM_1:" << poseCaptureConfig["CAM1"].toString();
    qDebug(AppControllerLog) << "CAM_2:" << poseCaptureConfig["CAM2"].toString();
    qDebug(AppControllerLog) << "WIDTH:" << poseCaptureConfig["WIDTH"].toInt();
    qDebug(AppControllerLog) << "HEIGHT:" << poseCaptureConfig["HEIGHT"].toInt();
    qDebug(AppControllerLog) << "JSON_SIZE:" << poseCaptureConfig["JSON_SIZE"].toInt();
    qDebug(AppControllerLog) << "FLAG_SIZE:" << poseCaptureConfig["FLAG_SIZE"].toInt();
    qDebug(AppControllerLog) << "SEM_SHM1:" << poseCaptureConfig["SEM_SHM1"].toString();
    qDebug(AppControllerLog) << "SEM_SHM2:" << poseCaptureConfig["SEM_SHM2"].toString();
    qDebug(AppControllerLog) << "DUALMODE:" << poseCaptureConfig["DUALMODE"].toString();
     qDebug(AppControllerLog) << "VIEW1:" << poseCaptureConfig["VIEW1"].toString();
     qDebug(AppControllerLog) << "VIEW2:" << poseCaptureConfig["VIEW2"].toString();
     qDebug(AppControllerLog) << "TEST_MODE:" << poseCaptureConfig["TEST_MODE"].toString();
     qDebug(AppControllerLog) << "TEST_FOLDER:" << poseCaptureConfig["TEST_FOLDER"].toString();

     qDebug(AppControllerLog) << "CONNECTIONS:";
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        qDebug(AppControllerLog) << "(" << it.key().first << "," << it.key().second << ") ->" << it.value();
    }
    qDebug(AppControllerLog) << "==============================";

}


/**
 * @brief Devuelve el hash de configuración de captura actual.
 */
QHash<QString, QVariant> AppController::getPoseCaptureConfig() const {
    return poseCaptureConfig;
}

/**
 * @brief Establece una nueva configuración de captura.
 * @param newPoseCaptureConfig Mapa clave-valor con nueva configuración.
 */
void AppController::setPoseCaptureConfig(const QHash<QString, QVariant> &newPoseCaptureConfig) {
    poseCaptureConfig = newPoseCaptureConfig;
}

/**
 * @brief Establece los keypoints desde un mapa externo.
 * @param newKeypoints Hash de ID -> nombre del keypoint.
 */
void AppController::setKeypoints(const QHash<int, QString> &newKeypoints) {
    keypoints = newKeypoints;
}

/**
 * @brief Devuelve el conjunto de keypoints actuales.
 */
QHash<int, QString> AppController::getKeypoints() const {
    return keypoints;
}

/**
 * @brief Establece las conexiones entre keypoints.
 * @param newConnections Hash de pares de ID -> nombre de la conexión.
 */
void AppController::setConnections(const QHash<QPair<int, int>, QString> &newConnections) {
    connections = newConnections;
}
/**
 * @brief Inicia una sesión de ejercicio para un cliente y un ejercicio dados.
 *
 * Se crean las estructuras necesarias (`TrainingSesion`, `ExerciseEspec`),
 * se configura el `PoseManager`, se lanza el diálogo `UserClientSesionExecution`
 * y se inicia la captura de poses.
 *
 * @param clientId ID del cliente.
 * @param exerciseId ID del ejercicio.
 */
void AppController::startExerciseSesion(int clientId, int exerciseId) {

     sesion = trainingManager->startSesion(clientId, exerciseId);

    if (!sesion) {
        qCritical(AppControllerLog) << "No se pudo iniciar la sesión de entrenamiento";
        return;
    }

     espec = trainingManager->getExercise(exerciseId);
    if (!espec) {
        qCritical(AppControllerLog) << "No se encontró la especificación del ejercicio";
        return;
    }

    auto dialog = new UserClientSesionExecution(sesion,this,soundFeedbackManager);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(poseManager.data(), &PoseManager::newImage1,
            dialog, &UserClientSesionExecution::onNewImage1);
    connect(poseManager.data(), &PoseManager::newImage2,
            dialog, &UserClientSesionExecution::onNewImage2);
    connect(poseManager.data(), &PoseManager::feedbackGenerated,
            dialog, &UserClientSesionExecution::onFeedbackReceived);
    // connect(poseManager.data(), &PoseManager::exerciseCompleted,
    //         dialog, &UserClientSesionExecution::onExerciseComplete);
    // connect(poseManager.data(), &PoseManager::exerciseInit,
    //         dialog, &UserClientSesionExecution::onExerciseInit);
    // connect(poseManager.data(), &PoseManager::repetitionComplete,
    //         dialog, &UserClientSesionExecution::onRepetitionComplete);
    // connect(poseManager.data(), &PoseManager::setComplete,
    //         dialog, &UserClientSesionExecution::onSetComplete);
    // connect(poseManager.data(), &PoseManager::timeout,
    //         dialog, &UserClientSesionExecution::onTimeoutReceived);

    //connect(dialog, &QDialog::accepted, this, [=]() {trainingManager->endCurrentSesion();});
    poseManager->resetMemory();
    dialog->show();
     poseManager->applyConfiguration(poseCaptureConfig,connections,keypoints);


    QMetaObject::invokeMethod(this, "startPoseCapture", Qt::QueuedConnection);
    qDebug(AppControllerLog)<<"Sesion de cliente" << clientId <<"iniciada para el ejercicio: "<<  exerciseId;

}
/**
 * @brief Finaliza la sesión de ejercicio en curso y detiene la captura.
 * @return true si se guardó correctamente; false en caso contrario.
 */
bool AppController::finishExerciseSesion(){

     poseManager->stopCapture();
    if (trainingManager->endCurrentSesion()){
        qDebug(AppControllerLog)<<"Sesion guargada correctamente";
        return true;
    }else  {
        qDebug(AppControllerLog)<<"La Sesion no se ha podido guardar correctamente";
        return false;
    }
    //disconnect(poseManager.data(), nullptr, dialog, nullptr);
}
/**
 * @brief Inicia el análisis post-sesión de los datos de captura.
 */
void AppController::startSesionAnalysis() {
    poseManager->runAnalysis();
}

/**
 * @brief Devuelve el puntero compartido al MetricsManager.
 */
QSharedPointer<MetricsManager> AppController::getMetricsManager() const {
    return metricsManager;
}

/**
 * @brief Pausa el análisis de la sesión actual.
 */
void AppController::pauseSesion() {
    poseManager->pauseAnalysis();
}
/**
 * @brief Slot invocado para iniciar la captura de poses.
 *
 * Inicializa el `PoseManager` con los datos de sesión y especificación del ejercicio.
 */
void AppController::startPoseCapture() {
    if (!poseManager.isNull() && sesion && espec) {
        poseManager->init(sesion, espec,dualmode);
    }
    qDebug(AppControllerLog)<<"*****Inciando captura "<<sesion->getExerciseSummary()->getDuration()<<sesion->getExerciseSummary()->getRepetitions();
}


void AppController::interruptSerie() {
    if (!poseManager.isNull()) {
        poseManager->newSerie();
        qDebug(AppControllerLog) << "Serie interrumpida manualmente desde AppController.";
    }
}









