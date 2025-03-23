#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QDebug>
#include <nlohmann/json.hpp>

class JsonUtils {
public:
    static nlohmann::json readJsonFile(const QString& filePath);
};

#endif // JSONUTILS_H
