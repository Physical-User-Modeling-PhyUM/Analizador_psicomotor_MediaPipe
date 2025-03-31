#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "db/dbtable.h"
#include "enums/TypeBDEnum.h"

#include <QSqlDatabase>

class DBManager : public QObject {
    Q_OBJECT

public:
    explicit DBManager(const QString& dbPath, QObject* parent = nullptr);
    ~DBManager();

    bool openDatabase(const QString& dbPath);
    void closeDatabase();


    DBTable get(TypeBDEnum tipo, DBFilter filtro=DBFilter::none,  QString valorFiltro="");
    bool save(TypeBDEnum tipo, DBTable& elem, DBFilter filtro=DBFilter::none,  QString valorFiltro="");
    bool deleteRow(TypeBDEnum tipo, DBFilter filtro,  QString& valorFiltro);
    bool initializeSchema();


private:
    QSqlDatabase qdb;
    QStringList getTableColumns(const QString& tableName);
    bool isValidFilterForTable(const QString& tableName, DBFilter filtro);
    DBTable getTable(const QString& query);
    bool resetDatabase(const QString& dbPath = QString());
    QString TableName(TypeBDEnum tipo, DBFilter filtro);
};

#endif // DBMANAGER_H
