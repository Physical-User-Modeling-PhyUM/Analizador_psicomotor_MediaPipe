/**
 * @file fakedbmanager.cpp
 * @brief Implementación de FakeDBManager.
 */
#include "fakedbmanager.h"

/**
 * @brief Constructor que inicializa el DBManager base con una ruta ficticia.
 * @param parent Objeto padre opcional.
 */
FakeDBManager::FakeDBManager(QObject* parent)
    : DBManager("test.db", parent) {}  // O ":memory:" si quieres simular SQLite en memoria

/**
 * @brief Devuelve la tabla simulada asociada al tipo dado, o una vacía por defecto.
 */
// DBTable FakeDBManager::get(TypeBDEnum tipo, DBFilter filtro, const QString valorFiltro)  {
//     return testTables.value(tipo, DBTable("empty"));
// }



DBTable FakeDBManager::get(TypeBDEnum tipo, DBFilter filtro,  QString valorFiltro) {

    QString tableName = TypeBDEnumToString(tipo);

    DBTable tabla = testTables.value(tipo, DBTable(tableName));

    //qDebug()<< "  FAKEDB-get tabla";
    //tabla.print();
    if (filtro == DBFilter::none || valorFiltro.isEmpty()) {
       // qDebug()<< "  FAKEDB-get tabla sin filtrar";
        return tabla;
    }

    QString campo = BDFilterToString(filtro);
    QStringList columnas = tabla.getColumnsNames();

    if (!columnas.contains(campo,Qt::CaseInsensitive)) {
        //qDebug()<< "  FAKEDB-get No contiene el campo:"<<campo;
        return DBTable(tableName);
    }

    DBTable tablaF(tableName);
    tablaF.setColumns(columnas);

    for (int i = 0; i < tabla.rowCount(); ++i) {
        // qDebug()<< "  FAKEDB-get filtrando tabla";
        if (tabla.value(i, campo).toString() == valorFiltro) {
          //  qDebug()<< "  FAKEDB-get Buscando:"<<valorFiltro<<"Encontrado:"<<tabla.value(i, campo).toString();
            tablaF.addRow(tabla.getRow(i));
        }
    }

    return tablaF;
}
/**
 * @brief Simula el guardado de una tabla para un tipo dado.
 */
bool FakeDBManager::save(TypeBDEnum tipo, DBTable& elem, DBFilter filtro, QString valorFiltro) {
    if (elem.rowCount() == 0) return false;

    QString tableName = TypeBDEnumToString(tipo);
    QStringList columnas = elem.getColumnsNames();
    QString idColumn = columnas.isEmpty() ? QString() : columnas.first();

    DBTable tabla = testTables.value(tipo, DBTable(tableName));
    DBTable tablaFus(tabla.getTableName());

    tablaFus.setColumns(columnas);


    for (int i = 0; i < tabla.rowCount(); ++i) {
        QVariant clave = tabla.value(i, idColumn);

        bool seReemplaza = false;
        for (int j = 0; j < elem.rowCount(); ++j) {
            if (clave == elem.value(j, idColumn)) {
                seReemplaza = true;
                break;
            }
        }

        if (!seReemplaza) {
            tablaFus.addRow(tabla.getRow(i));
        }
    }

    // Agregar filas nuevas (o reemplazadas)
    for (int i = 0; i < elem.rowCount(); ++i) {
        tablaFus.addRow(elem.getRow(i));
    }

    testTables.insert(tipo, tablaFus);
    return true;
}



/**
 * @brief Simula la eliminación de una tabla por tipo.
 */
bool FakeDBManager::deleteRow(TypeBDEnum tipo, DBFilter filtro, QString& valorFiltro) {
    if (!testTables.contains(tipo)) return false;

    DBTable tabla = testTables.value(tipo, DBTable(TypeBDEnumToString(tipo)));
    QString campo = BDFilterToString(filtro);
    QStringList columnas = tabla.getColumnsNames();

    // Buscar índice de la columna (insensible a mayúsculas)
    int colIndex = -1;
    for (int i = 0; i < columnas.size(); ++i) {
        if (columnas[i].compare(campo, Qt::CaseInsensitive) == 0) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        qWarning() << "[FakeDB] deleteRsow: columna no encontrada en tabla:" << campo;
        return false;
    }

    DBTable resultado(TypeBDEnumToString(tipo));
    resultado.setColumns(columnas);
    bool eliminado = false;

    for (int i = 0; i < tabla.rowCount(); ++i) {
        QList<QVariant> fila = tabla.getRow(i);

        if (fila.size() <= colIndex) {
            qWarning() << "[FakeDB] deleteRow: índice fuera de rango en fila" << i
                       << "| Esperado:" << columnas.size() << "| Recibido:" << fila.size();
            continue;
        }

        if (fila[colIndex].toString() != valorFiltro) {
            resultado.addRow(fila);
        } else {
            eliminado = true;
        }
    }

    if (eliminado) {
        testTables.insert(tipo, resultado);
    }

    return eliminado;
}

/**
 * @brief Establece una tabla de prueba para un tipo dado.
 */
void FakeDBManager::setTestTable(TypeBDEnum tipo, const DBTable& table) {

    testTables.insert(tipo, table);
}

/**
 * @brief Limpia todas las tablas simuladas.
 */
void FakeDBManager::clearTables() {
    testTables.clear();
}
/**
 * @brief Obtiene el id .
 */
int FakeDBManager::generateID(TypeBDEnum tipo){
    return testTables.value(tipo,DBTable("")).rowCount();

}
