/**
 * @file imageutils.h
 * @brief Declaración de la clase ImageUtils con utilidades para comparar imágenes en formato OpenCV.
 *
 * Esta clase ofrece métodos estáticos relacionados con el análisis de imágenes mediante OpenCV,
 * permitiendo, por ejemplo, comprobar si dos imágenes son idénticas en contenido.
 */

#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <opencv2/opencv.hpp>

/**
 * @class ImageUtils
 * @brief Utilidades estáticas para la comparación de imágenes en formato OpenCV.
 *
 * Esta clase contiene métodos que permiten realizar operaciones comunes de comparación entre imágenes,
 * como verificar si dos imágenes son iguales pixel a pixel tras una posible normalización de canal de color.
 */
class ImageUtils
{
public:
    /**
     * @brief Compara dos imágenes y determina si son visualmente iguales.
     *
     * Este método compara el tamaño, tipo y contenido de las dos imágenes. Si ambas imágenes tienen
     * tres canales (formato BGR), se convierten previamente a escala de grises para una comparación neutral.
     * Luego se calcula la diferencia absoluta y se comprueba si existe algún pixel diferente.
     *
     * @param mat1 Primera imagen a comparar (cv::Mat).
     * @param mat2 Segunda imagen a comparar (cv::Mat).
     * @return true si las imágenes son idénticas visualmente; false en caso contrario.
     */
    static bool areSameImage(const cv::Mat& mat1, const cv::Mat& mat2);
};

#endif // IMAGEUTILS_H
