#ifndef TESTDBTABLE_H
#define TESTDBTABLE_H

#include <QObject>
#include <QTest>
#include "db/dbtable.h"

class TestDBTable : public QObject
{
    Q_OBJECT

private slots:
    // CAJA NEGRA
    void testEquivalenceClass_validRowAccess();       // Clases de equivalencia
    void testEquivalenceClass_invalidColumn();        // Columna no existente
    void testBoundaryCondition_rowOutOfRange();       // Valores límite de índice
    void testBoundaryCondition_emptyTable();          // Tabla sin filas
    void testEmptyColumnsAndRows();                   // Caso sin columnas ni filas

    // CAJA BLANCA
    void testLoop_columnSearchCaseInsensitive();      // Prueba de bucle en value()
    void testInternalState_addRowUpdatesCorrectly();  // Cobertura interna de rows
    void testLogicalCondition_columnIndexNegative();  // Cubrimiento lógico
    void testLoop_getRowIteratesAllColumns();         // Bucle en getRow
};

#endif // TESTDBTABLE_H
