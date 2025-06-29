/**
 * @file posemanager.cpp
 * @brief Implementación de la clase PoseManager para captura, sincronización y análisis de poses en tiempo real.
 *
 * Este módulo se encarga de gestionar la comunicación con procesos de detección de poses en Python,
 * leer datos desde memoria compartida, sincronizar información entre múltiples vistas (cámaras),
 * y ejecutar una máquina de estados para analizar el movimiento y generar feedback.
 */


#include "posemanager.h"
#include <QDir>


// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(PoseManagerLog, "posemanager")
/**
 * @brief Constructor por defecto de la clase PoseManager.
 *
 * Inicializa el objeto y deja preparada la estructura para aplicar la configuración
 * y lanzar la captura de poses.
 */
PoseManager::PoseManager(QObject *parent)
    : QObject(parent)
{
    try {



        //loadConfig();
        //startPythonProcesses();
        qDebug(PoseManagerLog) << "Constructor PoseManager: dualMode= " << dualMode;
        //init();


    }catch(const std::exception &e) {
        qCritical(PoseManagerLog) << "Error fatal:" << e.what();
    }
}
/**
 * @brief Destructor de PoseManager.
 *
 * Detiene los procesos Python y libera la memoria compartida y semáforos si están activos.
 */
PoseManager::~PoseManager() {

    if (frameTimer) {
        frameTimer->stop();
        frameTimer->deleteLater();
        frameTimer = nullptr;
    }
    stopPythonProcesses();
    resetMemory();

}
/**
 * @brief Aplica la configuración necesaria para capturar y analizar poses.
 * @param config Diccionario con los parámetros de configuración.
 * @param conn Conexiones entre keypoints.
 * @param kpts Mapa de keypoints disponibles.
 */
void PoseManager::applyConfiguration(QHash<QString, QVariant>& config,QHash<QPair<int, int>, QString>& conn,
    QHash<int, QString>& kpts)
{
    WIDTH = config["WIDTH"].toInt();
    HEIGHT = config["HEIGHT"].toInt();
    JSON_SIZE = config["JSON_SIZE"].toInt();
    FLAG_SIZE = config["FLAG_SIZE"].toInt();
    FRAME_SIZE = WIDTH * HEIGHT * 3;
    TOTAL_SIZE = FRAME_SIZE + JSON_SIZE;

    CAM_1 = config["CAM1"].toString();
    CAM_2 = config["CAM2"].toString();
    SEM_SHM1 = config["SEM_SHM1"].toString();
    SEM_SHM2 = config["SEM_SHM2"].toString();
    pythonScript = config["PYTHON_SCRIPT"].toString();
    //view1=PoseViewFromString(config["VIEW1"].toString().toLower());
    //view2=PoseViewFromString(config["VIEW2"].toString().toLower());
    testInputFolder=config["TEST_FOLDER"].toString();
    testMode=config["TEST_MODE"].toBool();
    MAX_ALLOWED_MISSES=config["MAX_ALLOWED_MISSES"].toInt();
    STARTING_MISSES_FRAMES=config["STARTING_MISSING_FRAMES"].toInt();
    if (config.contains("DUALMODE")) dualMode = config["DUALMODE"].toBool();

    connections.clear();
    // for (const auto& pair : conn.keys()) {
    //     connections.append(pair);
    // }
    connections=conn;
    configured=true;
    if (testMode) {
        enableTestMode(testInputFolder);
    }
}


/**
 * @brief Lanza los procesos Python que capturan poses desde cámara.
 *
 * Inicia uno o dos procesos según el modo (`dualmode`), conectando sus señales de log y error.
 */
