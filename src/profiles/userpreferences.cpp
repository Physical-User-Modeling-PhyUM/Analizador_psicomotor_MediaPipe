#include "userpreferences.h"


UserPreferences::UserPreferences(const QMap<QString, QString> &data)
{
    idPref = data.contains("idPref") ? data["idPref"].toInt() : 0;
    idUser = data.contains("idUser") ? data["idUser"].toInt() : 0;
    language = data.contains("language") ? data["language"] : "English";
    notifications = data.contains("notifications") ? (data["notifications"] == "true") : false;
    units = data.contains("units") ? data["units"] : "Metric";
}

int UserPreferences::getId() const
{
    return 0;
}
