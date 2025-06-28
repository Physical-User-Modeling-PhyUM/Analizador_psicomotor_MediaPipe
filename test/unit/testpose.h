/**
 * @file testpose.h
 * @brief Declaración de la clase TestPose para la validación funcional de la clase Pose.
 *
 * Esta clase implementa un conjunto de pruebas funcionales sobre la clase `Pose`, que incluyen
 * validaciones de inicialización, interpretación de keypoints y conexiones, cálculo de ángulos
 * y visualización gráfica de poses sobre imágenes.
 */

#ifndef TESTPOSE_H
#define TESTPOSE_H

#include <iostream>
#include "../../src/pose/pose.h"
#include <QCoreApplication>
#include <QFile>
#include "utils/jsonutils.h"
#include "utils/imageutils.h"

/**
 * @class TestPose
 * @brief Clase de pruebas funcionales para la clase Pose.
 *
 * Esta clase permite verificar la correcta funcionalidad de la clase `Pose`, evaluando la lectura
 * de datos desde JSON, la gestión de keypoints y conexiones, el cálculo de métricas geométricas
 * como ángulos y distancias, así como la representación gráfica sobre imágenes reales.
 */
class TestPose {

public:
    /**
     * @brief Ejecuta todas las pruebas definidas sobre la clase Pose.
     *
     * Carga los archivos necesarios (JSON de keypoints, conexiones, imágenes) y realiza
     * las validaciones secuenciales de comportamiento de la clase `Pose`.
     */
    void runTests();

    nlohmann::json keypoints1;  ///< Keypoints en formato JSON para la primera imagen.
    nlohmann::json keypoints2;  ///< Keypoints en formato JSON para la segunda imagen.
    nlohmann::json JsonConnections; ///< JSON que define las conexiones entre keypoints.
    QHash<QPair<int, int>, QString> connections; ///< Conexiones entre keypoints como pares de índices.
    QHash<int, QString> keypoints; ///< Identificadores de keypoints y sus nombres.
    cv::Mat image1; ///< Imagen asociada a los keypoints1.
    cv::Mat image2; ///< Imagen asociada a los keypoints2.

private:
    /**
     * @brief Verifica que la inicialización de la clase Pose sea correcta.
     *
     * Se asegura de que el objeto `Pose` interprete correctamente el timestamp, los keypoints,
     * las conexiones y la imagen original.
     */
    void testPoseInitialization();

    /**
     * @brief Comprueba la consistencia de los keypoints y sus conexiones.
     *
     * Evalúa si los pares de keypoints están correctamente enlazados según las conexiones definidas.
     */
    void testKeypointsAndConnections();

    /**
     * @brief Valida el cálculo de ángulos entre pares de keypoints.
     *
     * Ejecuta comparaciones entre valores esperados y los calculados por la clase `Pose`.
     */
    void testAngles();

    /**
     * @brief Prueba el cálculo de distancias entre keypoints (pendiente de implementación).
     */
    void testDistance();

    /**
     * @brief Verifica que los keypoints y sus conexiones se dibujan correctamente sobre una imagen.
     *
     * Superpone los puntos y líneas de conexión sobre la imagen original para revisión visual.
     */
    void testDrawKeyPoints();
};

#endif // TESTPOSE_H
