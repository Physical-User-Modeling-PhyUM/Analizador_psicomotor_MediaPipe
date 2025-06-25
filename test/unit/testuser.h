#ifndef TESTUSER_H
#define TESTUSER_H

#include <QObject>

/**
 * @file testuser.h
 * @brief Pruebas unitarias para la clase User.
 *
 * Esta clase contiene pruebas de unidad para validar la correcta creación,
 * modificación y acceso a los atributos de la clase User. Se incluyen pruebas
 * funcionales (caja negra) sobre la construcción y los métodos básicos.
 */
class TestUser : public QObject {
    Q_OBJECT

private slots:

    /**
     * @brief Inicializa el entorno de pruebas.
     *
     * Este método se ejecuta antes de cada prueba individual.
     * Se puede usar para crear estructuras comunes o imprimir cabecera.
     */
    void init_Test();

    /**
     * @brief Finaliza el entorno de pruebas.
     *
     * Se llama tras finalizar una prueba para limpieza o debug.
     */
    void finish_Test();

    /**
     * @brief Prueba de construcción válida del objeto User.
     *
     * Verifica que los datos suministrados se almacenan correctamente
     * al construir el objeto.
     *
     * @pre Se proveen datos válidos para ID, nombre, email y contraseña.
     * @post Los datos deben coincidir con los proporcionados.
     */
    void testConstructorValido();

    /**
     * @brief Verifica los métodos de modificación de campos clave.
     *
     * Se comprueba que los setters actualizan correctamente el estado
     * del objeto, y los getters reflejan esos cambios.
     *
     * @pre Se crea un usuario con datos iniciales.
     * @post Se esperan valores modificados en nombre y correo electrónico.
     */
    void testCamposEspecificos();
};

#endif // TESTUSER_H
