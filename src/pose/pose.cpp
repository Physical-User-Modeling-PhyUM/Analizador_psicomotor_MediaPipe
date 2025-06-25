/*!
 * \file pose.cpp
 * \brief Implementación de la clase Pose para representar una postura humana a partir de keypoints.
 *
 * Esta clase permite construir una instancia de postura (`Pose`) a partir de datos JSON generados por modelos de visión por computadora como MediaPipe u OpenPose.
 * Proporciona utilidades para extraer ángulos, distancias, y dibujar sobre imágenes.
 */
#include "pose.h"
#include <cmath>
#include <iostream>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(PoseLog, "pose")
/*!
 * \brief Constructor de la clase Pose a partir de datos JSON, una imagen y un conjunto de conexiones entre keypoints.
 * \param jsonData Objeto JSON que contiene la información de la postura (timestamp y keypoints).
 * \param image Imagen OpenCV en formato BGR sobre la que se puede dibujar.
 * \param connections Conjunto de pares de keypoints que representan conexiones anatómicas (por ejemplo, hombro-codo).
 */
Pose::Pose(const nlohmann::json &jsonData, cv::Mat image, QHash<QPair<int, int>, QString>& connections) {
    if (jsonData.contains("timestamp")) {
        timestamp = jsonData["timestamp"].get<int64_t>();
    } else {
        timestamp = 0; // Valor por defecto
        qDebug(PoseLog) << "Advertencia: 'timestamp' no encontrado en el JSON";
    }
    int width = image.cols > 0 ? image.cols : 1;
    int height = image.rows > 0 ? image.rows : 1;

    // obtenemos los datos de Keypoints y conexiones entre ellos
    if (jsonData.contains("keypoints")) {

        // for (auto it = jsonData["keypoints"].begin(); it != jsonData["keypoints"].end(); ++it) {
        //     int key = std::stoi(it.key());
        //     keypoints[key] = QPointF(it.value()[0], it.value()[1]);
        // }
        for (auto it = jsonData["keypoints"].begin(); it != jsonData["keypoints"].end(); ++it) {
            std::string key_str = it.key();

            // Verifica que el keypoint tiene las claves "x" y "y", se salta los que no
            if (!it.value().contains("x") || !it.value().contains("y")) {
                qCritical() << "Error: Keypoint sin valores 'x' o 'y' en JSON: " << QString::fromStdString(it.value().dump());
                continue;
            }

            int key = std::stoi(key_str);
            double x=it.value()["x"].get<double>();
            double y= it.value()["y"].get<double>();

            // if (x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0) {
            //     qWarning(PoseLog) << "Keypoint fuera de rango normalizado: (" << x << "," << y << ")";
            //     continue;
            // }

            keypoints[key] = QPointF(x*width,y*height);
        }

    }
    this->connections=connections;

    this->image_bgr=image;
}
/*!
 * \brief Constructor alternativo que permite inicializar solo con un timestamp.
 * \param timestamp Marca de tiempo de la postura.
 */
Pose::Pose(int64_t timestamp):timestamp(timestamp){

    cv::Mat image_bgr=cv::Mat();
    QMap<int, QPointF> keypoints= QMap<int, QPointF> ();
    QHash<QPair<int, int>, QString>connections=QHash<QPair<int, int>, QString>();

}

/*!
 * \brief Obtiene el timestamp asociado a esta postura.
 * \return Timestamp como int64_t.
 */
int64_t Pose::getTimestamp() const {
    return timestamp;
}
/*!
 * \brief Devuelve el conjunto de keypoints registrados para esta postura.
 * \return Un QMap con los índices de keypoints como claves y sus posiciones en la imagen como QPointF.
 */
QMap<int, QPointF> Pose::getKeypoints() const {
    return keypoints;
}
/*!
 * \brief Verifica si dos keypoints están conectados directamente según el mapa de conexiones.
 * \param keypoint1 Índice del primer keypoint.
 * \param keypoint2 Índice del segundo keypoint.
 * \return true si están conectados, false en caso contrario.
 */
bool Pose::areConnected(int keypoint1, int keypoint2) const {
    return connections.contains(qMakePair(keypoint1, keypoint2)) ||
           connections.contains(qMakePair(keypoint2, keypoint1));
}
/*!
 * \brief Calcula la distancia euclidiana en píxeles entre dos keypoints.
 * \param keypoint1 Índice del primer keypoint.
 * \param keypoint2 Índice del segundo keypoint.
 * \return Distancia en píxeles entre los dos puntos.
 * \throws std::runtime_error si alguno de los keypoints no existe.
 */
double Pose::getPixelDistance(int keypoint1, int keypoint2) const{
    if (!keypoints.contains(keypoint1) || !keypoints.contains(keypoint2)) {
        qCritical(PoseLog) << "No existen los Keypoints para calcular el ángulo";
        throw std::runtime_error("Error: no existen los Keypoints para calcular el ángulo");
    }

    QPointF p1 = keypoints[keypoint1];
    QPointF p2 = keypoints[keypoint2];

    double difX = p2.x() - p1.x();
    double difY = p2.y() - p1.y();

    return std::sqrt(difX * difX + difY * difY);
}

