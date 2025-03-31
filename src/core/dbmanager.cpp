#include "dbmanager.h"
#include <QDebug>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QCoreApplication>

DBManager::DBManager(const QString& dbPath, QObject* parent)
    : QObject(parent) {
    openDatabase(dbPath);
}

DBManager::~DBManager() {
    closeDatabase();
}

bool DBManager::openDatabase(const QString& dbPath) {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        qdb = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        qdb = QSqlDatabase::addDatabase("QSQLITE");
        qdb.setDatabaseName(dbPath);
    }

    if (!qdb.open()) {
        qWarning() << "Error abriendo la base de datos con Qt:" << qdb.lastError().text();
        return false;
    }

    return true;
}

void DBManager::closeDatabase() {
    if (qdb.isOpen()) {
        qdb.close();
    }
}

QStringList DBManager::getTableColumns(const QString& tableName) {
    QString query = QString("PRAGMA table_info(%1)").arg(tableName);
    DBTable info = getTable(query);

    QStringList columnas;
    for (int i = 0; i < info.rowCount(); ++i) {
        columnas << info.value(i, "name").toString();
    }
    return columnas;
}

bool DBManager::isValidFilterForTable(const QString& tableName, DBFilter filtro) {

    QString nombreColumna = BDFilterToString(filtro);

    if (nombreColumna.isEmpty()) return false;

    QSqlQuery query(qdb);
    QString sql = QString("PRAGMA table_info(%1)").arg(tableName);

    if (!query.exec(sql)) {
        qWarning() << "Error obteniendo la info de la tabla:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        // el indice 1 guarda los nombres de los campos
        QString columnName = query.value(1).toString();
        if (columnName == nombreColumna) {
            return true;
        }
    }

    return false;
}

DBTable DBManager::getTable(const QString& queryStr) {
    DBTable table("QueryResult");
    QSqlQuery query(qdb);

    if (!query.exec(queryStr)) {
        qWarning() << "Error ejecutando la consulta:" << query.lastError().text();
        return table;
    }

    QStringList columnNames;
    for (int i = 0; i < query.record().count(); ++i) {
        columnNames << query.record().fieldName(i);
    }
    table.setColumns(columnNames);

    while (query.next()) {
        QList<QVariant> row;
        for (int i = 0; i < columnNames.size(); ++i) {
            row.append(query.value(i));
        }
        table.addRow(row);
    }

    return table;
}

bool DBManager::resetDatabase(const QString& dbPath) {
    closeDatabase();

    QString connName = qdb.connectionName();
    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase::removeDatabase(connName);
    }

    return openDatabase(dbPath.isEmpty() ? qdb.databaseName() : dbPath);
}

QString DBManager::TableName(TypeBDEnum tipo, DBFilter filtro){
    QString tableName;
    // Determinar nombre de tabla según tipo y filtro
    switch (tipo) {
    case TypeBDEnum::User: tableName = "users"; break;
    case TypeBDEnum::Client: tableName = "clients"; break;
    case TypeBDEnum::Trainer: tableName = "trainers"; break;
    case TypeBDEnum::Admin: tableName = "admins"; break;
    case TypeBDEnum::Preferences: tableName = "userpreferences"; break;
    case TypeBDEnum::ExerciseEspec: tableName = "exerciseespecs"; break;
    case TypeBDEnum::TrainingWorkout:tableName = "trainingworkouts";break;
    case TypeBDEnum::WorkoutExercises:tableName ="workout_exerciseespec" ;break;
    case TypeBDEnum::ProfileFunctional: tableName = "client_functional"; break;
    case TypeBDEnum::ProfileMorphology: tableName = "client_morphology"; break;
    case TypeBDEnum::ProfileROM: tableName = "client_rom"; break;
    case TypeBDEnum::ProfileLifestyle: tableName = "client_lifestyle"; break;
    case TypeBDEnum::ProfileMedical: tableName = "client_medical"; break;
    case TypeBDEnum::ProfileGeneral: tableName = "client_general"; break;

    default:
        tableName = "";
    }
    return tableName;
}

