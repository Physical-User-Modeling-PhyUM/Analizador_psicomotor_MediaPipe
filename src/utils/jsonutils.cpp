#include "jsonutils.h"

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
