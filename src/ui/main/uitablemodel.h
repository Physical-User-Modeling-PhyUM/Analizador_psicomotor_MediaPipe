/**
 * @file uitablemodel.h
 * @brief Declaración de la clase UiTableModel, modelo de tabla para representar y editar datos en la interfaz gráfica.
 *
 * Esta clase extiende `QAbstractTableModel` y proporciona un modelo de datos bidimensional utilizado principalmente
 * en componentes de tipo `QTableView`. Permite cargar, editar y validar datos estructurados por filas y columnas.
 * Incorpora mecanismos para marcar columnas como de solo lectura y conectar un gestor de validación externo para
 * comprobar las modificaciones realizadas por el usuario.
 */

#ifndef UITABLEMODEL_H
#define UITABLEMODEL_H

#include "core/validationmanager.h"
#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include <QStringList>

/**
 * @class UiTableModel
 * @brief Modelo de tabla personalizable utilizado en la interfaz de usuario.
 *
 * Esta clase permite representar conjuntos de datos tabulares en vistas tipo `QTableView`, facilitando la edición,
 * validación y manipulación de los mismos. Cada celda se almacena como un `QVariant`, y es posible definir
 * encabezados personalizados, así como columnas de solo lectura. Puede integrarse con un `ValidationManager`
 * para restringir la entrada de datos a través de reglas definidas.
 */
class UiTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /**
     * @brief Constructor explícito.
     * @param parent Widget padre opcional.
     */
    explicit UiTableModel(QObject *parent = nullptr);

    /**
     * @brief Establece el conjunto de datos y los encabezados del modelo.
     * @param rows Lista de filas, donde cada fila es una lista de `QVariant`.
     * @param headers Encabezados de columna.
     */
    void setDataSet(const QList<QList<QVariant>> &rows, const QStringList &headers);

    /// @copydoc QAbstractTableModel::rowCount
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /// @copydoc QAbstractTableModel::columnCount
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /// @copydoc QAbstractTableModel::data
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /// @copydoc QAbstractTableModel::headerData
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /// @copydoc QAbstractTableModel::flags
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /// @copydoc QAbstractTableModel::setData
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /**
     * @brief Devuelve el conjunto de datos completo.
     * @return Lista de filas, cada una con sus valores como `QVariant`.
     */
    QList<QList<QVariant>> getDataSet() const;

    /**
     * @brief Añade una nueva fila al conjunto de datos.
     * @param row Fila representada como una lista de `QVariant`.
     */
    void addRow(QList<QVariant> row);

    /**
     * @brief Establece si una columna es de solo lectura.
     * @param column Índice de la columna.
     * @param readOnly Verdadero si se desea que la columna sea de solo lectura.
     */
    void setColumnReadOnly(int column, bool readOnly = true);

    /**
     * @brief Asocia un gestor de validación para validar datos introducidos.
     * @param validator Puntero al `ValidationManager`.
     */
    void setValidationManager(QSharedPointer<ValidationManager> validator);

signals:
    /**
     * @brief Señal emitida cuando el modelo ha sido modificado por el usuario.
     */
    void modelModified();

private:
    QList<QList<QVariant>> dataSet; ///< Conjunto de datos organizado en filas y columnas.
    QStringList colHeaders; ///< Encabezados de columna.
    QSet<int> readOnlyColumns; ///< Conjunto de columnas marcadas como solo lectura.
    QSharedPointer<ValidationManager> validationManager; ///< Manejador externo de validación de datos.
};

#endif // UITABLEMODEL_H
