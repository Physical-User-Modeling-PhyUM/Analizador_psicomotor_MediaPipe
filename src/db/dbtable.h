/**
 * @file dbtable.h
 * @brief Declaración de la clase DBTable, estructura auxiliar para manipular datos tabulares en memoria.
 *
 * Esta clase ofrece una representación simple de una tabla relacional,
 * permitiendo acceder, modificar y consultar filas y columnas utilizando nombres de columnas.
 * Se utiliza principalmente como intermediario entre la base de datos y las capas superiores de la aplicación.
 */

#ifndef DBTABLE_H
#define DBTABLE_H

#include <QStringList>
#include <QVariant>

/**
 * @class DBTable
 * @brief Representación en memoria de una tabla con columnas y filas tipo QVariant.
 *
 * Proporciona métodos para insertar y acceder a datos tabulares de forma estructurada.
 * Útil para gestionar datos provenientes de SQLite u otros motores relacionales en la lógica de la aplicación.
 */
class DBTable
{
public:
    /**
     * @brief Constructor que inicializa el nombre de la tabla.
     * @param tableName Nombre lógico de la tabla.
     */
    explicit DBTable(QString tableName);

    /**
     * @brief Devuelve el valor de una celda por fila y nombre de columna.
     * @param row Índice de fila.
     * @param column Nombre de la columna.
     * @return Valor contenido en la celda como QVariant.
     */
    QVariant value(int row, const QString& column) const;

    /**
     * @brief Establece el valor de una celda.
     * @param row Índice de fila.
     * @param column Nombre de la columna.
     * @param new_value Nuevo valor a asignar.
     */
    void setValue(int row, const QString& column, QVariant new_value);

    /**
     * @brief Devuelve el número de filas actuales en la tabla.
     */
    int rowCount() const;

    /**
     * @brief Devuelve el número de columnas.
     */
    int columnCount() const;

    /**
     * @brief Devuelve la lista de nombres de columnas.
     */
    QStringList getColumnsNames() const;

    /**
     * @brief Devuelve el nombre de la columna dada su posición.
     * @param columnNumber Índice de la columna.
     * @return Nombre de la columna.
     */
    QString getColumnName(int columnNumber) const;

    /**
     * @brief Establece los nombres de las columnas.
     * @param colNames Lista de nombres de columnas.
     */
    void setColumns(const QStringList& colNames);

    /**
     * @brief Añade una nueva fila a la tabla.
     * @param rowData Lista de valores a insertar.
     */
    void addRow(const QList<QVariant>& rowData);

    /**
     * @brief Devuelve los valores de una fila como lista ordenada.
     * @param rowIndex Índice de la fila.
     */
    QList<QVariant> getRow(int rowIndex) const;

    /**
     * @brief Verifica si la tabla está vacía.
     * @return true si no contiene filas.
     */
    bool isEmpty();

    /**
     * @brief Verifica si la tabla contiene un valor dado en una columna.
     * @param column Nombre de la columna.
     * @param value Valor a buscar.
     * @return true si el valor existe.
     */
    bool contains(const QString& column, QVariant value);

    /**
     * @brief Imprime el contenido de la tabla en consola (debug).
     */
    void print() const;

    /**
     * @brief Devuelve el nombre de la tabla.
     */
    QString getTableName() const;

    friend class TestDBTable;

private:
    QString tableName;                     ///< Nombre lógico de la tabla.
    QStringList columns;                   ///< Lista de nombres de columnas.
    QList<QList<QVariant>> rows;           ///< Datos tabulares: filas con valores ordenados por columna.

    // Alternativas futuras:
    // QList<QHash<QString, QVariant>> rows;
    // QHash<int, QHash<QString, QVariant>> rows;
};

#endif // DBTABLE_H
