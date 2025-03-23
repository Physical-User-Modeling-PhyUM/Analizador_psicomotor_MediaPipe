#include "pose.h"
#include <cmath>
#include <iostream>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(PoseLog, "pose")

Pose::Pose(const nlohmann::json &jsonData, cv::Mat image,QVector<QPair<int, int>> connections) {
    if (jsonData.contains("timestamp")) {
        timestamp = jsonData["timestamp"].get<int64_t>();
    } else {
        timestamp = 0; // Valor por defecto
        qDebug(PoseLog) << "Advertencia: 'timestamp' no encontrado en el JSON";
    }


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
            keypoints[key] = QPointF(it.value()["x"].get<double>(), it.value()["y"].get<double>());
        }

    }
    this->connections=connections;

    this->image_bgr=image;
}

int64_t Pose::getTimestamp() const {
    return timestamp;
}

QMap<int, QPointF> Pose::getKeypoints() const {
    return keypoints;
}

bool Pose::areConnected(int keypoint1, int keypoint2) const {
    return connections.contains(qMakePair(keypoint1, keypoint2)) ||
           connections.contains(qMakePair(keypoint2, keypoint1));
}
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




QVector<QPair<int, int> > Pose::getConnections() const
{
    return connections;
}

cv::Mat Pose::getImage_bgr() const
{
    return image_bgr;
}



cv::Mat Pose::drawKeypoints(){


    if (image_bgr.empty()) {
        qCritical(PoseLog)<< "Error: Imagen vacía, no se pueden dibujar keypoints.";
        return image_bgr;
    }

    // Dibujar keypoints individuales
    for (auto it = keypoints.begin(); it != keypoints.end(); ++it) {

        cv::Point point(it.value().x(), it.value().y());
        cv::circle(image_bgr, point, 5, cv::Scalar(0, 255, 0), -1);
    }

    // Dibujar conexiones entre keypoints
    for (const auto &connection : connections) {
        int kp1 = connection.first;
        int kp2 = connection.second;

        if (keypoints.contains(kp1) && keypoints.contains(kp2)) {
            //Obtenemos los puntos
            int x1=keypoints[kp1].x();
            int y1=keypoints[kp1].y();
            int x2=keypoints[kp2].x();
            int y2=keypoints[kp2].y();
            //Creamos los puntos y dibujamos la línea entre ellos con OpenCV
            cv::Point pt1(x1,y1);
            cv::Point pt2(x2, y2);
            cv::line(image_bgr, pt1, pt2, cv::Scalar(255, 0, 0), 2);
        }
    }
    //creamos la imagen con los
    return image_bgr;
    //QImage qimg(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
}

QHash<QString, double> Pose::getAngles() const{

    QHash<QString, double> angles;

    for (const auto &connection : connections) {
        int keypoint1 = connection.first;
        int keypoint2 = connection.second;
        QString line = QString("%1_%2").arg(keypoint1).arg(keypoint2);

        if (!keypoints.contains(keypoint1) || !keypoints.contains(keypoint2)) {
            angles[line] = -1;  // Si falta un keypoint, asignamos -1
            qCWarning(PoseLog) << "Keypoint faltante para línea:" << line;
            continue;
        }

        // Calculamos el ángulo de la línea
        angles[line] = getAngle(keypoint1, keypoint2);
    }

    return angles;
}
