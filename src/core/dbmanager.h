/**
 * @file dbmanager.h
 * @brief Declaración de la clase DBManager para la gestión centralizada de la base de datos SQLite.
 *
 * Esta clase proporciona una interfaz genérica para realizar operaciones CRUD
 * sobre distintas tablas del sistema, usando `TypeBDEnum` para identificar las entidades.
 * Las operaciones son realizadas utilizando la clase auxiliar DBTable, lo que permite trabajar
 * con los datos sin acoplamiento directo con SQL.
 *
 * También ofrece utilidades como generación de ID únicos, validación de filtros y carga del esquema.
 *
 * @see DBTable, TypeBDEnum, DBFilter
 */

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "db/dbtable.h"
#include "enums/TypeBDEnum.h"

#include <QSqlDatabase>


#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(DBManagerLog);

/**
 * @class DBManager
 * @brief Clase responsable de gestionar la conexión y las operaciones de lectura/escritura en una base de datos SQLite.
 *
 * DBManager encapsula el uso de QSqlDatabase, permitiendo realizar operaciones de acceso,
 * actualización, inserción y eliminación sobre las entidades del sistema.
 * Usa `TypeBDEnum` para abstraer las tablas, y `DBFilter` para aplicar filtros sobre las consultas.
 * Las consultas se devuelven o reciben en formato `DBTable` para facilitar el desacoplamiento del backend.
 */
class DBManager : public QObject {
    Q_OBJECT


public:
    /**
     * @brief Constructor principal.
     * @param dbPath Ruta al archivo de base de datos SQLite.
     * @param parent Objeto padre Qt.
     */
    explicit DBManager(const QString& dbPath, QObject* parent = nullptr);

    /**
     * @brief Destructor. Cierra la base de datos si está abierta.
     */
    ~DBManager();

    /**
     * @brief Abre la base de datos desde una ruta específica.
     * @param dbPath Ruta del archivo `.db`.
     * @return true si la apertura fue exitosa.
     */
    bool openDatabase(const QString& dbPath);

    /**
     * @brief Cierra la conexión activa con la base de datos.
     */
    void closeDatabase();

    /**
     * @brief Recupera los datos de una tabla con un filtro opcional.
     * @param tipo Tipo de entidad (tabla).
     * @param filtro Campo por el cual filtrar.
     * @param valorFiltro Valor del filtro.
     * @return Objeto DBTable con los resultados.
     */
    virtual DBTable get(TypeBDEnum tipo, DBFilter filtro = DBFilter::none, const QString valorFiltro = "");

    /**
     * @brief Recupera datos filtrando por una lista de valores.
     * @param tipo Tipo de entidad.
     * @param filtro Campo de filtrado.
     * @param listaValores Lista de valores a aplicar con IN.
     * @return DBTable con los resultados.
     */
    DBTable get(TypeBDEnum tipo, DBFilter filtro, const QStringList& listaValores = {});

    /**
     * @brief Recupera datos usando múltiples condiciones.
     * @param tipo Tipo de entidad.
     * @param filtros Mapa de filtros y sus valores.
     * @return DBTable con los resultados.
     */
    DBTable get(TypeBDEnum tipo, const QMap<DBFilter, QVariant>& filtros);

    /**
     * @brief Guarda datos en la base de datos.
     * @param tipo Tipo de entidad.
     * @param elem Datos en formato DBTable.
     * @param filtro Filtro condicional opcional.
     * @param valorFiltro Valor del filtro.
     * @return true si se guardó correctamente.
     */
    virtual bool save(TypeBDEnum tipo, DBTable& elem, DBFilter filtro = DBFilter::none, QString valorFiltro = "");

    /**
     * @brief Elimina una fila con filtro simple.
     * @param tipo Tipo de entidad.
     * @param filtro Campo de filtrado.
     * @param valorFiltro Valor a comparar.
     * @return true si se eliminó correctamente.
     */
    virtual bool deleteRow(TypeBDEnum tipo, DBFilter filtro, QString& valorFiltro);

    /**
     * @brief Elimina múltiples filas usando una lista de valores.
     * @param tipo Tipo de entidad.
     * @param filtro Campo de filtrado.
     * @param listaValores Lista de valores.
     * @return true si se ejecutó correctamente.
     */
    bool deleteRow(TypeBDEnum tipo, DBFilter filtro, const QStringList& listaValores = {});

    /**
     * @brief Elimina filas con múltiples filtros.
     * @param tipo Tipo de entidad.
     * @param filtros Mapa de campo-valor.
     * @return true si se ejecutó correctamente.
     */
    bool deleteRow(TypeBDEnum tipo, const QMap<DBFilter, QVariant>& filtros);

    /**
     * @brief Inicializa el esquema de la base de datos a partir de un archivo SQL.
     * @return true si el esquema se cargó correctamente.
     */
    bool initializeSchema();

    /**
     * @brief Genera el primer ID disponible no usado en la tabla.
     * @param tipo Tipo de entidad.
     * @return Un nuevo ID libre, o -1 si hay error.
     */
    virtual int generateID(TypeBDEnum tipo);


    friend class TestDB;


private:
    QSqlDatabase qdb;  ///< Conexión a la base de datos SQLite.

    /**
     * @brief Devuelve las columnas de una tabla SQLite.
     * @param tableName Nombre de la tabla.
     * @return Lista de nombres de columnas.
     */
    QStringList getTableColumns(const QString& tableName);

    /**
     * @brief Valida si un filtro es aplicable a una tabla.
     * @param tableName Nombre de la tabla.
     * @param filtro Filtro DBFilter.
     * @return true si es válido.
     */
    bool isValidFilterForTable(const QString& tableName, DBFilter filtro);

    /**
     * @brief Ejecuta una consulta SQL libre y devuelve los resultados como DBTable.
     * @param query Cadena SQL.
     * @return DBTable con resultados.
     */
    DBTable getTable(const QString& query);

    /**
     * @brief Cierra y reabre la base de datos, útil para recargar el archivo.
     * @param dbPath Nueva ruta de base de datos, si aplica.
     * @return true si se reinició correctamente.
     */
    bool resetDatabase(const QString& dbPath = QString());

    /**
     * @brief Devuelve el nombre de la tabla correspondiente al tipo.
     * @param tipo Tipo de entidad.
     * @return Nombre de la tabla SQL.
     */
    QString TableName(TypeBDEnum tipo);

    /**
     * @brief Devuelve el nombre de la columna ID primaria para una tabla.
     * @param tipo Tipo de entidad.
     * @return Nombre del campo de clave primaria.
     */
    QString getNameIdColumn(TypeBDEnum tipo) const;


};

#endif // DBMANAGER_H
