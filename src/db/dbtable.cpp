#include "dbtable.h"

DBTable::DBTable(QString tableName):tableName(tableName) {}


QVariant DBTable::value(int row, const QString& column) const
{
    int colIndex = columns.indexOf(column);

    if (colIndex < 0 || row < 0 || row >= rows.size())
        return QVariant();
    return rows[row][colIndex];
}

int  DBTable::rowCount() const { return rows.size();}


int DBTable::columnCount() const { return columns.size(); }

QStringList DBTable::getColumnsNames() const {return columns;}

QString DBTable::getColumnName(int columnNumber) const{
    return columns[columnNumber];
}

void DBTable::setColumns(const QStringList& colNames) {
    columns = colNames;
}

void DBTable::addRow(const QList<QVariant>& rowData) {
    rows.append(rowData);
}

QList<QVariant> DBTable::getRow(int rowIndex) const {
    QList<QVariant> row;
    for (const QString& col : columns) {
        row.append(value(rowIndex, col));
    }
    return row;
}