bool DBManager::save(TypeBDEnum tipo, DBTable& elem, DBFilter filtro, QString valorFiltro) {

    QString tableName=TableName(tipo,filtro);
    if (tableName=="") return false;

    if (elem.rowCount() == 0) return false;

    QStringList columns = elem.getColumnsNames();
    QString fieldList = columns.join(", ");

    for (int row = 0; row < elem.rowCount(); ++row) {
        QList<QVariant> valores = elem.getRow(row);

        // Formatear valores con comillas simples escapadas
        QStringList valoresFormateados;
        for (const QVariant& val : valores) {
            QString valorSeguro = val.toString().replace("'", "''"); // escapamos comillas simples
            valoresFormateados << QString("'%1'").arg(valorSeguro);
        }

        QString valueList = valoresFormateados.join(", ");

        QString queryStr;

        bool aplicarFiltro = !valorFiltro.isEmpty() &&
                             filtro != DBFilter::none &&
                             isValidFilterForTable(tableName, filtro);

        if (aplicarFiltro) {
            QString campoFiltro = BDFilterToString(filtro);
            QString filtroFormateado = valorFiltro.replace("'", "''");
            queryStr = QString("REPLACE INTO %1 (%2) VALUES (%3) WHERE %4 = '%5'")
                           .arg(tableName, fieldList, valueList, campoFiltro, filtroFormateado);
        } else {
            queryStr = QString("REPLACE INTO %1 (%2) VALUES (%3)")
            .arg(tableName, fieldList, valueList);
        }

        QSqlQuery query(qdb);
        if (!query.exec(queryStr)) {
            qWarning() << "Error ejecutando la consulta REPLACE INTO en el método save():"<< query.lastError().text();
            return false;
        }
    }

    return true;
}


DBTable DBManager::get(TypeBDEnum tipo, DBFilter filtro,  QString valorFiltro) {

    QString tableName=TableName(tipo,filtro);
    if (tableName=="") return DBTable("error");

    // Contruimos la consulta
    QString queryStr;
    bool aplicarFiltro = !valorFiltro.isEmpty() &&
                         filtro != DBFilter::none &&
                         isValidFilterForTable(tableName, filtro);

    if (aplicarFiltro) {
        QString campo = BDFilterToString(filtro);
        QString valorEscapado = valorFiltro.replace("'", "''");
        queryStr = QString("SELECT * FROM %1 WHERE %2 = '%3'")
                       .arg(tableName, campo, valorEscapado);
    } else {
        queryStr = QString("SELECT * FROM %1").arg(tableName);
    }

    // Se ejecuta la consulta
    DBTable table(tableName);
    QSqlQuery query(qdb);

    if (!query.exec(queryStr)) {
        qWarning() << "Error ejecutando SELECT en get():" << query.lastError().text();
        return table;
    }

    // Obtener nombres de columnas
    QStringList columnas;
    for (int i = 0; i < query.record().count(); ++i) {
        columnas << query.record().fieldName(i);
    }
    table.setColumns(columnas);

    // Añadir filas
    while (query.next()) {
        QList<QVariant> fila;
        for (int i = 0; i < columnas.size(); ++i) {
            fila.append(query.value(i));
        }
        table.addRow(fila);
    }

    return table;
}




bool DBManager::initializeSchema() {

    QString path = QCoreApplication::applicationDirPath() + "/config/schema.sql";
    QFile sqlFile(path);
    //QFile sqlFile("config/schema.sql");

    if (!sqlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo SQL:" << sqlFile.errorString();
        return false;
    }

    QTextStream in(&sqlFile);
    QString sqlContent = in.readAll();
    sqlFile.close();

    QSqlQuery query;
    QStringList statements = sqlContent.split(';', Qt::SkipEmptyParts);

    for (QString stmt : statements) {
        stmt = stmt.trimmed();

        if (!stmt.isEmpty()) {
            if (!query.exec(stmt)) {
                qDebug() << "Error ejecutando:" << stmt;
                qDebug() << "Detalles:" << query.lastError().text();
                return false;
            }
        }
    }

    return true;
}

bool DBManager::deleteRow(TypeBDEnum tipo, DBFilter filtro,  QString& valorFiltro) {

    QString tableName=TableName(tipo,filtro);
    if (tableName=="") return false;

    if (!isValidFilterForTable(tableName, filtro)) return false;

    QString campo = BDFilterToString(filtro);
    QString valorEscapado = valorFiltro.replace("'", "''");

    QString queryStr = QString("DELETE FROM %1 WHERE %2 = '%3'")
                           .arg(tableName, campo, valorEscapado);

    QSqlQuery query(qdb);
    if (!query.exec(queryStr)) {
        qWarning() << "Error ejecutando DELETE:" << query.lastError().text();
        return false;
    }

    return true;
}



