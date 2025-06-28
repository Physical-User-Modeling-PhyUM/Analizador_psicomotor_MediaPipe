/**
 * @file testdb.h
 * @brief Declaración de la clase TestDB para pruebas unitarias de la clase DBManager.
 *
 * Esta clase implementa pruebas funcionales y estructurales para validar el comportamiento
 * de la clase `DBManager`, incluyendo la gestión de esquemas, inserciones, consultas,
 * eliminaciones y utilidades internas de la base de datos.
 */

#ifndef TESTDB_H
#define TESTDB_H

#include <QtTest>
#include "core/dbmanager.h"
#include "enums/TypeBDEnum.h"

/**
 * @class TestDB
 * @brief Clase de pruebas unitarias para el gestor de base de datos DBManager.
 *
 * Incluye pruebas de caja negra que verifican el comportamiento observable desde la interfaz
 * pública, y pruebas de caja blanca sobre métodos privados accesibles indirectamente.
 */
class TestDB : public QObject {
    Q_OBJECT

private slots:

    /**
     * @brief Inicializa el entorno antes de ejecutar la primera prueba.
     *
     * Crea una instancia de `DBManager` y define la ruta a la base de datos de pruebas.
     */
    void initTestCase();

    /**
     * @brief Limpia el entorno después de ejecutar todas las pruebas.
     *
     * Libera recursos como el puntero `DBManager` y borra la base de datos de pruebas.
     */
    void cleanupTestCase();

    // ==============================
    // === Pruebas de Caja Negra ===
    // ==============================

    /**
     * @brief Verifica que la base de datos se abra correctamente.
     */
    void testOpenDatabase();

    /**
     * @brief Comprueba que el esquema de base de datos se crea correctamente.
     */
    void testInitializeSchema();

    /**
     * @brief Valida la inserción y posterior recuperación de un registro.
     */
    void testSaveAndGet();

    /**
     * @brief Verifica que se puede aplicar un filtro en la operación de guardado.
     */
    void testSaveWithFilter();

    /**
     * @brief Comprueba la correcta recuperación de múltiples valores.
     */
    void testGetWithMultipleValues();

    /**
     * @brief Valida la recuperación y eliminación de registros usando múltiples filtros.
     */
    void testGetAndDeleteWithMultipleFilters();

    /**
     * @brief Verifica que un registro pueda ser eliminado correctamente.
     */
    void testDeleteRow();

    /**
     * @brief Prueba la generación automática de identificadores únicos.
     */
    void testGenerateID();

    // ===============================
    // === Pruebas de Caja Blanca ===
    // ===============================

    /**
     * @brief Verifica la obtención correcta de columnas de una tabla.
     *
     * Esta prueba accede indirectamente a un método privado mediante pruebas controladas.
     */
    void testGetTableColumns();

    /**
     * @brief Comprueba la validez de los filtros definidos para una tabla específica.
     */
    void testIsValidFilterForTable();

private:
    DBManager* dbManager; ///< Puntero al gestor de base de datos a probar.
    QString dbPath;       ///< Ruta al archivo de base de datos temporal utilizado en las pruebas.
};

#endif // TESTDB_H
