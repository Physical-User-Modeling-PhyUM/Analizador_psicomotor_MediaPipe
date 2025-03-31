#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include <QString>
#include <QMap>

//Esto hay que editarlo lo vemos conforme se implemetne la UI
class UserPreferences {
public:
    //explicit UserPreferences(int id, int userId, const QString &language, bool notifications, const QString &units);
    explicit UserPreferences ( const QMap<QString, QString> &data);
    int getId() const;
    int getUserId() const;
    QString getLanguage() const;
    bool getNotifications() const;
    QString getUnits() const;

private:
    int idPref;
    int idUser;
    QString language;
    bool notifications;
    QString units;
};

#endif // USERPREFERENCES_H
