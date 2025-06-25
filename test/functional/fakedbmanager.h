
#ifndef FAKEDBMANAGER_H
#define FAKEDBMANAGER_H

#include "core/dbmanager.h"
#include "db/dbtable.h"
#include "enums/TypeBDEnum.h"
#include <QMap>
#include <QPair>



/**
 * @file fakedbmanager.h
 * @brief FakeDBManager para pruebas unitarias sin acceso a base de datos real.
 */

/**
 * @class FakeDBManager
 * @brief Implementación simulada de DBManager para pruebas unitarias.
 * Permite inyectar tablas fijas en memoria para simular respuestas sin acceso a base de datos real.
 */

class FakeDBManager : public DBManager {


public:
    /**
     * @brief Constructor del FakeDBManager.
     * @param parent Objeto padre opcional.
     */
    explicit FakeDBManager(QObject* parent = nullptr);
    /**
     * @brief Simula la obtención de una tabla.
     * @param tipo Tipo de entidad (tabla).
     * @param filtro Tipo de filtro .s
     * @param valorFiltro Valor del filtro .
     * @return Tabla simulada del tipo solicitado.
     */
    DBTable get(TypeBDEnum tipo, DBFilter filtro=DBFilter::none, const QString valorFiltro="")  override;
    /**
     * @brief Simula el guardado de una tabla.
     * @param tipo Tipo de entidad.
     * @param elem Tabla a guardar.
     * @param filtro Filtro opcional.
     * @param valorFiltro Valor del filtro.
     * @return true si la operación fue exitosa.
     */
    bool save(TypeBDEnum tipo, DBTable& elem, DBFilter filtro=DBFilter::none,  QString valorFiltro="")override;
    /**
     * @brief Simula la eliminación de filas de una tabla.
     * @param tipo Tipo de entidad.
     * @param filtro Filtro de eliminación.
     * @param valorFiltro Valor del filtro.
     * @return true si se eliminó la tabla.
     */
   bool deleteRow(TypeBDEnum tipo, DBFilter filtro,  QString& valorFiltro)override;
    /**
     * @brief Establece una tabla simulada para un tipo determinado.
     * @param tipo Tipo de entidad.
     * @param table Tabla simulada.
     */
    void setTestTable(TypeBDEnum tipo, const DBTable& table);
    /**
     * @brief Limpia todas las tablas simuladas.
     */
    void clearTables();
    /**
     * @brief Obtiene el siguiente Id libre según la tabla
     * @param tipo Tipo de entidad.
     * @return el id generado.
     */
    int generateID(TypeBDEnum tipo)override;

private:
    QMap<TypeBDEnum, DBTable> testTables;
};

#endif // FAKEDBMANAGER_H