void PoseManager::startPythonProcesses() {
    qInfo(PoseManagerLog) << "Iniciando procesos de Python...";
    qInfo(PoseManagerLog) << "Ejecutando en modo dual..."<<dualMode;
    QString pythonPath = "/Users/MZT/vscode/MPipe/MediaPipe/venv/bin/python3";
    QString scriptPath = QDir(QCoreApplication::applicationDirPath()).filePath(pythonScript);

    qInfo(PoseManagerLog) << "Ejecutando script: " << scriptPath;

    // Proceso para cámara 1
    process_cam1 = new QProcess(this);
    process_cam1->setProgram(pythonPath);
    process_cam1->setArguments({scriptPath, "0"});

    connect(process_cam1, &QProcess::readyReadStandardOutput, this, &PoseManager::PythonProccesLogOutput1);
    connect(process_cam1, &QProcess::readyReadStandardError, this, &PoseManager::PythonProccesErrorOutput1);

    process_cam1->start();
    qDebug(PoseManagerLog) << "Ejecutando Python con: " << process_cam1->program() << process_cam1->arguments();
    //QThread::sleep(2);


    if (!process_cam1->waitForStarted(2000)) {
        qCritical(PoseManagerLog) << "Error al iniciar el proceso de cámara 1.";

    } else {
        qInfo(PoseManagerLog) << "Capturador de cámara 1 iniciado.";
    }

    // Proceso para cámara 2,
    if (dualMode) {
        process_cam2 = new QProcess(this);
        process_cam2->setProgram(pythonPath);
        process_cam2->setArguments({scriptPath, "1"});
        process_cam2->start();
        qDebug(PoseManagerLog) << "Ejecutando Python con: " << process_cam2->program() << process_cam2->arguments();

        connect(process_cam2, &QProcess::readyReadStandardOutput, this, &PoseManager::PythonProccesLogOutput2);
        connect(process_cam2, &QProcess::readyReadStandardError, this, &PoseManager::PythonProccesErrorOutput2);

        if (!process_cam2->waitForStarted(2000)) {
            qCritical(PoseManagerLog) << "Error al iniciar el proceso de cámara 2.";
        } else {
            qInfo(PoseManagerLog) << "Capturador de cámara 2 iniciado.";
        }
    }
}

/**
 * @brief Establece conexión con la memoria compartida y semáforos para lectura de datos.
 * @return true si la conexión fue exitosa, false en caso de error.
 */
