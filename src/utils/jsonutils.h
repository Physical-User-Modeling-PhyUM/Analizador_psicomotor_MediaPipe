/**
 * @file jsonutils.h
 * @brief Declaración de la clase JsonUtils que proporciona funciones para la lectura de archivos JSON utilizando la biblioteca nlohmann::json.
 *
 * Esta clase contiene métodos estáticos que simplifican la carga de archivos JSON desde disco,
 * integrando funcionalidades de Qt (`QFile`, `QString`) con el parser `nlohmann::json` de C++ moderno.
 */

#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QDebug>
#include <nlohmann/json.hpp>

/**
 * @class JsonUtils
 * @brief Clase de utilidades estáticas para la manipulación de archivos JSON usando la biblioteca nlohmann::json.
 *
 * Esta clase ofrece una interfaz sencilla para leer archivos JSON en disco, devolviendo estructuras de tipo `nlohmann::json`
 * completamente parseadas. Es especialmente útil cuando se combina Qt para la gestión de archivos con procesamiento JSON avanzado en C++.
 */
class JsonUtils {
public:
    /**
     * @brief Lee y parsea un archivo JSON ubicado en el sistema de archivos.
     *
     * Este método abre el archivo en modo solo lectura, intenta parsearlo usando la biblioteca `nlohmann::json`,
     * y maneja errores de apertura o de formato JSON mediante logs de advertencia.
     *
     * @param filePath Ruta completa al archivo JSON.
     * @return Objeto nlohmann::json con los datos cargados, o vacío en caso de error.
     */
    static nlohmann::json readJsonFile(const QString& filePath);
};

#endif // JSONUTILS_H
