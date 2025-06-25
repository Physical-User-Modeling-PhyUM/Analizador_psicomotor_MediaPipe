#ifndef TESTFITNESSTRAINER_H
#define TESTFITNESSTRAINER_H

#include <QObject>

/**
 * @file testfitnesstrainer.h
 * @brief Pruebas unitarias para la clase FitnessTrainer.
 *
 * Esta clase realiza pruebas sobre la inicialización de objetos
 * tipo entrenador personal, y los métodos específicos relacionados
 * con su experiencia profesional, área de especialización y resumen.
 */
class TestFitnessTrainer : public QObject {
    Q_OBJECT

private slots:

    /**
     * @brief Inicializa el entorno de pruebas.
     */
    void init_Test();

    /**
     * @brief Finaliza el entorno de pruebas.
     */
    void finish_Test();

    /**
     * @brief Verifica que el constructor inicializa correctamente los campos.
     *
     * @pre Se pasan todos los campos necesarios mediante QHash.
     * @post Los atributos del entrenador deben reflejar los valores recibidos.
     */
    void testConstructorValido();

    /**
     * @brief Verifica que el constructor con QHash no inicializa la clase con campos incorrectos o faltantes.
     *
     * @pre No se proveen todos los campos requeridos o correctos.
     * @post El objeto debe estar vació, id =-1.
     */
    void test_ConstructorInvalido();

    /**
     * @brief Verifica el comportamiento de los métodos de modificación y acceso a atributos específicos.
     *
     * Se prueban los métodos de modificación de especialidad, años de experiencia y resumen.
     */
    void testModificacionCamposProfesionales();
};

#endif // TESTFITNESSTRAINER_H
