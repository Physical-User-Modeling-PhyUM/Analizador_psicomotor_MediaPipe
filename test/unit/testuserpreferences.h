#ifndef TESTUSERPREFERENCES_H
#define TESTUSERPREFERENCES_H

#include <QObject>

/**
 * @file testuserpreferences.h
 * @brief Pruebas unitarias para la clase UserPreferences.
 *
 * Esta clase valida la correcta construcción de preferencias de usuario
 * y el comportamiento de los métodos para interpretar datos de configuración.
 */
class TestUserPreferences : public QObject {
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
     * @brief Verifica el comportamiento de UserPreferences cuando se inicializa con datos vacíos.
     *
     * @post Se espera que los valores por defecto se asignen correctamente.
     */
    void testValoresPorDefecto();

    /**
     * @brief Verifica que UserPreferences interprete correctamente datos válidos desde el mapa.
     *
     * @pre El mapa contiene todos los campos esperados.
     * @post Los valores extraídos deben coincidir con los proporcionados.
     */
    void testInicializacionCompleta();
};

#endif // TESTUSERPREFERENCES_H
