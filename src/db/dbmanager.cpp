#include "dbmanager.h"
#include <QDebug>

DBManager::DBManager(const QString& dbPath, QObject* parent)
    : QObject(parent), db(nullptr) {
    openDatabase(dbPath);
}

DBManager::~DBManager() {
    closeDatabase();
}

bool DBManager::openDatabase(const QString& dbPath) {
    int result = sqlite3_open(dbPath.toUtf8().constData(), &db);
    if (result != SQLITE_OK) {
        qWarning() << "Error abriendo la base de datos:" << sqlite3_errmsg(db);
        return false;
    }
    return true;
}

void DBManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DBManager::executeQuery(const QString& query) {
    char* errMsg = nullptr;
    int result = sqlite3_exec(db, query.toUtf8().constData(), nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK) {
        qWarning() << "SQLite error:" << errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

BDTable DBManager::getData(const QString& query) {
    BDTable table("QueryResult");  // o puedes dejar tableName vacío
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, query.toUtf8().constData(), -1, &stmt, nullptr) == SQLITE_OK) {
        int cols = sqlite3_column_count(stmt);
        QStringList columnNames;

        // Obtener nombres de columnas
        for (int i = 0; i < cols; ++i) {
            QString colName = QString::fromUtf8(sqlite3_column_name(stmt, i));
            columnNames.append(colName);
        }
        table.setColumns(columnNames);

        // Leer filas
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            QList<QVariant> row;
            for (int i = 0; i < cols; ++i) {
                QVariant value;

                switch (sqlite3_column_type(stmt, i)) {
                case SQLITE_INTEGER:
                    value = sqlite3_column_int64(stmt, i);
                    break;
                case SQLITE_FLOAT:
                    value = sqlite3_column_double(stmt, i);
                    break;
                case SQLITE_TEXT:
                    value = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
                    break;
                case SQLITE_NULL:
                    value = QVariant();
                    break;
                default:
                    value = QVariant();  // fallback
                    break;
                }
                row.append(value);
            }
            table.addRow(row);
        }
    } else {
        qWarning() << "Fallo al ejecutar la consulta: " << query;
    }

    sqlite3_finalize(stmt);
    return table;
}
