#include "uitablemodel.h"

UiTableModel::UiTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

void UiTableModel::setDataSet(const QList<QList<QVariant>> &rows, const QStringList &headers) {
    beginResetModel();
    dataSet = rows;
    colHeaders = headers;
    endResetModel();
}

int UiTableModel::rowCount(const QModelIndex &) const {
    return dataSet.size();
}

int UiTableModel::columnCount(const QModelIndex &) const {
    return colHeaders.size();
}

QVariant UiTableModel::data(const QModelIndex &index, int role) const {
    // if (!index.isValid() || role != Qt::DisplayRole)
    //     return QVariant();

    // QList<QVariant> row = dataSet.at(index.row());
    // return index.column() < row.size() ? row.at(index.column()) : QVariant();
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QList<QVariant> row = dataSet.at(index.row());
        return index.column() < row.size() ? row.at(index.column()) : QVariant();
    }

    return QVariant();
}

QVariant UiTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal && section < colHeaders.size())
        return colHeaders[section];

    return QVariant();
}

Qt::ItemFlags UiTableModel::flags(const QModelIndex &index) const {

    if (!index.isValid())
        return Qt::NoItemFlags;
    //flag que no permite la edición de la columna
    Qt::ItemFlags defaultFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    if (readOnlyColumns.contains(index.column()))
        return defaultFlags;
    else
        return defaultFlags | Qt::ItemIsEditable;
}

bool UiTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (!index.isValid() || role != Qt::EditRole)
        return false;

    if (validationManager!=nullptr && index.column() < colHeaders.size()) {
        QString fieldName = colHeaders.at(index.column());

        UserField fieldU = UserFieldFromString(fieldName);
        ExEspecField fieldE=ExEspecFieldFromString(fieldName);
        WorkoutField fieldW=WorkoutFieldFromString(fieldName);

        if (fieldU!=UserField::Unknown){
            if (!validationManager->validate(fieldU, value)) {
                return false;
            }
        }else if (fieldE!=ExEspecField::Unknown){
            if (!validationManager->validate(fieldE, value)) {
                return false;
            }

        }else if (fieldW!=WorkoutField::Unknown){
            if (!validationManager->validate(fieldW, value)) {
                return false;
            }

        }
    }



    if (index.row() < dataSet.size() && index.column() < dataSet[index.row()].size()) {
        dataSet[index.row()][index.column()] = value;
        emit dataChanged(index, index);
        emit modelModified();
        return true;
    }

    return false;
}

QList<QList<QVariant>> UiTableModel::getDataSet() const {
    return dataSet;
}

void UiTableModel::addRow(QList<QVariant> row)
{
    int position = dataSet.size();
    beginInsertRows(QModelIndex(), position, position);
    dataSet.append(row);
    endInsertRows();
    emit modelModified();
}
void UiTableModel::setColumnReadOnly(int column, bool readOnly)
{
    if (readOnly)
        readOnlyColumns.insert(column);
    else
        readOnlyColumns.remove(column);
}

void UiTableModel::setValidationManager(QSharedPointer<ValidationManager> validator) {
    this->validationManager = validator;
}
