/**
 * @file dbmanager.cpp
 * @brief Implementación de la clase DBManager, responsable de gestionar el acceso a la base de datos SQLite.
 *
 * Esta clase actúa como un wrapper sobre QSqlDatabase y proporciona métodos para acceder,
 * insertar, actualizar y eliminar datos mediante el uso de la clase auxiliar DBTable.
 * Adicionalmente, permite filtrar resultados por distintos campos definidos en DBFilter
 * y gestionar el esquema de la base de datos desde archivos SQL externos.
 */

#include "dbmanager.h"
#include <QDebug>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QCoreApplication>

// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(DBManagerLog, "DBManager")

/**
 * @brief Constructor de DBManager.
 * @param dbPath Ruta del archivo de base de datos SQLite.
 * @param parent Objeto padre de Qt.
 */
DBManager::DBManager(const QString& dbPath, QObject* parent)
    : QObject(parent) {
    openDatabase(dbPath);
}
/**
 * @brief Destructor de DBManager.
 *
 * Cierra la base de datos si está abierta.
 */
DBManager::~DBManager() {
    closeDatabase();
}
/**
 * @brief Abre la conexión con la base de datos SQLite.
 * @param dbPath Ruta al archivo de la base de datos.
 * @return true si la base de datos se abre correctamente, false en caso contrario.
 */
bool DBManager::openDatabase(const QString& dbPath) {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        qdb = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        qdb = QSqlDatabase::addDatabase("QSQLITE");
        qdb.setDatabaseName(dbPath);
    }

    if (!qdb.open()) {
        qWarning(DBManagerLog) << "Error abriendo la base de datos con Qt:" << qdb.lastError().text();
        return false;
    }

    return true;
}
/**
 * @brief Cierra la conexión actual con la base de datos.
 */
void DBManager::closeDatabase() {
    if (qdb.isOpen()) {
        qdb.close();
    }
}
/**
 * @brief Obtiene la lista de columnas de una tabla SQLite.
 * @param tableName Nombre de la tabla.
 * @return Lista con los nombres de las columnas.
 */
QStringList DBManager::getTableColumns(const QString& tableName) {
    QString query = QString("PRAGMA table_info(%1)").arg(tableName);
    DBTable info = getTable(query);

    QStringList columnas;
    for (int i = 0; i < info.rowCount(); ++i) {
        columnas << info.value(i, "name").toString();
    }
    return columnas;
}
/**
 * @brief Verifica si un filtro es aplicable a una tabla.
 * @param tableName Nombre de la tabla.
 * @param filtro Filtro de tipo DBFilter.
 * @return true si el filtro es válido para esa tabla, false en caso contrario.
 */
