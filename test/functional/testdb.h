#ifndef TESTDB_H
#define TESTDB_H
#include <QtTest>
#include "core/dbmanager.h"
#include "enums/TypeBDEnum.h"

class TestDB : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Caja Negra
    void testOpenDatabase();
    void testInitializeSchema();
    void testSaveAndGet();
    void testSaveWithFilter();
    void testGetWithMultipleValues();
    void testDeleteRow();
    void testGenerateID();

    // Caja Blanca (sobre método privado accesible)
    void testGetTableColumns();
    void testIsValidFilterForTable();

private:
    DBManager* dbManager;
    QString dbPath;
};
#endif // TESTDB_H
