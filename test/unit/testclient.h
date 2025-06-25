#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QObject>

/**
 * @file testclient.h
 * @brief Pruebas unitarias para la clase Client.
 *
 * Esta clase realiza pruebas funcionales y estructurales sobre la clase Client,
 * enfocándose en la gestión del calendario de entrenamientos y el vínculo
 * con el perfil del cliente.
 */
class TestClient : public QObject {
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
     * @brief Verifica que el constructor con QHash inicializa correctamente los campos heredados.
     *
     * @pre Se proporcionan campos mínimos válidos.
     * @post Los campos base deben estar correctamente inicializados.
     */
    void test_ConstructorValido();
    /**
     * @brief Verifica que el constructor con QHash no inicializa con datos inválidos.
     *
     * @pre Se proporcionan campos mínimos válidos.
     * @post Los campos base deben estar correctamente inicializados.
     */
    void test_ConstructorInvalido();
    /**
     * @brief Prueba la vinculación y recuperación de un perfil asociado.
     *
     * Se crea un perfil, se asigna al cliente y se comprueba su correcta vinculación.
     */
    void test_AsociacionPerfil();

    /**
     * @brief Verifica las operaciones del calendario de entrenamiento del cliente.
     *
     * Se insertan, consultan y eliminan sesiones en fechas específicas.
     */
    void test_CalendarioEntrenamientos();
};

#endif // TESTCLIENT_H
