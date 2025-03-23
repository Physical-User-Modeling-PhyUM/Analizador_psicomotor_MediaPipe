#include "posemanager.h"


// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(PoseManagerLog, "posemanager")

PoseManager::PoseManager(StateMachine& poseAnalyzer,QWidget *parent,bool dualMode, bool test)
    : QMainWindow(parent),poseAnalyzer(poseAnalyzer), dualMode(dualMode) {
    try {

            loadConfig();


            startPythonProcesses();
            qDebug(PoseManagerLog) << "Constructor PoseManager: dualMode= " << dualMode;
            //init();


    }catch(const std::exception &e) {
       qCritical(PoseManagerLog) << "Error fatal:" << e.what();
    }
}

PoseManager::~PoseManager() {
    stopPythonProcesses();
    releaseMemory();

}

void PoseManager::loadConfig() {

    QString configPath = QCoreApplication::applicationDirPath() + "/config/poseConfig.json";

    qDebug(PoseManagerLog) << "Buscando config.json en:" << configPath;

    std::ifstream file(configPath.toStdString());
    if (!file.is_open()) {
        qCritical(PoseManagerLog) << "No se pudo abrir config.json";
        return;
    }
    nlohmann::json config;

    file >> config;
    file.close();

    // Verificamos que las claves esenciales existen
    std::vector<std::string> required_keys = {"WIDTH", "HEIGHT", "JSON_SIZE","FLAG_SIZE","PYTHON_SCRIPT", "CAM1", "CAM2","SEM_SHM1","SEM_SHM2"};

    for (const auto& key : required_keys) {
        if (!config.contains(key)) {

            qCritical(PoseManagerLog) << "Falta la clave en poseConfig.json:" << key;
            exit(1);
        }
    }

    WIDTH = config["WIDTH"];
    HEIGHT = config["HEIGHT"];
    FRAME_SIZE =WIDTH*HEIGHT*3;
    JSON_SIZE = config["JSON_SIZE"];
    FLAG_SIZE = config["FLAG_SIZE"];
    TOTAL_SIZE = FRAME_SIZE+JSON_SIZE;
    CAM_1 = QString::fromStdString(config["CAM1"].get<std::string>());
    CAM_2 = QString::fromStdString(config["CAM2"].get<std::string>());
    pythonScript = QString::fromStdString(config["PYTHON_SCRIPT"]);
    SEM_SHM1=QString::fromStdString(config["SEM_SHM1"]);
    SEM_SHM2=QString::fromStdString(config["SEM_SHM2"]);
    // Imprimir configuración cargada
    qDebug(PoseManagerLog) << "=== Configuración Cargada ===";
    qDebug(PoseManagerLog) << "CAM_1: " << CAM_1.toStdString() ;
    qDebug(PoseManagerLog) << "CAM_2: " << CAM_2.toStdString() ;
    qDebug(PoseManagerLog)<< "WIDTH: " << WIDTH  ;
    qDebug(PoseManagerLog) << "HEIGHT: " << HEIGHT ;
    qDebug(PoseManagerLog) << "FRAME_SIZE: " << WIDTH*HEIGHT*3 ;
    qDebug(PoseManagerLog) << "MAX_JSON_SIZE: " << JSON_SIZE ;
    qDebug(PoseManagerLog) << "MAX_TOTAL_SIZE: " << TOTAL_SIZE;
    qDebug(PoseManagerLog) << "PYTHON_SCRIPT: " << pythonScript.toStdString() ;
    qDebug(PoseManagerLog) << "SEM_SHM1: " << SEM_SHM1.toStdString() ;
    qDebug(PoseManagerLog) << "SEM_SHM2: " << SEM_SHM2.toStdString() ;
    qDebug(PoseManagerLog) << "=============================" ;

    // if (config.contains("connections")) {
    //     for (auto &conn : config["connections"]) {
    //         connections.append(qMakePair(conn[0].get<int>(), conn[1].get<int>()));
    //     }
    // }
    if (config.contains("CONNECTIONS")) {
        for (auto &conn : config["CONNECTIONS"]) {
            if (!conn.is_array() || conn.size() != 2) {
                qCritical() << "Error en el formato de 'CONNECTIONS' en el JSON";
                continue;
            }
            connections.append(qMakePair(conn[0].get<int>(), conn[1].get<int>()));
        }
    }

}

void PoseManager::startPythonProcesses() {


    qDebug(PoseManagerLog) <<"Iniciando procesos de Python...";
    QString scriptPath = QCoreApplication::applicationDirPath() + "/" + pythonScript;

    // Comprobar si el script de Python existe antes de ejecutarlo
    if (!QFile::exists(scriptPath)) {
       qCritical(PoseManagerLog) <<"Error: No se encontró el script de Python en " << scriptPath.toStdString() ;
        shm_unlink(CAM_1.toUtf8().constData());
        return;
    }

    qDebug(PoseManagerLog) << "Ejecutando script: " << scriptPath.toStdString();

    process_cam1 = new QProcess(this);
    process_cam1->start("python3", QStringList() << pythonScript << "0");
    //conectamos la salida de la aplicacion de ppython para ver sus logs
    connect(process_cam1, &QProcess::readyReadStandardOutput, this, &PoseManager::PythonProccesLogOutput);
    connect(process_cam1, &QProcess::readyReadStandardError, this, &PoseManager::PythonProccesErrorOutput);

    if (dualMode) {
        process_cam2 = new QProcess(this);
        process_cam2->start("python3", QStringList() << pythonScript << "1");
        connect(process_cam2, &QProcess::readyReadStandardOutput, this, &PoseManager::PythonProccesLogOutput);
        connect(process_cam2, &QProcess::readyReadStandardError, this, &PoseManager::PythonProccesErrorOutput);

    }
}