/*!
 * \brief Calcula el ángulo de orientación de la línea entre dos keypoints respecto a la vertical superior (eje Y negativo).
 *
 * El ángulo se mide en grados, en sentido horario:
 * - 0° si el segundo punto está directamente sobre el primero.
 * - 90° si está a la derecha.
 * - 180° si está debajo.
 * - 270° si está a la izquierda.
 *
 * \param keypoint1 Índice del primer keypoint (inicio del segmento).
 * \param keypoint2 Índice del segundo keypoint (fin del segmento).
 * \return Ángulo en grados normalizado en el rango [0, 360).
 * \throws std::runtime_error si los keypoints no existen.
 */

double Pose::getAngle(int keypoint1, int keypoint2) const {
    if (!keypoints.contains(keypoint1) || !keypoints.contains(keypoint2)) {
        qCritical(PoseLog) << "No existen los Keypoints para calcular el ángulo";
        throw std::runtime_error("Error: no existen los Keypoints para calcular el ángulo");
    }

    QPointF p1 = keypoints[keypoint1];
    QPointF p2 = keypoints[keypoint2];

    double difX = p2.x() - p1.x();
    double difY = p2.y() - p1.y();

    // La referencia del cero es arriba. Usamos una referencia matemática
    double angle = std::atan2(difX, -difY) * 180.0 / M_PI;

    // Normalizarmos  el ángulo a [0, 360°]
    if (angle < 0) {
        angle += 360.0;
    }

    return angle;
}


/*!
 * \brief Devuelve la lista de conexiones entre keypoints como pares de índices.
 * \return QVector de pares de enteros representando conexiones.
 */

QVector<QPair<int, int> > Pose::getConnections() const
{
    return connections.keys();
}
/*!
 * \brief Devuelve la imagen en formato BGR asociada a esta postura.
 * \return Imagen original en formato cv::Mat.
 */
cv::Mat Pose::getImage_bgr() const
{
    return image_bgr;
}


/*!
 * \brief Dibuja los keypoints y las conexiones sobre una copia de la imagen original.
 *
 * Cada keypoint se dibuja como un círculo verde.
 * Las conexiones válidas se dibujan como líneas azules entre los puntos correspondientes.
 * El método valida los límites de cada punto para evitar errores de acceso fuera de imagen.
 *
 * \return Imagen con anotaciones en formato cv::Mat.
 */
cv::Mat Pose::drawKeypoints() {
    if (image_bgr.empty()) {
        qCritical(PoseLog) << "Error: Imagen vacía, no se pueden dibujar keypoints.";
        return image_bgr;
    }

    // Clonamos la imagen para evitar dibujar encima en llamadas sucesivas y saturar al sistema
    cv::Mat output = image_bgr.clone();
     qDebug(PoseLog) << "Image size: " << output.cols << "x" << output.rows;
    // Dibujar keypoints
    for (auto it = keypoints.begin(); it != keypoints.end(); ++it) {
        const QPointF& pt = it.value();
        if (pt.x() >= 0 && pt.y() >= 0 && pt.x() < output.cols && pt.y() < output.rows) {
            cv::circle(output, cv::Point(static_cast<int>(pt.x()), static_cast<int>(pt.y())), 5, cv::Scalar(0, 255, 0), -1);
        } else {
            qWarning(PoseLog) << "Keypoint fuera de rango: " << pt;
        }
    }

    // Dibujamos las conexiones entre los keypoints
    for (const auto& connection : connections.keys()) {
        int kp1 = connection.first;
        int kp2 = connection.second;

        if (keypoints.contains(kp1) && keypoints.contains(kp2)) {
            const QPointF& p1 = keypoints[kp1];
            const QPointF& p2 = keypoints[kp2];

            if (p1.x() >= 0 && p1.y() >= 0 && p2.x() >= 0 && p2.y() >= 0 &&
                p1.x() < output.cols && p1.y() < output.rows &&
                p2.x() < output.cols && p2.y() < output.rows) {
                cv::line(output,
                         cv::Point(static_cast<int>(p1.x()), static_cast<int>(p1.y())),
                         cv::Point(static_cast<int>(p2.x()), static_cast<int>(p2.y())),
                         cv::Scalar(255, 0, 0), 2);
            } else {
                qWarning(PoseLog) << "Conexión ignorada: keypoints fuera de rango [" << kp1 << "," << kp2 << "]"
                                  <<connections.value(QPair(kp1,kp2));
            }
        }
    }

    return output;
}



/*!
 * \brief Calcula los ángulos de todas las conexiones definidas en esta postura.
 *
 * Cada conexión se representa con un identificador (nombre anatómico o cadena), y se calcula el ángulo entre los dos keypoints conectados.
 *
 * \return QHash con los identificadores de conexión como clave y el ángulo en grados como valor.
 */

QHash<QString, double> Pose::getAngles() const {
    QHash<QString, double> angles;

    for (const auto& connection : connections.keys()) {
        int k1 = connection.first;
        int k2 = connection.second;


        if (!keypoints.contains(k1) || !keypoints.contains(k2)) {
            qWarning(PoseLog) << "getAngles: Keypoint faltante para línea: " << QString("%1_%2").arg(k1).arg(k2);
            continue;
        }
        double angleDeg=getAngle(k1,k2);
        QString key=connections.value(QPair(k1,k2));
        //QString key = QString("%1_%2").arg(k1).arg(k2);
        angles.insert(key, angleDeg);
    }

    return angles;
}
