#include "Sqlite3_Test.h"


 Sqlite3_Test::Sqlite3_Test(){}

int Sqlite3_Test::createDB()
{
    sqlite3* db;
    int result = sqlite3_open("test.db", &db);
    if (result != SQLITE_OK) {
        qDebug() << "Error al abrir la base de datos:" << sqlite3_errmsg(db);
        return 1;
    }

    const char* query = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)";
    char* errMsg = nullptr;
    result = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK) {
        qDebug() << "Error al ejecutar consulta:" << errMsg;
        sqlite3_free(errMsg);
    } else {
        qDebug() << "Tabla creada correctamente.";
    }

    sqlite3_close(db);
    return 0;
}
