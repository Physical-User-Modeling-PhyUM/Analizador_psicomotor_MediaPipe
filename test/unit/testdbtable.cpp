#include "testdbtable.h"

// ------------------------
//  CAJA NEGRA
// ------------------------

void TestDBTable::testEquivalenceClass_validRowAccess()
{
    DBTable table("test");
    table.setColumns({"id", "nombre"});
    table.addRow({1, "Alice"});
    QCOMPARE(table.value(0, "id").toInt(), 1);
    QCOMPARE(table.value(0, "nombre").toString(), QString("Alice"));
}

void TestDBTable::testEquivalenceClass_invalidColumn()
{
    DBTable table("test");
    table.setColumns({"id"});
    table.addRow({1});
    QCOMPARE(table.value(0, "no_existe"), QVariant());
}

void TestDBTable::testBoundaryCondition_rowOutOfRange()
{
    DBTable table("test");
    table.setColumns({"x"});
    table.addRow({42});
    QCOMPARE(table.value(-1, "x"), QVariant());
    QCOMPARE(table.value(1, "x"), QVariant());
}

void TestDBTable::testBoundaryCondition_emptyTable()
{
    DBTable table("test");
    table.setColumns({"id", "val"});
    QCOMPARE(table.rowCount(), 0);
    QVERIFY(table.isEmpty());
}

void TestDBTable::testEmptyColumnsAndRows()
{
    DBTable table("test");
    QCOMPARE(table.columnCount(), 0);
    QCOMPARE(table.rowCount(), 0);
    table.addRow({}); // ¿se acepta fila vacía?
    QCOMPARE(table.rowCount(), 1); // permitido
}

// ------------------------
// CAJA BLANCA
// ------------------------

void TestDBTable::testLoop_columnSearchCaseInsensitive()
{
    DBTable table("test");
    table.setColumns({"Nombre"});
    table.addRow({"Carlos"});
    // Fuerza a recorrer todo `columns`
    QCOMPARE(table.value(0, "nombre").toString(), "Carlos");
}

void TestDBTable::testInternalState_addRowUpdatesCorrectly()
{
    DBTable table("test");
    table.setColumns({"a", "b"});
    QCOMPARE(table.rows.size(), 0);
    table.addRow({1, 2});
    QCOMPARE(table.rows.size(), 1);
    QCOMPARE(table.rows[0][1].toInt(), 2);
}

void TestDBTable::testLogicalCondition_columnIndexNegative()
{
    DBTable table("test");
    table.setColumns({"uno", "dos"});
    table.addRow({10, 20});
    QVERIFY(table.value(0, "tres").isNull());
}

void TestDBTable::testLoop_getRowIteratesAllColumns()
{
    DBTable table("test");
    table.setColumns({"c1", "c2", "c3"});
    table.addRow({10, 20, 30});
    QList<QVariant> fila = table.getRow(0);
    QCOMPARE(fila.size(), 3);
    QCOMPARE(fila[2].toInt(), 30);
}
