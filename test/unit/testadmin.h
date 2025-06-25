#ifndef TESTADMIN_H
#define TESTADMIN_H

#include <QObject>

/**
 * @file testadmin.h
 * @brief Pruebas unitarias para la clase Admin.
 *
 * Esta clase prueba un objeto Admin
 * y la funcionalidad de los métodos relacionados, rol y atributos específicos del administrador.
 */
class TestAdmin : public QObject {
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
     * @brief Verifica que el constructor con QHash inicializa correctamente los campos.
     *
     * Incluye validación de campos heredados y específicos del administrador.
     *
     * @pre Se proveen todos los campos requeridos.
     * @post El objeto debe reflejar los valores insertados.
     */
    void testConstructorValido();


    /**
     * @brief Verifica que el constructor con QHash no inicializa la clase con campos incorrectos o faltantes.
     *
     * @pre No se proveen todos los campos requeridos o correctos.
     * @post El objeto debe estar vació, id =-1.
     */
    void test_ConstructorInvalido();
};

#endif // TESTADMIN_H