bool PoseManager::connectSharedMemory() {

    qDebug(PoseManagerLog) << "PoseManager (C++)> Conectando a la memoria compartida...";

    // Esperar a que el script Python cree el archivo .ready
    QString readyFile1 = QCoreApplication::applicationDirPath() + "/" + CAM_1.mid(1) + ".ready";
    int waitCounter = 0;
    while (!QFile::exists(readyFile1)) {
        qInfo(PoseManagerLog) << "Esperando archivo de señal para cámara 1:" << readyFile1;
        QThread::sleep(1);
        waitCounter++;
        if (waitCounter > 10) {
            qCritical(PoseManagerLog) << "Timeout esperando archivo .ready para cámara 1";
            return false;
        }
    }
    qDebug(PoseManagerLog) << "Archivo .ready detectado. Procediendo a abrir la memoria compartida.";

    // Intentar abrir el semáforo
    sem1 = sem_open(SEM_SHM1.toUtf8().constData(), 0);
    if (sem1 == SEM_FAILED) {
        qCritical(PoseManagerLog) << "PoseManager (C++)> Error al abrir el semáforo. Asegúrate de que el capturador1 está corriendo.";
        return false;
    }

    qDebug(PoseManagerLog) << "PoseManager (C++)> Conectando a la memoria compartida...";



    // Intentar abrir la memoria compartida


    while ((shm_fd1 = shm_open(CAM_1.toUtf8().constData(), O_RDWR, 0666)) == -1) {

        qCritical(PoseManagerLog) << "> No se encontró la memoria compartida. Esperando al Capturador...";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Mapear la memoria compartida en el espacio de direcciones
    //shm_1 = (unsigned char*) mmap(NULL, TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
    shm_1 = (char*) mmap(NULL, TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
    if (shm_1 == MAP_FAILED) {
        qCritical(PoseManagerLog) << "> Error al mapear la memoria compartida "<<CAM_1;
        return false;
    }

    if (dualMode){
        qDebug(PoseManagerLog) << "PoseManager (C++)> Conectando a la memoria compartida de la segunda cámara...";

        QString readyFile2 = QCoreApplication::applicationDirPath() + "/" + CAM_2.mid(1) + ".ready";
        int waitCounter = 0;
        while (!QFile::exists(readyFile2)) {
            qInfo(PoseManagerLog) << "Esperando archivo de señal para cámara 2:" << readyFile2;
            QThread::sleep(1);
            waitCounter++;
            if (waitCounter > 10) {
                qCritical(PoseManagerLog) << "Timeout esperando archivo .ready para cámara 2";
                return false;
            }
        }
        qDebug(PoseManagerLog) << "Archivo .ready de cámara 2 detectado. Procediendo a abrir la memoria compartida.";

        sem2 = sem_open(SEM_SHM2.toUtf8().constData(), 0);
        if (sem2 == SEM_FAILED) {
            qCritical(PoseManagerLog) << "PoseManager (C++)> Error al abrir el semáforo de cámara 2.";
            return false;
        }

        shm_fd2 = shm_open(CAM_2.toUtf8().constData(), O_RDWR, 0666);
        while (shm_fd2 == -1) {
            shm_fd2 = shm_open(CAM_2.toUtf8().constData(), O_RDWR, 0666);
            qCritical(PoseManagerLog) << "> No se encontró la memoria compartida de cámara 2. Esperando...";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        shm_2 = (char*) mmap(NULL, TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);
        if (shm_2 == MAP_FAILED) {
            qCritical(PoseManagerLog) << "> Error al mapear la memoria compartida de cámara 2 " << CAM_2;
            return false;
        }
    }

    qDebug(PoseManagerLog) << "PoseManager (C++)> Conectado a la memoria compartida ...fd= "<<shm_fd1;
    return true;
}


/**
 * @brief Detiene y elimina los procesos Python de captura de datos.
 */
void PoseManager::stopPythonProcesses() {
    qInfo(PoseManagerLog) << "=== Capturador parado ===";

    if (process_cam1) {
        disconnect(process_cam1, nullptr, this, nullptr);
        process_cam1->terminate();
        if (!process_cam1->waitForFinished(2000)) {
            process_cam1->kill();
            process_cam1->waitForFinished();
        }
        process_cam1->deleteLater();
        process_cam1 = nullptr;
    }

    if (process_cam2) {
        disconnect(process_cam2, nullptr, this, nullptr);
        process_cam2->terminate();
        if (!process_cam2->waitForFinished(2000)) {
            process_cam2->kill();
            process_cam2->waitForFinished();
        }
        process_cam2->deleteLater();
        process_cam2 = nullptr;
    }

    qInfo(PoseManagerLog) << "Procesos Python detenidos correctamente.";
}


/**
 * @brief Lee una nueva pose desde la memoria compartida o desde archivos de test.
 * @param cam_name Nombre de la cámara (recurso).
 * @param sem Semáforo asociado a la memoria.
 * @param shm Puntero a la memoria compartida.
 * @param view Vista (frontal o lateral).
 * @return Puntero a la nueva Pose, o nullptr si hay error.
 */

Pose *PoseManager::getNewPose(QString cam_name,sem_t* sem,unsigned char *shm, PoseView view) {
     // --- Modo Test (lee las poses de una carpeta) ---
    if (testMode) {
        if (currentTestFrameIndex >= testFrames.size()) {
            qDebug(PoseManagerLog) << "No hay más frames de test disponibles.";
            return nullptr;
        }

        QString base = testFrames[currentTestFrameIndex++];
        QString jsonPath = QDir(testInputFolder).filePath(base + ".json");
        QString imgPath = QDir(testInputFolder).filePath(base + ".png");

        QFile jsonFile(jsonPath);
        if (!jsonFile.open(QIODevice::ReadOnly)) {
            qWarning(PoseManagerLog) << "No se pudo abrir JSON:" << jsonPath;
            return nullptr;
        }

        QByteArray jsonData = jsonFile.readAll();
        jsonFile.close();
        qDebug(PoseManagerLog) << "JSON leído:" << jsonPath;
        nlohmann::json poseData;
        try {
            poseData = nlohmann::json::parse(jsonData.toStdString());
        } catch (...) {
            qWarning(PoseManagerLog) << "Error al parsear JSON en testMode";
            return nullptr;
        }

        cv::Mat image = cv::imread(imgPath.toStdString());
        if (image.empty()) {
            qWarning(PoseManagerLog) << "No se pudo cargar imagen:" << imgPath;
            return nullptr;
        }
        qDebug(PoseManagerLog) << "Imagen leída:" << imgPath;
        return new Pose(poseData, image, connections);

    }


    // --- Modo normal (tiempo real desde memoria compartida) ---
    // leemos si la memoria está libre
    qDebug(PoseManagerLog) << "Verificando memoria en: " << cam_name;
    int64_t lastTimestamp = (view == view1) ? lastTimestamp1 : lastTimestamp2;
    sem_wait(sem);

    // Leer la imagen
    cv::Mat raw(HEIGHT, WIDTH, CV_8UC3, shm);
    cv::Mat image = raw.clone();

    // Leer el JSON
    std::string json_str(reinterpret_cast<char*>(shm + FRAME_SIZE), JSON_SIZE);
    json_str.erase(json_str.find('\0'));  // Eliminar bytes nulos al final

    // //QImage image_rgb(shm_1, WIDTH, HEIGHT, QImage::Format_RGB888);
    // std::string  json_str="";
    // cv::Mat image;

    // std::string mensaje(shm_1);
    //  std::cout << "Lector (C++)> Mensaje leído: " << mensaje << std::endl;
    // qDebug(PoseManagerLog)<<"Este es el mensaje"<<mensaje;
    // sem_post(sem);
    // return nullptr;
    sem_post(sem);

    if (json_str.empty()){
        qCritical(PoseManagerLog) << "Lector (C++)> JSON vacío, esperando datos...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // Intentamos interpretar los datos de memoria a un JSON
    nlohmann::json json_data;

    try{
        json_data = nlohmann::json::parse(json_str);

    } catch (const nlohmann::json::parse_error& e) {
        qCritical(PoseManagerLog) << "Error al interpretar la memoria como JSON: " << e.what();
        return nullptr;
    }

    // se lee el JSON y se busca un timestamp
    if (!json_data.contains("timestamp")) {
        qCritical(PoseManagerLog)<< "Error: La memoria no contiene un timestamp";
        return nullptr;
    }

    int64_t timestamp = json_data["timestamp"];

    // Comprobar si la pose ya fue procesada
    if (timestamp == lastTimestamp) {
        qWarning(PoseManagerLog) << "Error: La memoria no contiene una nueva pose";

        if (view == view1) lastTimestamp1 = timestamp;
        else lastTimestamp2 = timestamp;

       // return nullptr;
    }

    // Actualizar el timestamp y crear la nueva Pose
    if (view==view1) lastTimestamp1 = timestamp;
    else lastTimestamp2 = timestamp;


    //generamos la Pose
    return  new Pose(json_data,image,connections);
    //qDebug(PoseManagerLog)<<json_str;
    //cv::imshow("Imagen1", image);
    //cv::waitKey(1);  // Refrescar la imagen
    //return nullptr;


}

/**
 * @brief Inicializa la sesión, la máquina de estados y el temporizador de captura.
 * @param sesion Sesión de entrenamiento actual.
 * @param espec Especificación del ejercicio.
 * @param dual Indica si se usa modo dual cámara.
 */

void PoseManager::init(QSharedPointer<TrainingSesion> sesion, QSharedPointer<ExerciseEspec> espec, bool dual)
{
    if (!testMode)startPythonProcesses();

    //dualMode = dual;
    runningSesion = sesion;
    poseAnalyzer = QSharedPointer<StateMachine>::create(espec);
    running = true;
    runningAnalysis=false;

    if (!testMode && !connectSharedMemory()) {
        qWarning(PoseManagerLog) << "Error al conectar con la memoria compartida.";
        return;
    }
    // inicializamos el tiempo de inicia de tomas de poses
    lastTimestamp1 = QDateTime::currentMSecsSinceEpoch() - 1;
    lastTimestamp2 = QDateTime::currentMSecsSinceEpoch() - 1;

    frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &PoseManager::processNextFrame);
    frameTimer->start(33);

    //startPythonProcesses();
}
/**
 * @brief Activa el análisis de poses durante la ejecución.
 */
void PoseManager::runAnalysis()
{
    runningAnalysis=true;
}
/**
 * @brief Pausa temporalmente el análisis biomecánico de las poses.
 */
void PoseManager::pauseAnalysis()
{
    runningAnalysis=false;
}

/**
 * @brief Procesa un nuevo frame desde la(s) cámara(s), genera feedback si aplica.
 *
 * Este método es llamado periódicamente por el temporizador. Captura poses de una o ambas cámaras,
 * sincroniza vistas si es necesario, y ejecuta el análisis con la máquina de estados para generar feedback.
 */
void PoseManager::processNextFrame()
{
    if (!running) return;

    QSharedPointer<Pose> pose1(getNewPose(CAM_1, sem1, (uchar*)shm_1, view1));
    QSharedPointer<Pose> pose2 = nullptr;

    int64_t timestamp = pose1 ? pose1->getTimestamp() : QDateTime::currentMSecsSinceEpoch();

    QHash<PoseView, QHash<QString, double>> anglesByView;

    // Mostrar imagen 1
    if (pose1) {
        cv::Mat output = pose1->drawKeypoints();
        emit newImage1(output);
        qDebug(PoseManagerLog) << "Imagen1 emitida";
    }

    // Procesamiento de CAM2
    if (dualMode) {
        QSharedPointer<Pose> temPose(getNewPose(CAM_2, sem2, (uchar*)shm_2, view2));
        if (temPose) {
            // Mostrar imagen2 directamente
            cv::Mat output2 = temPose->drawKeypoints();
            emit newImage2(output2);
            qDebug(PoseManagerLog) << "Imagen2 emitida";

            // Guardar ángulos para análisis posterior
            angleQueueView2.enqueue(qMakePair(temPose->getTimestamp(), temPose->getAngles()));

            // la cola la mantendremos con un máximo de treinta sets de datos
            while (angleQueueView2.size() > 30) {
                angleQueueView2.dequeue();
            }
        }

        // Limpiar cola de ángulos vieja
        while (!angleQueueView2.isEmpty()) {
            QPair<int64_t, QHash<QString, double>> pair = angleQueueView2.head();
            int64_t time2=pair.first;
            QHash<QString, double> angles=pair.second;
            // limpiamoos la cola de aquellos datos más antiguos que los actuales de pose1
            if (time2 < timestamp - SYNC_TOLERANCE_MS) {
                angleQueueView2.dequeue();
                continue;
            }

            if (std::fabs(time2 - timestamp) <= SYNC_TOLERANCE_MS) {
                pose2 = QSharedPointer<Pose>::create(timestamp);
                anglesByView[view2] = angles;
                angleQueueView2.dequeue();
                qDebug(PoseManagerLog) << "Ángulos cam2 sincronizados con diferencia:" << std::fabs(time2 - timestamp);
                break;
            }

            break;
        }

        if (!anglesByView.contains(view2)) {
            qWarning(PoseManagerLog) << "No se encontró ángulo de cam2 suficientemente cercano a timestamp:" << timestamp;
        }
    }


    // Si pose1 no está disponible, forzar objeto vacío y contar errores
    if (!pose1) {
        pose1 = QSharedPointer<Pose>::create(timestamp);
        if (runningAnalysis && ++view1MissCount >= MAX_ALLOWED_MISSES) {
            qCritical(PoseManagerLog) << "Fallo crítico en vista principal. Captura detenida.";
            stopCapture();
            return;
        }
    } else {
        view1MissCount = 0;
    }

    if (!runningAnalysis && initFrameCount!=-1) {
        initFrameCount++;
        if (initFrameCount >= STARTING_MISSES_FRAMES && pose1) {
            runningAnalysis = true;
            initFrameCount=-1;
            qInfo(PoseManagerLog) << "Análisis activado tras periodo de espera inicial.";
        } else {
            return;
        }
    }

    //agregamos los ángulos de pose1
    if (pose1) {
        anglesByView[view1] = pose1->getAngles();
    }

    // El análisis lo ejcutaremos sólo si hay al menos unos datos válidos en alguna de las vistas
    //y si hemos notificado que estamos listos
    if (runningAnalysis && !anglesByView.isEmpty()) {
        FeedBack feedback(poseAnalyzer->run(anglesByView, timestamp));
        emit feedbackGenerated(feedback);
        qDebug(PoseManagerLog) << "FeedBack emitido";
    }

    if (poseAnalyzer->isComplete()) {
        running = false;
        frameTimer->stop();

        runningSesion->setReport(poseAnalyzer->getReport());
        runningSesion->setComplete(true);
        emit exerciseCompleted();

        if (!testMode) stopPythonProcesses();
        if (!testMode) resetMemory();
    }
}

/**
 * @brief Libera todos los recursos compartidos utilizados en la captura de poses.
 *
 * Desmapea y elimina las memorias compartidas, semáforos y archivos auxiliares como `.ready`.
 */

void PoseManager::resetMemory() {

    //stopCapture();

    cv::destroyAllWindows();
    qInfo(PoseManagerLog) << "== Reiniciando recursos compartidos (memoria y semáforos) ==";

    // Cerramos recursos si están activos
    if (shm_1) {

        munmap(shm_1, TOTAL_SIZE);
        shm_1 = nullptr;
    }
    if (sem1) {
        sem_close(sem1);
        sem1 = nullptr;
    }
    if (!CAM_1.isEmpty()) {
        shm_unlink(CAM_1.toUtf8().constData());
    }
    if (!SEM_SHM1.isEmpty()) {
        sem_unlink(SEM_SHM1.toUtf8().constData());
    }

    if (dualMode) {
        qInfo(PoseManagerLog) << "== Reiniciando recurso de segunda cámara ==";
        if (shm_2) {
            munmap(shm_2, TOTAL_SIZE);
            shm_2 = nullptr;
        }
        if (sem2) {
            sem_close(sem2);
            sem2 = nullptr;
        }
        if (!CAM_2.isEmpty()) {
            shm_unlink(CAM_2.toUtf8().constData());
        }
        if (!SEM_SHM2.isEmpty()) {
            sem_unlink(SEM_SHM2.toUtf8().constData());
        }
    }

    // Eliminar archivos .ready si existen
    QFile ready1(QCoreApplication::applicationDirPath() + "/" + CAM_1.mid(1) + ".ready");
    if (ready1.exists()) ready1.remove();

    if (dualMode) {
        QFile ready2(QCoreApplication::applicationDirPath() + "/" + CAM_2.mid(1) + ".ready");
        if (ready2.exists()) ready2.remove();
    }

    qInfo(PoseManagerLog) << "== Recursos reiniciados con éxito ==";
    //exit(0);
}

/**
 * @brief Detiene la captura de poses y marca la sesión como incompleta.
 *
 * Se utiliza cuando ocurre un error crítico o el usuario detiene la ejecución.
 */
void PoseManager::stopCapture(){
    qDebug(PoseManagerLog) << "activada la señal para parar captura";

    if (frameTimer && frameTimer->isActive()) {
        frameTimer->stop();
    }


    if (running)  {
        runningSesion->setReport(poseAnalyzer->getReport());
        runningSesion->setComplete(false);
        stopPythonProcesses();
        resetMemory();
    }
}
/**
 * @brief Método auxiliar para pruebas de conexión a memoria compartida (actualmente vacío).
 */
void PoseManager::testShareMemory(){



}
/**
 * @brief Captura y muestra la salida estándar del proceso Python de la cámara 1.
 */
void PoseManager::PythonProccesLogOutput1(){

    QByteArray output = process_cam1->readAllStandardOutput();
    qInfo(PoseManagerLog) << "PYTHON_LOG_CAM1:" << QString::fromUtf8(output);


}
/**
 * @brief Captura y muestra errores del proceso Python de la cámara 1.
 */
void PoseManager::PythonProccesErrorOutput1(){

    QByteArray output = process_cam1->readAllStandardError();
    qWarning(PoseManagerLog) << "PYTHON_ERROR_CAM1:" << QString::fromUtf8(output);

}
/**
 * @brief Captura y muestra la salida estándar del proceso Python de la cámara 2.
 */
void PoseManager::PythonProccesLogOutput2(){

    QByteArray output = process_cam2->readAllStandardOutput();
    qInfo(PoseManagerLog) << "PYTHON_LOG_CAM2:" << QString::fromUtf8(output);

}
/**
 * @brief Captura y muestra errores del proceso Python de la cámara 2.
 */
void PoseManager::PythonProccesErrorOutput2(){

     qWarning(PoseManagerLog) << "[Cam2 error]" << process_cam2->readAllStandardError();
    QByteArray output = process_cam2->readAllStandardError();
    qWarning(PoseManagerLog) << "PYTHON_ERROR_CAM2:" << QString::fromUtf8(output);
}

void PoseManager::newSerie() {
    if (!poseAnalyzer)
        return;

    qInfo(PoseManagerLog) << "== Serie interrumpida manualmente desde interfaz ==";

    // Reinicia estado y contadores para forzar una nueva serie
    poseAnalyzer->newSerie();
}

/**
 * @brief Activa el modo test, cargando las poses desde una carpeta local.
 * @param folderPath Ruta a la carpeta con imágenes y archivos JSON.
 */
void PoseManager::enableTestMode(const QString& folderPath) {
    testMode = true;
    testInputFolder = folderPath;

    QDir dir(testInputFolder);
    QStringList files = dir.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);
    testFrames.clear();

    for (const QString& file : files) {
        testFrames.append(QFileInfo(file).completeBaseName());
    }
    currentTestFrameIndex = 0;

    qInfo(PoseManagerLog) << "Modo de prueba activado con carpeta:" << folderPath;
    qInfo(PoseManagerLog) << "Se han encontrado" << testFrames.size() << "frames.";
}
