/**
 * @file imageutils.cpp
 * @brief Implementación de las funciones de utilidad para comparación de imágenes definidas en ImageUtils.
 */

#include "imageutils.h"
#include <opencv2/opencv.hpp>

/**
 * @brief Compara dos imágenes para verificar si son visualmente iguales.
 *
 * Este método realiza una comparación pixel a pixel entre las dos imágenes.
 * - Verifica primero que ambas imágenes tengan el mismo tamaño y tipo.
 * - Si ambas son imágenes en color (3 canales), se convierten a escala de grises para asegurar una comparación neutral.
 * - Se calcula la diferencia absoluta entre las imágenes y se cuenta el número de píxeles diferentes.
 *
 * @param mat1 Primera imagen.
 * @param mat2 Segunda imagen.
 * @return true si no hay diferencias entre las imágenes; false en caso contrario.
 */
bool ImageUtils::areSameImage(const cv::Mat &mat1, const cv::Mat &mat2)
{
    // Verificamos si tienen el mismo tamaño y tipo
    if (mat1.size() != mat2.size() || mat1.type() != mat2.type()) {
        return false;
    }

    cv::Mat img1 = mat1;
    cv::Mat img2 = mat2;

    // Convertimos ambas imágenes a un mismo formato si es necesario
    if (img1.channels() == 3 && img2.channels() == 3) {
        // Si ambas imágenes son BGR, las convertimos a un espacio de color neutral (grises)
        cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
        cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
    }

    cv::Mat diff;
    // Calculamos la diferencia absoluta
    cv::absdiff(img1, img2, diff);

    // Si no hay píxeles diferentes, son iguales
    return cv::countNonZero(diff) == 0;
}
