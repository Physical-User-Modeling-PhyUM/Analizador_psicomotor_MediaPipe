#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "dbtable.h"

#include <sqlite3.h>

class DBManager : public QObject {
    Q_OBJECT
public:
    explicit DBManager(const QString& dbPath, QObject* parent = nullptr);
    ~DBManager();

    bool executeQuery(const QString& query);
    DBTable getTable(const QString& query);
    bool initializeSchema();

private:
    sqlite3* db;
    bool openDatabase(const QString& dbPath);
    void closeDatabase();
};

#endif // DBMANAGER_H