// bool DBManager::initializeSchema() {

//     QStringList schema = {

//     //Definimos el esquema que gestiona los usuarios
//     R"(CREATE TABLE IF NOT EXISTS users (
//            idUser INTEGER PRIMARY KEY,
//            userName TEXT NOT NULL,
//            email TEXT NOT NULL UNIQUE,
//            password TEXT NOT NULL,
//            userType TEXT NOT NULL,
//            join_up_date TEXT,
//            last_login TEXT,
//            profile_picture BLOB
//     ))",

    //         R"(CREATE TABLE IF NOT EXISTS clients (
    //            idClient INTEGER PRIMARY KEY,
    //            idUser INTEGER NOT NULL,
    //            expLevel INTEGER,
    //            age INTEGER,
    //            sex TEXT,
    //            weight REAL,
    //            height REAL,
    //            plan TEXT,
    //            FOREIGN KEY (idUser) REFERENCES users(idUser)
    //     ))",

    //         R"(CREATE TABLE IF NOT EXISTS fitnesstrainers (
    //            idTrainer INTEGER PRIMARY KEY,
    //            idUser INTEGER NOT NULL,
    //            qualifications TEXT,
    //            specialtyArea TEXT,
    //            experience_years INTEGER,
    //            resumme TEXT,
    //            FOREIGN KEY (idUser) REFERENCES users(idUser)
    //     ))",

    //         R"(CREATE TABLE IF NOT EXISTS admins (
    //            idAdmin INTEGER PRIMARY KEY,
    //            idUser INTEGER NOT NULL,
    //            roleDescription TEXT,
    //            canManageUsers BOOLEAN,
    //            canModifyPlans BOOLEAN,
    //            FOREIGN KEY (idUser) REFERENCES users(idUser)
    //     ))",

    //         R"(CREATE TABLE IF NOT EXISTS userpreferences (
    //            idPref INTEGER PRIMARY KEY,
    //            idUser INTEGER NOT NULL,
    //            language TEXT,
    //            notifications BOOLEAN,
    //            units TEXT,
    //            FOREIGN KEY (idUser) REFERENCES users(idUser)
    //     ))",

    //         R"(CREATE TABLE IF NOT EXISTS bodyrecords (
    //            id INTEGER PRIMARY KEY AUTOINCREMENT,
    //            idClient INTEGER NOT NULL,
    //            metric TEXT,
    //            value TEXT,
    //            FOREIGN KEY (idClient) REFERENCES clients(idClient)
    //     ))",

    //         R"(CREATE TABLE IF NOT EXISTS lifestylerecords (
    //            id INTEGER PRIMARY KEY AUTOINCREMENT,
    //            idClient INTEGER NOT NULL,
    //            type TEXT,
    //            value TEXT,
    //            FOREIGN KEY (idClient) REFERENCES clients(idClient)
    //     ))",

    //         R"(CREATE TABLE IF NOT EXISTS medicalconditions (
    //            id INTEGER PRIMARY KEY AUTOINCREMENT,
    //            idClient INTEGER NOT NULL,
    //            condition TEXT,
    //            value TEXT,
    //            FOREIGN KEY (idClient) REFERENCES clients(idClient)
    //     ))",

    //     //Definimos el esquema que gestiona los entrenamientos
    //         R"(CREATE TABLE IF NOT EXISTS exerciseespecs (
    //            idEx INTEGER PRIMARY KEY,
    //            name TEXT,
    //            description TEXT,
    //            exerciseType TEXT,
    //            targetMuscle TEXT,
    //            equipment TEXT,
    //            series INTEGER,
    //            repetitions INTEGER,
    //            duration INTEGER,
    //            weight INTEGER,
    //            RestTime INTEGER,


    //     ))",

    //     //Definimos el esquema que gestiona las métricas

    // };




    //     for (const QString& sql : schema) {
    //         if (!executeQuery(sql)) {
    //             qWarning() << "Error creando tabla con SQL:" << sql;
    //             return false;
    //         }
    //     }
    //     return true;
    // }
