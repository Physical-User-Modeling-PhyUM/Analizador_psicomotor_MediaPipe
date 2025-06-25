/**
 * @file pose.h
 * @brief Define la clase Pose para representar y procesar datos de una pose humana obtenida mediante visión artificial.
 */

#ifndef POSE_H
#define POSE_H

#include <QMap>
#include <QPointF>
#include <QVector>
#include <QPair>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(PoseLog);

/**
 * @class Pose
 * @brief Representa una pose humana a partir de datos de keypoints y una imagen.
 *
 * Esta clase permite cargar datos desde un JSON (formato MediaPipe u OpenPose), gestionar conexiones
 * entre keypoints, calcular ángulos y distancias, y visualizar la pose sobre la imagen original.
 */
class Pose {
public:
    /**
     * @brief Constructor que inicializa la pose desde datos JSON y una imagen.
     * @param jsonData Objeto JSON con keypoints y timestamp.
     * @param image Imagen en formato OpenCV.
     * @param connections Mapa de conexiones entre keypoints.
     */
    explicit Pose(const nlohmann::json &jsonData, cv::Mat image, QHash<QPair<int, int>, QString>& connections);

    /**
     * @brief Constructor para crear una pose vacía con timestamp definido.
     * @param timestamp Marca de tiempo asociada a la pose.
     */
    explicit Pose(int64_t timestamp);

    /**
     * @brief Devuelve el timestamp asociado a la pose.
     * @return Valor de timestamp.
     */
    int64_t getTimestamp() const;

    /**
     * @brief Devuelve todos los keypoints almacenados.
     * @return Mapa de keypoints indexados por ID.
     */
    QMap<int, QPointF> getKeypoints() const;

    /**
     * @brief Verifica si dos keypoints están conectados directamente.
     * @param keypoint1 ID del primer keypoint.
     * @param keypoint2 ID del segundo keypoint.
     * @return true si están conectados, false en caso contrario.
     */
    bool areConnected(int keypoint1, int keypoint2) const;

    /**
     * @brief Calcula la distancia en píxeles entre dos keypoints.
     * @param keypoint1 ID del primer keypoint.
     * @param keypoint2 ID del segundo keypoint.
     * @return Distancia euclidiana en píxeles.
     */
    double getPixelDistance(int keypoint1, int keypoint2) const;

    /**
     * @brief Calcula el ángulo del vector entre dos keypoints respecto a la vertical.
     * @param keypoint1 ID del primer keypoint.
     * @param keypoint2 ID del segundo keypoint.
     * @return Ángulo en grados (0-360), respecto al eje Y negativo.
     */
    double getAngle(int keypoint1, int keypoint2) const;

    /**
     * @brief Devuelve todas las conexiones disponibles entre keypoints.
     * @return Lista de pares de índices conectados.
     */
    QVector<QPair<int, int>> getConnections() const;

    /**
     * @brief Dibuja los keypoints y sus conexiones sobre la imagen original.
     * @return Imagen OpenCV con keypoints y líneas dibujadas.
     */
    cv::Mat drawKeypoints();

    /**
     * @brief Devuelve la imagen original (en formato BGR).
     * @return Imagen en formato cv::Mat.
     */
    cv::Mat getImage_bgr() const;

    /**
     * @brief Calcula el ángulo de cada línea definida en connections.
     * @return Mapa de nombre de línea a ángulo en grados.
     */
    QHash<QString, double> getAngles() const;

private:
    int64_t timestamp;                                     ///< Marca de tiempo asociada a la pose.
    cv::Mat image_bgr;                                     ///< Imagen de la cual se extrajo la pose.
    QMap<int, QPointF> keypoints;                          ///< Mapa de ID de keypoint a posición (normalizada y escalada).
    QHash<QPair<int, int>, QString> connections;           ///< Conexiones nombradas entre pares de keypoints.
};

#endif // POSE_H
