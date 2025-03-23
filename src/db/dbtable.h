#ifndef DBTABLE_H
#define DBTABLE_H

#include <QStringList>
#include <QVariant>

class DBTable
{
public:
    explicit DBTable(QString tableName);

    QVariant value(int row, const QString& column) const;
    int rowCount() const;
    int columnCount() const;
    QStringList getColumnsNames()const ;
    QString getColumnName(int columnNumber) const;
    void setColumns(const QStringList& colNames);
    void addRow(const QList<QVariant>& rowData) ;

private:
    QString tableName;
    QStringList columns;
    QList<QList<QVariant>> rows;
};

#endif // DBTABLE_H