bool PoseManager::connectSharedMemory() {


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
        // Intentar abrir el semáforo
        sem2 = sem_open(SEM_SHM2.toUtf8().constData(), 0);
        if (sem2 == SEM_FAILED) {
            qCritical(PoseManagerLog) << "PoseManager (C++)> Error al abrir el semáforo. Asegúrate de que el capturador2 está corriendo.";
            return false;
        }

        // Intentar abrir la memoria compartida
        shm_fd2 = shm_open(CAM_1.toUtf8().constData(), O_RDWR, 0666);

        while (shm_fd2 == -1) {
            shm_fd2 = shm_open(CAM_1.toUtf8().constData(), O_RDWR, 0666);
            qCritical(PoseManagerLog) << "> No se encontró la memoria compartida. Esperando al Capturador...";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Mapear la memoria compartida en el espacio de direcciones
        //shm_2 = (unsigned char*) mmap(NULL, TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
        if (shm_2 == MAP_FAILED) {
            qCritical(PoseManagerLog) << "> Error al mapear la memoria compartida "<<CAM_2;
            return false;
        }

    }
     qDebug(PoseManagerLog) << "PoseManager (C++)> Conectado a la memoria compartida ...fd= "<<shm_fd1;
    return true;
}

void PoseManager::stopPythonProcesses() {

    process_cam1->terminate();
    if (dualMode) process_cam2->terminate();
   qDebug(PoseManagerLog) << "=== Capturador parado ===";
}

Pose *PoseManager::getNewPose(QString cam_name,sem_t* sem,unsigned char *shm, int64_t &lastTimestamp) {

    // leemos si la memoria está libre


    qDebug(PoseManagerLog) << "Verificando memoria en: " << cam_name;

    sem_wait(sem);

    // Leer la imagen
    cv::Mat image(HEIGHT, WIDTH, CV_8UC3, shm);


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
        return nullptr;
    }

    // Actualizar el timestamp y crear la nueva Pose
    lastTimestamp = timestamp;

    //generamos la Pose
    return  new Pose(json_data,image,connections);
    //qDebug(PoseManagerLog)<<json_str;
    //cv::imshow("Imagen1", image);
    //cv::waitKey(1);  // Refrescar la imagen
    //return nullptr;


}


void PoseManager::init(TrainingSesion sesion) {

    Pose* pose1=nullptr;
    Pose* pose2=nullptr;
    running=true;

    //Revisamos la memoria compartida
    if (connectSharedMemory()){

        // iniciamos la captura de imagenes
        int64_t lastTimestamp= QDateTime::currentMSecsSinceEpoch()-1;

        // Usamos un QTimer para ejecutar la verificación de poses periódicamente
        // timer = new QTimer(this);
        // connect(timer, &QTimer::timeout, this, &PoseManager::stopCapture);
        // timer->start(1000);

        while (running){

            pose1 = getNewPose(CAM_1,sem1, (uchar*)shm_1, lastTimestamp);
            pose2 = dualMode ? getNewPose(CAM_2,sem2, (uchar*)shm_2, lastTimestamp) : nullptr;

            if (pose1 == nullptr && pose2 == nullptr) {
                qWarning(PoseManagerLog) << "No existen nuevas poses";
                QThread::msleep(100);
                continue;
            }

            // Procesar la pose si existe (es lo más recoendable?)
            //FeedBack feedback = poseAnalyzer.analyze(pose1, pose2);


            QHash<QString, double> angles;
            // Dibujar keypoints y emitir la imagen modificada
            if (pose1!=nullptr){
                angles=pose1->getAngles();
                pose1->drawKeypoints();
                emit newImage1(pose1->getImage_bgr());
            }
            if (pose2!=nullptr){
                for (auto it = pose2->getAngles().constBegin(); it != pose2->getAngles().constEnd(); ++it) {
                        angles.insert(it.key()+"_L", it.value());
                }
                pose2->drawKeypoints();
                emit newImage2(pose2->getImage_bgr());

            }
            FeedBack feedback =FeedBack(poseAnalyzer.run(angles,lastTimestamp));
            emit feedbackGenerated(feedback);

            if(poseAnalyzer.isComplete()){
                running=false;
                sesion.setReport(poseAnalyzer.getReport());

            }
            // Dormimos el bucle para evitar consumo excesivo de CPU
            //QCoreApplication::processEvents();
            //QThread::msleep(100);
        }

    }

    // Liberar memoria de las poses
    delete pose1;
    delete pose2;
    pose1 = nullptr;
    pose2 = nullptr;
    //stopPythonProcesses();
    releaseMemory();
}

void PoseManager::releaseMemory() {
    //liberamos la memoria compartida
    if (shm_1) {
        munmap(shm_1, TOTAL_SIZE);
        //close(shm_fd1);
        shm_1=nullptr;
        sem_close(sem1);
    }
    if (dualMode && shm_2) {
        munmap(shm_2, TOTAL_SIZE);
        sem_close(sem2);
        shm_2=nullptr;
    }
    qDebug(PoseManagerLog) << "Liberando memoria compartida al salir..."<<CAM_1.toUtf8().constData()<<CAM_2.toUtf8().constData();
    cv::destroyAllWindows();
    exit(0);
}


void PoseManager::stopCapture(){
    qDebug(PoseManagerLog) << "activada la señal para parar captura";
    //running =false;


}
void PoseManager::testShareMemory(){



}

void PoseManager::PythonProccesLogOutput(){
  qDebug(PoseManagerLog) << "Python output: " << process_cam1->readAllStandardOutput();



}
void PoseManager::PythonProccesErrorOutput(){

    qDebug(PoseManagerLog) << "Python error: " << process_cam1->readAllStandardOutput();


}