bool DBManager::isValidFilterForTable(const QString& tableName, DBFilter filtro) {

    QString nombreColumna = BDFilterToString(filtro);

    if (nombreColumna.isEmpty()) return false;

    QSqlQuery query(qdb);
    QString sql = QString("PRAGMA table_info(%1)").arg(tableName);

    if (!query.exec(sql)) {
        qWarning(DBManagerLog) << "Error obteniendo la info de la tabla:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        // el indice 1 guarda los nombres de los campos paa la tabla
        QString columnName = query.value(1).toString();
        if (columnName.compare(nombreColumna, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }

    return false;
}
/**
 * @brief Ejecuta una consulta SQL y devuelve los resultados como un DBTable.
 * @param queryStr Cadena SQL de la consulta.
 * @return Objeto DBTable con el resultado de la consulta.
 */
DBTable DBManager::getTable(const QString& queryStr) {
    DBTable table("QueryResult");
    QSqlQuery query(qdb);

    if (!query.exec(queryStr)) {
        qWarning(DBManagerLog) << "Error ejecutando la consulta:" << query.lastError().text();
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
/**
 * @brief Reinicia la conexión a la base de datos, eliminando y reabriendo la conexión.
 * @param dbPath Ruta opcional al archivo de base de datos. Si está vacío se reutiliza el anterior.
 * @return true si la reconexión fue exitosa.
 */
bool DBManager::resetDatabase(const QString& dbPath) {
    closeDatabase();

    QString connName = qdb.connectionName();
    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase::removeDatabase(connName);
    }

    return openDatabase(dbPath.isEmpty() ? qdb.databaseName() : dbPath);
}
/**
 * @brief Devuelve el nombre de la tabla asociado a un tipo de base de datos.
 * @param tipo Enumerador TypeBDEnum.
 * @return Nombre de la tabla correspondiente.
 */
QString DBManager::TableName(TypeBDEnum tipo){
    QString tableName;
    // Determinar nombre de tabla según tipo y filtro
    switch (tipo) {
        case TypeBDEnum::User: tableName = "users"; break;
        case TypeBDEnum::Client: tableName = "clients"; break;
        case TypeBDEnum::Trainer: tableName = "fitnesstrainers"; break;
        case TypeBDEnum::Admin: tableName = "admins"; break;
        case TypeBDEnum::Preferences: tableName = "userpreferences"; break;
         case TypeBDEnum::ClientWorkoutCalendar: tableName = "client_workout_calendar"; break;

        case TypeBDEnum::ExerciseEspec: tableName = "exercise_espec"; break;
        case TypeBDEnum::TrainingWorkout:tableName = "training_workout";break;
        case TypeBDEnum::WorkoutExercises:tableName ="workout_exercise_espec" ;break;

        case TypeBDEnum::State: return "state";
        case TypeBDEnum::AngleConstraint: return "angle_constraint";
        case TypeBDEnum::Transition: return "transition";


        case TypeBDEnum::ProfileFunctional: tableName = "functional_records"; break;
        case TypeBDEnum::ProfileMorphology: tableName = "morfological_records"; break;
        case TypeBDEnum::ProfileROM: tableName = "rom_records"; break;
        case TypeBDEnum::ProfileLifestyle: tableName = "lifestyle_records"; break;
        case TypeBDEnum::ProfileMedical: tableName = "medical_records"; break;
        case TypeBDEnum::ProfileGeneral: tableName = "general_records"; break;
        case TypeBDEnum::ProfileAditional: tableName = "aditional_records"; break;

          case TypeBDEnum::TrainingSesion: tableName = "TrainingSesion"; break;
         case TypeBDEnum::ExerciseSummary: tableName = "ExerciseSummary"; break;
         case TypeBDEnum::SesionReport: tableName = "SesionReport"; break;
        default:
            tableName = "";
    }
    return tableName;
}
/**
 * @brief Devuelve el nombre de la columna clave primaria asociada al tipo.
 * @param tipo Enumerador TypeBDEnum.
 * @return Nombre del campo de ID.
 */
QString DBManager::getNameIdColumn(TypeBDEnum tipo) const {
    switch (tipo) {

    case TypeBDEnum::User:
    case TypeBDEnum::Client:
    case TypeBDEnum::Trainer:
    case TypeBDEnum::Admin:

        return "idUser";
    case TypeBDEnum::Preferences:
        return "idPref";
    case TypeBDEnum::ExerciseEspec:
        return "idEx";
    case TypeBDEnum::TrainingWorkout:
        return "idWorkout";
    case TypeBDEnum::State:
        return "idState";
    case TypeBDEnum::AngleConstraint:
        return "idState";
    case TypeBDEnum::Transition:
        return "idEx";
    case TypeBDEnum::WorkoutExercises:
        return "idWorkout";
    case TypeBDEnum::TrainingSesion:
        return "idSesion";
    case TypeBDEnum::ProfileGeneral:
    case TypeBDEnum::ProfileFunctional:
    case TypeBDEnum::ProfileMorphology:
    case TypeBDEnum::ProfileROM:
    case TypeBDEnum::ProfileLifestyle:
    case TypeBDEnum::ProfileMedical:
    case TypeBDEnum::ProfileAditional:
        return "idClient";
    default:
        return "";
    }
}

/**
 * @brief Inserta o actualiza filas en la base de datos desde un DBTable.
 *
 * Si se especifica un filtro válido, intenta hacer un UPDATE si ya existe una fila que coincide.
 * De lo contrario, realiza un INSERT o REPLACE según corresponda.
 *
 * @param tipo Tipo de entidad en la base de datos.
 * @param elem Objeto DBTable que contiene los datos a guardar.
 * @param filtro Filtro opcional que define una condición adicional.
 * @param valorFiltro Valor del filtro.
 * @return true si se ejecutó correctamente, false si hubo errores.
 */
bool DBManager::save(TypeBDEnum tipo, DBTable& elem, DBFilter filtro, QString valorFiltro) {
    QString tableName = TableName(tipo);

    if (tableName.isEmpty()) return false;
    if (elem.rowCount() == 0) return false;

    QStringList columns = elem.getColumnsNames();
    QString fieldList = columns.join(", ");
    QString idColumn = getNameIdColumn(tipo);

    for (int row = 0; row < elem.rowCount(); ++row) {
        QList<QVariant> valores = elem.getRow(row);


        QStringList valoresFormateados;

        for (const QVariant& val : valores) {

            QString valorSeguro = val.toString().replace("'", "''");
            valoresFormateados << QString("'%1'").arg(valorSeguro);
        }

        QString valueList = valoresFormateados.join(", ");

        bool filtroAplicable = !valorFiltro.isEmpty() &&
                             filtro != DBFilter::none &&
                             isValidFilterForTable(tableName, filtro);

        QSqlQuery query(qdb);
        QString queryStr;

        if (filtroAplicable) {
            QString campoFiltro = BDFilterToString(filtro);
            QString filtroFormateado = valorFiltro.replace("'", "''");

            // Obtenemos el  valor de la clave primaria para esa fila
            int idxClave = columns.indexOf(idColumn);
            if (idxClave == -1) {
                qWarning(DBManagerLog) << "No se encontró la columna clave primaria:" << idColumn;
                return false;
            }
            QString valorClave = valoresFormateados[idxClave];

            // ¿Existe ya una fila con ese filtro y esa clave?
            //Comprobacion para hacer un update o un insert
            QString checkQuery = QString("SELECT COUNT(*) FROM %1 WHERE %2 = '%3' AND %4 = %5").arg(tableName, campoFiltro, filtroFormateado, idColumn, valorClave);
            if (!query.exec(checkQuery)) {

                qWarning(DBManagerLog) << "Error ejecutando SELECT previo en save():" << query.lastError().text();
                return false;
            }

            query.next();
            int count = query.value(0).toInt();

            if (count > 0) {

                QStringList updateconditions;
                for (int i = 0; i < columns.size(); ++i) {
                    updateconditions << QString("%1 = %2").arg(columns[i], valoresFormateados[i]);
                }
                QString conditions = updateconditions.join(", ");
                queryStr = QString("UPDATE %1 SET %2 WHERE %3 = '%4' AND %5 = %6") .arg(tableName, conditions, campoFiltro, filtroFormateado, idColumn, valorClave);
            } else {

                queryStr = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(tableName, fieldList, valueList);
            }

        } else {
            // En caso de que el filtro no se aplique se modifican las filas con misma clave primarioa
            //Se agregarán las nuevas filas al hacer el replace
            queryStr = QString("REPLACE INTO %1 (%2) VALUES (%3)").arg(tableName, fieldList, valueList);
        }

        if (!query.exec(queryStr)) {
            qWarning(DBManagerLog) << "Error ejecutando la consulta en save():" << queryStr << " ->" << query.lastError().text();
            return false;
        }
    }

    return true;
}

/**
 * @brief Recupera datos desde la base de datos con filtro simple.
 * @param tipo Tipo de datos a recuperar.
 * @param filtro Campo por el cual se desea filtrar.
 * @param valorFiltro Valor del campo.
 * @return DBTable con los resultados.
 */
DBTable DBManager::get(TypeBDEnum tipo, DBFilter filtro,  QString valorFiltro) {

    QString tableName=TableName(tipo);
    if (tableName=="") return DBTable("error");

    // Contruimos la consulta para pbtener la tabl
    QString queryStr;
    bool aplicarFiltro = !valorFiltro.isEmpty() &&
                         filtro != DBFilter::none &&
                         isValidFilterForTable(tableName, filtro);

    if (aplicarFiltro) {
        QString campo = BDFilterToString(filtro);
        QString valorFOrmateado = valorFiltro.replace("'", "''");
        queryStr = QString("SELECT * FROM %1 WHERE %2 = '%3'") .arg(tableName, campo, valorFOrmateado);
    } else {
        queryStr = QString("SELECT * FROM %1").arg(tableName);
    }

    // Se ejecuta la consulta
    DBTable table(tableName);
    QSqlQuery query(qdb);

    if (!query.exec(queryStr)) {
        qWarning(DBManagerLog) << "Error ejecutando SELECT en get():" << query.lastError().text();
        return table;
    }

    // Obtenemos los  nombres de columnas para incorporarlos a como los nombres de campo de DBTable
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
/**
 * @brief Recupera datos usando un filtro con múltiples valores.
 * @param tipo Tipo de datos a recuperar.
 * @param filtro Campo por el cual se desea filtrar.
 * @param listaValores Lista de valores para aplicar un filtro IN.
 * @return DBTable con los resultados.
 */
DBTable DBManager::get(TypeBDEnum tipo, DBFilter filtro, const QStringList& listaValores) {
    QString tableName = TableName(tipo);
    if (tableName == "") return DBTable("error");

    QString queryStr;
    bool aplicarFiltro = !listaValores.isEmpty() &&
                         filtro != DBFilter::none &&
                         isValidFilterForTable(tableName, filtro);

    if (aplicarFiltro) {
        QString campo = BDFilterToString(filtro);

        QStringList valoresEscapados;
        for (const QString& v : listaValores) {
            valoresEscapados << "'" + v.trimmed().replace("'", "''") + "'";
        }

        QString inClause = valoresEscapados.join(",");
        queryStr = QString("SELECT * FROM %1 WHERE %2 IN (%3)").arg(tableName, campo, inClause);
    } else {
        queryStr = QString("SELECT * FROM %1").arg(tableName);
    }

    DBTable table(tableName);
    QSqlQuery query(qdb);

    if (!query.exec(queryStr)) {
        qWarning(DBManagerLog) << "Error ejecutando SELECT múltiple en get(): " << query.lastError().text();
        return table;
    }

    QStringList columnas;
    for (int i = 0; i < query.record().count(); ++i) {
        columnas << query.record().fieldName(i);
    }
    table.setColumns(columnas);

    while (query.next()) {
        QList<QVariant> fila;
        for (int i = 0; i < columnas.size(); ++i) {
            fila.append(query.value(i));
        }
        table.addRow(fila);
    }

    return table;
}
/**
 * @brief Recupera datos desde la base de datos aplicando múltiples filtros simultáneamente.
 * @param tipo Tipo de datos a recuperar.
 * @param filtros Mapa de filtros (campo-valor) a aplicar.
 * @return DBTable con los resultados.
 */
DBTable DBManager::get(TypeBDEnum tipo, const QMap<DBFilter, QVariant>& filtros) {
    QString tableName = TableName(tipo);
    if (tableName.isEmpty()) return DBTable("error");

    QStringList condiciones;

    for (auto it = filtros.constBegin(); it != filtros.constEnd(); ++it) {
        if (it.key() == DBFilter::none)
            continue;

        QString colName = BDFilterToString(it.key());

        if (!colName.isEmpty()) {
            QString cond =QString("%1 = '%2'").arg(colName, it.value().toString().replace("'", "''"));
            condiciones.append(cond);
        }
    }

    QString where = (condiciones.isEmpty()) ? "" : "WHERE " + condiciones.join(" AND ");

    QString queryStr = QString("SELECT * FROM %1 %2").arg(tableName, where);



    QSqlQuery query(qdb);

    if (!query.exec(queryStr)) {
        qWarning(DBManagerLog) << "Error ejecutando SELECT:" << queryStr << "::" << query.lastError().text();
        return DBTable("error");;
    }

    DBTable table(tableName);
    QStringList columnas;

    for (int i = 0; i < query.record().count(); ++i) {
        columnas << query.record().fieldName(i);
    }

    table.setColumns(columnas);

    while (query.next()) {

        QList<QVariant> fila;
        for (int i = 0; i < columnas.size(); ++i) {
            fila.append(query.value(i));
        }
        table.addRow(fila);
    }

    return table;
}
/**
 * @brief Ejecuta el script de inicialización del esquema de base de datos desde archivo.
 * @return true si se ejecutó correctamente, false si ocurrió un error.
 */
bool DBManager::initializeSchema() {

    QString path = QCoreApplication::applicationDirPath() + "/config/schema.sql";
    QFile sqlFile(path);
    //QFile sqlFile("config/schema.sql");

    if (!sqlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug(DBManagerLog) << "No se pudo abrir el archivo SQL:" << sqlFile.errorString();
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
/**
 * @brief Genera un nuevo ID libre para una tabla.
 *
 * Utiliza una consulta recursiva para buscar el menor entero positivo no utilizado como ID.
 * @param tipo Tipo de tabla.
 * @return Entero correspondiente al nuevo ID.
 */
int DBManager::generateID(TypeBDEnum tipo)
{
    // QString tableName = TableName(tipo);
    // QString idColumn = getNameIdColumn(tipo);  // Esta función debe mapear el tipo al nombre de la columna ID

    // QString queryStr = QString("SELECT MAX(%1) FROM %2").arg(idColumn, tableName);
    // QSqlQuery query;

    // if (!query.exec(queryStr)) {
    //     qCritical() << "DBManager::generateID - Error al ejecutar la consulta:" << query.lastError().text();
    //     return -1;
    // }

    // if (query.next()) {
    //     int maxId = query.value(0).toInt();
    //     return maxId + 1;
    // }

    // return 1;
    QString tableName = TableName(tipo);
    QString idColumn = getNameIdColumn(tipo);

    QString queryStr = QString(R"(
        WITH RECURSIVE seq(n) AS (
            SELECT 1
            UNION ALL
            SELECT n + 1 FROM seq WHERE n < 100000
        )
        SELECT n FROM seq
        WHERE n NOT IN (SELECT %1 FROM %2)
        ORDER BY n
        LIMIT 1;
    )").arg(idColumn, tableName);

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qCritical(DBManagerLog) << "DBManager::generateID  Error ejecutando consulta:" << query.lastError().text();
        return -1;
    }

    if (query.next() && query.value(0).isValid()) {
        return query.value(0).toInt();
    }

    return 1;

}

/**
 * @brief Elimina filas de una tabla usando un filtro simple.
 * @param tipo Tipo de entidad.
 * @param filtro Campo a aplicar.
 * @param valorFiltro Valor del filtro.
 * @return true si se eliminó correctamente.
 */
bool DBManager::deleteRow(TypeBDEnum tipo, DBFilter filtro,  QString& valorFiltro) {

    QString tableName=TableName(tipo);
    if (tableName=="") return false;

    if (!isValidFilterForTable(tableName, filtro)) return false;

    QString campo = BDFilterToString(filtro);
    QString valorEscapado = valorFiltro.replace("'", "''");

    QString queryStr = QString("DELETE FROM %1 WHERE %2 = '%3'").arg(tableName, campo, valorEscapado);

    QSqlQuery query(qdb);
    if (!query.exec(queryStr)) {
        qWarning(DBManagerLog) << "Error ejecutando DELETE:" << query.lastError().text();
        return false;
    }

    return true;
}
/**
 * @brief Elimina múltiples filas usando una lista de valores.
 * @param tipo Tipo de entidad.
 * @param filtro Campo a filtrar.
 * @param listaValores Lista de valores.
 * @return true si la operación fue exitosa.
 */
bool DBManager::deleteRow(TypeBDEnum tipo, DBFilter filtro, const QStringList& listaValores) {
    QString tableName = TableName(tipo);

    if (tableName == "") return false;
    if (!isValidFilterForTable(tableName, filtro)) return false;
    if (listaValores.isEmpty()) return true;

    QString campo = BDFilterToString(filtro);

    QStringList condiciones;
    for (QString v : listaValores) {
        condiciones << QString("'%1'").arg(v.replace("'", "''"));
    }

    QString whereClause = QString("%1 IN (%2)").arg(campo, condiciones.join(","));
    QString queryStr = QString("DELETE FROM %1 WHERE %2").arg(tableName, whereClause);

    QSqlQuery query(qdb);
    if (!query.exec(queryStr)) {
        qWarning(DBManagerLog) << "Error ejecutando DELETE :" << query.lastError().text();
        return false;
    }

    return true;
}
/**
 * @brief Elimina filas usando múltiples condiciones combinadas.
 * @param tipo Tipo de entidad.
 * @param filtros Mapa campo-valor a aplicar como condición AND.
 * @return true si se ejecutó correctamente.
 */
bool DBManager::deleteRow(TypeBDEnum tipo, const QMap<DBFilter, QVariant>& filtros) {
    QString tableName = TableName(tipo);
    if (tableName.isEmpty() || filtros.isEmpty()) return false;

    QStringList condiciones;
    for (auto it = filtros.constBegin(); it != filtros.constEnd(); ++it) {
        QString colName = BDFilterToString(it.key());
        if (!colName.isEmpty()) {
            QString cond=QString("%1 = '%2'").arg(colName, it.value().toString().replace("'", "''"));
            condiciones.append(cond);
        }
    }

    QString whereClause = condiciones.join(" AND ");
    QString queryStr = QString("DELETE FROM %1 WHERE %2").arg(tableName, whereClause);

    QSqlQuery query(qdb);
    if (!query.exec(queryStr)) {
        qWarning(DBManagerLog) << "Error ejecutando DELETE:" << queryStr << "::" << query.lastError().text();
        return false;
    }

    return true;
}

