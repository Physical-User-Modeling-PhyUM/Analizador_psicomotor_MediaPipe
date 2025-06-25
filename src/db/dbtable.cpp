/**
 * @file dbtable.cpp
 * @brief Implementación de la clase DBTable.
 */

#include "dbtable.h"
/**
 * @brief Constructor que asigna un nombre lógico a la tabla.
 */
DBTable::DBTable(QString tableName):tableName(tableName) {}

/**
 * @brief Devuelve el valor de una celda por índice de fila y nombre de columna.
 */
QVariant DBTable::value(int row, const QString& column) const {
    int colIndex = -1;
    for (int i = 0; i < columns.size(); ++i) {

        if (columns[i].compare(column, Qt::CaseInsensitive) == 0) {
            colIndex = i;
            break;
        }
    }

    if (colIndex < 0 || row < 0 || row >= rows.size()) {
        qWarning() << "DBTable::value - columna no encontrada:" << column << "- columnas:" << columns;
        return QVariant();
    }

    return rows[row][colIndex];
}
/**
 * @brief Asigna un nuevo valor a una celda específica.
 */
void DBTable::setValue(int row, const QString& column, QVariant new_value) {
    int colIndex = -1;
    for (int i = 0; i < columns.size(); ++i) {

        if (columns[i].compare(column, Qt::CaseInsensitive) == 0) {
            colIndex = i;
            break;
        }
    }

    if (colIndex < 0) {
        qWarning() << "DBTable::setValue - Columna no encontrada:" << column;
        return;
    }

    if (row < 0 || row >= rows.size()) {
        qWarning() << "DBTable::setValue - Índice de fila fuera de rango:" << row;
        return;
    }

    if (colIndex >= rows[row].size()) {
        qWarning() << "DBTable::setValue - Índice de columna fuera de rango para la fila:" << colIndex;
        return;
    }

    rows[row][colIndex] = new_value;
}

/**
 * @brief Devuelve el número de filas en la tabla.
 */
int  DBTable::rowCount() const { return rows.size();}

/**
 * @brief Devuelve el número de columnas.
 */
int DBTable::columnCount() const { return columns.size(); }
/**
 * @brief Devuelve la lista de nombres de columnas.
 */
QStringList DBTable::getColumnsNames() const {return columns;}
/**
 * @brief Devuelve el nombre de una columna por su índice.
 */
QString DBTable::getColumnName(int columnNumber) const{
    return columns[columnNumber];
}
/**
 * @brief Establece los nombres de las columnas.
 */
void DBTable::setColumns(const QStringList& colNames) {
    columns = colNames;
}
/**
 * @brief Añade una fila completa a la tabla.
 */
void DBTable::addRow(const QList<QVariant>& rowData) {
    rows.append(rowData);
}
/**
 * @brief Recupera una fila completa como lista ordenada de valores.
 */
QList<QVariant> DBTable::getRow(int rowIndex) const {
    QList<QVariant> row;
    for (const QString& col : columns) {
        row.append(value(rowIndex, col));
    }
    return row;
}
/**
 * @brief Indica si la tabla no contiene filas.
 */
bool DBTable::isEmpty()
{
    return rows.isEmpty();
}

/**
 * @brief Verifica si existe un valor específico en una columna.
 */
bool DBTable::contains( const QString& column, QVariant value){
    int colIndex = -1;
    for (int i = 0; i < columns.size(); ++i) {
        if (columns[i].toLower() == column.toLower())
        {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        qWarning() << "DBTable::contains - columna no encontrada:" << column;
        return false;
    }

    for (const auto& row : rows) {
        if (row[colIndex] == value) {
            return true;
        }
    }

    return false;
}



/**
 * @brief Imprime en consola el contenido completo de la tabla.
 */
void DBTable::print() const {
    // Imprimir nombres de columnas
    qDebug()<<tableName;
    QString header;
    for (const QString& col : columns) {
        header =header + QString("%1\t").arg(col);
    }
    qDebug().noquote() << header;

    // Imprimir cada fila
    for (const auto& row : rows) {
        QString fila;
        for (const QVariant& val : row) {
            fila =fila + QString("%1\t").arg(val.toString());
        }
        qDebug().noquote() << fila;
    }
}
/**
 * @brief Devuelve el nombre lógico de la tabla.
 */
QString DBTable::getTableName() const
{
    return tableName;
}

