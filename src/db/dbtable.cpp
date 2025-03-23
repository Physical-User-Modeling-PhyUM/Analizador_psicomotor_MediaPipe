#include "dbtable.h"

BDTable::BDTable(QString tableName):tableName(tableName) {}


QVariant BDTable::value(int row, const QString& column) const
{
    int colIndex = columns.indexOf(column);

    if (colIndex < 0 || row < 0 || row >= rows.size())
        return QVariant();
    return rows[row][colIndex];
}

int  BDTable::rowCount() const { return rows.size();}


int BDTable::columnCount() const { return columns.size(); }

QStringList BDTable::getColumnsNames() const {return columns;}

QString BDTable::getColumnName(int columnNumber) const{
    return columns[columnNumber];
}

void BDTable::setColumns(const QStringList& colNames) {
    columns = colNames;
}

void BDTable::addRow(const QList<QVariant>& rowData) {
    rows.append(rowData);
}
