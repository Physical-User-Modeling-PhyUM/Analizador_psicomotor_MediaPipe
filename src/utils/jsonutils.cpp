/**
 * @file jsonutils.cpp
 * @brief Implementación de los métodos de la clase JsonUtils para leer archivos JSON.
 */

#include "jsonutils.h"

/**
 * @brief Carga y parsea un archivo JSON desde la ruta especificada.
 *
 * Este método:
 * - Abre el archivo en modo lectura de texto.
 * - Si no se puede abrir, emite una advertencia mediante qWarning.
 * - Intenta parsear el contenido con `nlohmann::json::parse`.
 * - Si ocurre una excepción durante el parseo (archivo mal formado), se captura y se reporta el error.
 *
 * @param filePath Ruta al archivo JSON a leer.
 * @return Objeto JSON válido si la lectura y el parseo fueron exitosos; JSON vacío en caso contrario.
 */
nlohmann::json JsonUtils::readJsonFile(const QString& filePath) {

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Error: No se pudo abrir el archivo JSON:" << filePath;
        return {};
    }

    QByteArray jsonData = file.readAll();
    file.close();

    try {
        return nlohmann::json::parse(jsonData.constData());
    } catch (const std::exception& e) {
        qWarning() << "Error al parsear el JSON:" << e.what();
        return {};  // Devuelve un JSON vacío en caso de error
    }
}
