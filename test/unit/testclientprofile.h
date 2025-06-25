#ifndef TESTCLIENTPROFILE_H
#define TESTCLIENTPROFILE_H

#include <QObject>

/**
 * @file testclientprofile.h
 * @brief Pruebas unitarias para la clase ClientProfile.
 *
 * Esta clase valida el comportamiento de los métodos de almacenamiento y recuperación
 * de registros funcionales, morfológicos, de rango de movimiento, médicos, de estilo
 * de vida y datos adicionales del perfil del cliente.
 */
class TestClientProfile : public QObject {
    Q_OBJECT

private slots:

    /**
     * @brief Inicializa el entorno de pruebas.
     */
    void init_Test();

    /**
     * @brief Limpieza posterior a cada prueba.
     */
    void finish_Test();

    /**
     * @brief Verifica el almacenamiento y recuperación de un registro funcional.
     */
    void testRegistroFuncional();

    /**
     * @brief Verifica que no se almacenan valores fuera de rango en registro funcional.
     */
    void test_ValoresFuncional_FueraDeRango();
    /**
     * @brief Verifica el almacenamiento y recuperación de un valor morfológico en el perfil.
     */
    void testRegistroMorfologico();

    /**
     * @brief Verifica que no se almacenan valores fuera de rango.
     */
    void test_Valores_Morfologia_FueraDeRango();

    /**
     * @brief Verifica la inserción y recuperación de rangos de movimiento (ROM).
     */
    void testROM();
    /**
     * @brief Verifica que no se almacenan valores fuera de rango.
     */
    void test_ValoresROM_FueraDeRango();

    /**
     * @brief Verifica la adición y recuperación de registros médicos.
     */
    void testRegistroMedico();

    /**
     * @brief Verifica los métodos de inserción y acceso a datos de estilo de vida.
     */
    void testRegistroEstiloVida();

    /**
     * @brief Verifica la inserción y recuperación de datos adicionales del perfil.
     */
    void testDatosAdicionales();
};

#endif // TESTCLIENTPROFILE_H
