#ifndef UITABLEMODEL_H
#define UITABLEMODEL_H

#include "core/validationmanager.h"
#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include <QStringList>

class UiTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit UiTableModel(QObject *parent = nullptr);

    void setDataSet(const QList<QList<QVariant>> &rows, const QStringList &headers);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QList<QList<QVariant>> getDataSet() const;

    void addRow(QList<QVariant> row);
    void setColumnReadOnly(int column, bool readOnly = true);
    void setValidationManager(QSharedPointer<ValidationManager> validator);

signals:
    void modelModified();


private:
    QList<QList<QVariant>> dataSet;
    QStringList colHeaders;
    QSet<int> readOnlyColumns;
    QSharedPointer<ValidationManager> validationManager;
};

#endif // UITABLEMODEL_H
